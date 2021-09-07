#not using template
#include ../../../basic.mk

OUTPUT_ARM64 := example.arm64
OUTPUT_x86_64 := example.x86_64
OBJ_LIST =
LDFLAGS += -llsan
ARM64_LDFLAGS += -L/home/seokhoon.yoon/work/2021/build-ccic/BUILD/sysroots-components/aarch64/gcc-sanitizers/usr/lib

all:
	aarch64-gnu-linux-gcc main.cpp $(CFLAGS) $(LDFLAGS) $(ARM64_LDFLAGS) -o $(OUTPUT_ARM64)
	gcc main.cpp $(CFLAGS) $(LDFLAGS) -o $(OUTPUT_x86_64)
clean:
	rm -rf $(OUTPUT_ARM64) $(OUTPUT_x86_64)
