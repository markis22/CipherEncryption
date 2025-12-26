/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include "xparameters.h"
#include "xil_printf.h"
#include "sleep.h"

#include "xuartlite.h"
#include "xuartlite_l.h"

#include "PmodOLED.h"

/* -----------------------------
   Adjust these if your xparameters.h names differ
   ----------------------------- */
#ifndef XPAR_AXI_UARTLITE_0_DEVICE_ID
#error "AXI UARTLite not found in xparameters.h. Make sure axi_uartlite_0 exists in Vivado and you re-exported hardware."
#endif

#define UARTLITE_DEVICE_ID   XPAR_AXI_UARTLITE_0_DEVICE_ID
#define UARTLITE_BASEADDR    XPAR_AXI_UARTLITE_0_BASEADDR

#ifndef XPAR_PMODOLED_0_AXI_LITE_GPIO_BASEADDR
#error "PmodOLED base addresses not found in xparameters.h. Make sure the PmodOLED IP is in the block design and you re-exported hardware."
#endif

#define OLED_GPIO_BASEADDR   XPAR_PMODOLED_0_AXI_LITE_GPIO_BASEADDR
#define OLED_SPI_BASEADDR    XPAR_PMODOLED_0_AXI_LITE_SPI_BASEADDR

/* OLED orientation/invert options used by Digilent driver */
#define OLED_ORIENT          0
#define OLED_INVERT          0

/* Message limits */
#define LINE_MAX             64
#define MSG_MAX              48   /* keep it reasonable for OLED display */

/* -----------------------------
   Globals
   ----------------------------- */
static XUartLite Uart;
static PmodOLED  Oled;

/* -----------------------------
   UART helpers (blocking/polled)
   ----------------------------- */
static void uart_putc(char c)
{
    XUartLite_Send(&Uart, (u8*)&c, 1);
    while (XUartLite_IsSending(&Uart)) { }
}

static void uart_puts(const char *s)
{
    while (*s) uart_putc(*s++);
}

static char uart_getc_blocking(void)
{
    /* Use the low-level recv byte for simplicity */
    return (char)XUartLite_RecvByte(UARTLITE_BASEADDR);
}

/* -----------------------------
   Small utilities
   ----------------------------- */
static int is_digit(char c) { return (c >= '0' && c <= '9'); }

static char to_upper(char c)
{
    if (c >= 'a' && c <= 'z') return (char)(c - 'a' + 'A');
    return c;
}

/* Caesar shift (A-Z only). Spaces allowed. */
static char shift_char(char c, int shift, int dir_forward)
{
    if (c == ' ') return ' ';
    if (c < 'A' || c > 'Z') return 0; /* invalid */

    int x = c - 'A';
    int s = shift % 26;
    if (s < 0) s += 26;

    if (dir_forward) x = (x + s) % 26;
    else            x = (x - s + 26) % 26;

    return (char)('A' + x);
}

/* -----------------------------
   OLED helpers
   ----------------------------- */
static void oled_show_text(const char *line)
{
    /* Clear screen by re-initializing the display buffer area */
    OLED_ClearBuffer(&Oled);

    /* Put text at top-left; this driver prints in pages */
    OLED_SetCursor(&Oled, 0, 0);
    OLED_PutString(&Oled, (char*)line);

    OLED_Update(&Oled);
}

/* -----------------------------
   Parse command format:
   F<number><message>
   B<number><message>
   Examples:
     F3HELLO WORLD
     B12ATTACK AT DAWN
   ----------------------------- */
static int parse_and_encrypt(const char *in, char *out)
{
    /* Expect at least 3 chars: F/B + digit + message... */
    char mode = to_upper(in[0]);
    if (mode != 'F' && mode != 'B') return -1;

    int i = 1;
    if (!is_digit(in[i])) return -1;

    int shift = 0;
    while (is_digit(in[i])) {
        shift = shift * 10 + (in[i] - '0');
        i++;
        if (shift > 25) { /* keep it simple */
            shift %= 26;
        }
    }

    /* Remaining is message (allow leading spaces, but we’ll keep as-is) */
    int out_i = 0;
    for (; in[i] != '\0' && out_i < MSG_MAX; i++) {
        char c = to_upper(in[i]);

        if (c == '\r' || c == '\n') break;

        /* Allow spaces and A-Z only */
        if (!(c == ' ' || (c >= 'A' && c <= 'Z'))) {
            return -2; /* invalid character */
        }

        char enc = shift_char(c, shift, (mode == 'F'));
        if (enc == 0) return -2;

        out[out_i++] = enc;
    }

    out[out_i] = '\0';
    return 0;
}

/* -----------------------------
   Main
   ----------------------------- */
int main(void)
{
    int status;

    /* Init UARTLite (COM4 via UART USB-Pmod) */
    status = XUartLite_Initialize(&Uart, UARTLITE_DEVICE_ID);
    if (status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    /* Init OLED */
    OLED_Begin(&Oled, OLED_GPIO_BASEADDR, OLED_SPI_BASEADDR, OLED_ORIENT, OLED_INVERT);
    OLED_Clear(&Oled);
    OLED_Update(&Oled);

    uart_puts("\r\nUART(Pmod) + OLED Caesar Cipher Ready\r\n");
    uart_puts("Format: F3HELLO WORLD  or  B3HELLO WORLD\r\n");
    uart_puts("Enter to encrypt -> OLED shows encrypted only.\r\n\r\n");

    oled_show_text("READY");

    while (1) {
        char line[LINE_MAX];
        char enc[MSG_MAX + 1];

        int n = 0;
        uart_puts("> ");

        /* Read a line with echo + simple backspace support */
        while (1) {
            char c = uart_getc_blocking();

            /* Handle Enter (CR or LF) */
            if (c == '\r' || c == '\n') {
                uart_puts("\r\n");
                break;
            }

            /* Backspace (BS or DEL) */
            if (c == 0x08 || c == 0x7F) {
                if (n > 0) {
                    n--;
                    uart_puts("\b \b");
                }
                continue;
            }

            /* Enforce buffer limit */
            if (n >= (LINE_MAX - 1)) {
                /* ignore extra chars */
                continue;
            }

            /* Echo typed char */
            uart_putc(c);

            /* Store */
            line[n++] = c;
        }

        line[n] = '\0';

        /* Empty line -> just reprompt */
        if (n == 0) {
            continue;
        }

        int rc = parse_and_encrypt(line, enc);
        if (rc == 0) {
            oled_show_text(enc);
            uart_puts("OK (encrypted sent to OLED)\r\n");
        } else if (rc == -1) {
            uart_puts("ERR: Format must start with F or B, then a number, then message.\r\n");
            uart_puts("Example: F3HELLO WORLD\r\n");
        } else {
            uart_puts("ERR: Only letters A-Z and spaces allowed in message.\r\n");
        }
    }

    /* not reached */
    // OLED_End(&Oled);
    // return 0;
}
