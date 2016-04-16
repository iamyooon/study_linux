all: compile write read

compile: hello.c
	gcc -c hello.c -o hello.o 
write: 
	objcopy --add-section .version=version_info --set-section-flags .version=noload,readonly hello.o hello_w_ver.o
	gcc hello_w_ver.o -o hello
read: 
	objdump -sj .version hello

clean:
	rm *.o hello
