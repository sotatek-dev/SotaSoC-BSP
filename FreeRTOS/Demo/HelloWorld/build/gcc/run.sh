make clean
make -j
clear
/home/hienha/Projects/xpack-riscv-none-elf-gcc-13.2.0-2/bin/riscv-none-elf-objcopy -O ihex output/RTOSDemo.elf output/RTOSDemo.hex
qemu-system-riscv32 -nographic -machine virt -net none     -chardev stdio,id=con,mux=on -serial chardev:con     -mon chardev=con,mode=readline -bios none     -smp 4 -kernel ./output/RTOSDemo.elf
