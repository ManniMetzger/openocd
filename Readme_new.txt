This is an attempt to backport the aji driver from aji_openocd to current openocd.
Seems to be a success so far. The porting has been done by chatgpt
so blame that if you have problems.

AJI client adapter integration and build wiring.
New AJI driver sources under src/jtag/drivers/aji_client/.
Virtual JTAG/TAP support hooks in the JTAG core.
Board configs for Arria 10 AJI client.
RISC-V memory access polling fix.
FreeRTOS RISC-V thread register support.
GDB keepalive behavior change for LLDB compatibility.
Imported src/libaji_client.dll and src/libusb-1.0.dll.


How to build:

in Mingw64 console
    git submodule init jimtcl
    git submodule update jimtcl
    ./bootstrap
    ./configure --enable-aji_client --build=x86_64-w64-mingw32 --host=x86_64-w64-mingw32 --enable-internal-jimtcl
    ./make
    strip ./src/openocd.exe

