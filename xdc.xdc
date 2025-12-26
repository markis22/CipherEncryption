## --- PMOD OLED on JC ---
## Pmod_out_0 pins map to PMOD pins 1,2,3,4,7,8,9,10

set_property -dict { PACKAGE_PIN V15 IOSTANDARD LVCMOS33 } [get_ports { Pmod_out_0_pin1_io  }]; # JC pin 1  (jc[0])
set_property -dict { PACKAGE_PIN W15 IOSTANDARD LVCMOS33 } [get_ports { Pmod_out_0_pin2_io  }]; # JC pin 2  (jc[1])
set_property -dict { PACKAGE_PIN T11 IOSTANDARD LVCMOS33 } [get_ports { Pmod_out_0_pin3_io  }]; # JC pin 3  (jc[2])
set_property -dict { PACKAGE_PIN T10 IOSTANDARD LVCMOS33 } [get_ports { Pmod_out_0_pin4_io  }]; # JC pin 4  (jc[3])

set_property -dict { PACKAGE_PIN W14 IOSTANDARD LVCMOS33 } [get_ports { Pmod_out_0_pin7_io  }]; # JC pin 7  (jc[4])
set_property -dict { PACKAGE_PIN Y14 IOSTANDARD LVCMOS33 } [get_ports { Pmod_out_0_pin8_io  }]; # JC pin 8  (jc[5])
set_property -dict { PACKAGE_PIN T12 IOSTANDARD LVCMOS33 } [get_ports { Pmod_out_0_pin9_io  }]; # JC pin 9  (jc[6])
set_property -dict { PACKAGE_PIN U12 IOSTANDARD LVCMOS33 } [get_ports { Pmod_out_0_pin10_io }]; # JC pin 10 (jc[7])

## --- UART USB PMOD on JA (top row JA1-JA6) ---
## PMOD pin2 = RXD, pin3 = TXD

# FPGA TXD -> PMOD RXD (pin2 -> JA2)
set_property -dict { PACKAGE_PIN L14 IOSTANDARD LVCMOS33 } [get_ports { UART_0_txd }];

# FPGA RXD <- PMOD TXD (pin3 -> JA3)
set_property -dict { PACKAGE_PIN K16 IOSTANDARD LVCMOS33 } [get_ports { UART_0_rxd }];

