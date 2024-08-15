CC = x86_64-elf-gcc

build:
	cd gnu-tools/objectives/ && $(CC) *.c 
