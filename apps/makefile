all: program

program: program.o
	ld -melf_i386 program.o --oformat binary -o program.bin
program.o:
	as --32 program.s -o program.o

clean:
	rm *.o program.bin