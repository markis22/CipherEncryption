<h1>Embedded Systems Cipher Encryption Project (FPGA-Based)</h1>

<h2>Description</h2>

This project implements a functional Embedded Encryption System (EES) that integrates hardware and software on a Zynq FPGA platform. The system receives live serial input via a USB UART interface, processes the data on the Zynq processing system, encrypts qualifying characters using a Caesar Cipher algorithm, and displays the encrypted output on an OLED display connected through a PMOD interface.

Rather than treating individual components in isolation, this project was designed as a fully integrated embedded system that combines UART communication, AXI-based IP connectivity, embedded C programming, and hardware-software co-design. The project also addresses real-world embedded constraints such as limited display resolution, low communication speeds, synchronization challenges, and restricted I/O availability on FPGA development boards.

Through this implementation, the project demonstrates practical applications of embedded communications, basic encryption techniques, and disciplined system integration using FPGA-based platforms.

<br />

## Skills You Will Learn

- **FPGA System Design** – Integrated hardware and software components on a Zynq FPGA platform.
- **UART Communication** – Implemented serial communication using a USB UART interface.
- **AXI Interconnect** – Connected IP blocks using AXI interfaces in Vivado.
- **Embedded C Programming** – Developed and debugged firmware in Vitis.
- **Hardware-Software Co-Design** – Coordinated data flow between programmable logic and processing system.
- **Embedded Debugging** – Resolved timing, baud rate, and peripheral configuration issues.

<h2>Languages and Utilities Used</h2>

- <b>Embedded C</b>  
- <b>Vivado Design Suite</b>  
- <b>Vitis IDE</b>  

<h2>Environments Used</h2>

- <b>Zynq FPGA Development Board</b>  
- <b>Windows 11</b>  

<h2>Program Walk-through:</h2>

<p align="center">
<br />

First, a Zynq-based FPGA project was created in Vivado to serve as the hardware foundation for the embedded encryption system. The design included the Zynq Processing System, AXI interconnects, UART interfaces, and PMOD connections for the OLED display.

<br />
<br />

Conceptual Block Diagram illustrating system-level data flow between UART input, processing system, cipher logic, and OLED output: <br />
<img src="https://i.imgur.com/gcFYObR.png" height="50%" width="50%" alt="Conceptual Block Diagram"/>

<br />
<br />

Next, the hardware design was synthesized and validated to ensure proper AXI connectivity and peripheral configuration. RTL schematics were reviewed to confirm correct signal routing and module integration.

<br />
<br />

RTL Schematic generated after synthesis: <br />
<img src="https://i.imgur.com/90DDoDE.png" height="50%" width="50%" alt="RTL Schematic"/>

<br />
<br />

After hardware validation, the design was exported to Vitis, where embedded C firmware was developed. The firmware configured the UART interface to receive live serial data from a USB UART connection.

<br />
<br />

Incoming data was parsed character-by-character. The system identified uppercase alphabetical characters and applied a Caesar Cipher transformation using a predefined shift value. Non-uppercase characters were ignored or passed through unmodified.

<br />
<br />

The encrypted output was transmitted to the OLED display via a PMOD interface. Due to limited display space, only the encrypted (scrambled) characters were rendered, emphasizing cipher output rather than raw input.

<br />
<br />

Synthesis Schematic confirming finalized hardware implementation: <br />
<img src="https://i.imgur.com/JJnkRlW.png" height="50%" width="50%" alt="Synthesis Schematic"/>

<br />
<br />

Throughout development, several technical challenges were encountered and resolved, including UART pin misconfiguration, baud rate synchronization issues, library incompatibilities in Vitis, and OLED driver initialization errors. These issues were addressed through incremental testing, debugging, and validation of both hardware and software components.

<br />
<br />

Final system output demonstrating encrypted text displayed on the OLED screen: <br />
[![Demo Video](thumbnail.png)](https://youtube.com/shorts/OVHvdoQR91g)

<br />
<br />

</p>

<h2>Project Summary</h2>

This project successfully delivered a working Caesar Cipher implementation on a Zynq FPGA platform that processes live UART input and displays encrypted output on an OLED display. The system demonstrates effective integration of embedded communications, basic encryption logic, and FPGA-based hardware-software co-design.

<h2>Conclusion</h2>

In conclusion, this project demonstrates how embedded systems concepts such as UART communication, AXI interconnects, and encryption algorithms can be applied in a real-world FPGA environment. Despite challenges related to synchronization, peripheral configuration, and limited hardware resources, the system was successfully implemented through disciplined design and incremental testing.

The project provides a strong foundation for future enhancements, including support for additional cipher algorithms, improved performance through greater hardware acceleration, expanded display capabilities, and more user-friendly interfaces. Overall, this implementation highlights the importance of structured system integration when developing reliable and secure embedded solutions.
