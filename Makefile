CC = x86_64-elf-gcc

build:
  cd gnu-tools/
  $(CC) * -o *.o  
