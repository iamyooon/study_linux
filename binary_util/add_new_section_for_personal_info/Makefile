GCC ?= $(PREFIX)gcc
OBJCOPY ?= $(PREFIX)objcopy
OBJDUMP ?= $(PREFIX)objdump
all: compile write read

compile: hello.c
	$(GCC) -c hello.c -o hello.o
write:
	$(OBJCOPY) --add-section .version=version_info --set-section-flags .version=noload,readonly hello.o hello_w_ver.o
	$(GCC) hello_w_ver.o -o hello
read:
	$(OBJDUMP) -sj .version hello
clean:
	@rm *.o hello
