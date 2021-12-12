#not using template
#include ../../../basic.mk

OUTPUT_ARM64 := example.arm64
OUTPUT_x86_64 := example.x86_64
OBJ_LIST =
ARM64_LDFLAGS += -L../../sysroot/usr/lib

CXXFLAGS += " -fsanitize=address -I/home/geumran.lee/build-ccic/BUILD/sysroots-components/aarch64/gcc-sanitizers/usr/lib/gcc/aarch64-gnu-linux/9.3.0/include/"
LD += "-L/home/geumran.lee/build-ccic/BUILD/work/aarch64-gnu-linux/gcc-sanitizers/9.3.0-r0/image/usr/lib"

#/home/seokhoon.yoon/work/2021/build-ccic/BUILD/sysroots-components/aarch64/gcc-sanitizers/usr/lib

all: x86 aarch64

x86:
	gcc main.cpp $(CFLAGS) $(LDFLAGS) -o $(OUTPUT_x86_64)

aarch64:
	aarch64-gnu-linux-gcc $(CXXFLAGS) -lasan $(LD) $(ARM64_LDFLAGS) main.cpp -o $(OUTPUT_ARM64)

clean:
	rm -rf $(OUTPUT_ARM64) $(OUTPUT_x86_64)
