This is an attemt to backport the aji driver from aji_openocd to current openocd. Seems to be a success so far.

AJI client adapter integration and build wiring.
New AJI driver sources under src/jtag/drivers/aji_client/.
Virtual JTAG/TAP support hooks in the JTAG core.
Board configs for Arria 10 AJI client.
RISC-V memory access polling fix.
FreeRTOS RISC-V thread register support.
GDB keepalive behavior change for LLDB compatibility.
Imported src/libaji_client.dll and src/libusb-1.0.dll.