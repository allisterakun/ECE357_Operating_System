 To build helloworld.c:
    gcc -o helloworld.out helloworld.c

 Assemble/link build process of helloworld.s(on a x86-64 system):
    as helloworld.s -o helloworld.o --64
    ld helloworld.o -o helloworld -m elf_x86_64

Do 
    strace helloworld/helloworld.out
                                     to exam its opration.