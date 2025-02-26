KERNEL_DIR=kernel
TARGET_DIR=target
FILE=$(KERNEL_DIR)/kernel.c $(KERNEL_DIR)/lib.c $(KERNEL_DIR)/entry.S $(KERNEL_DIR)/sbi.c
LINKER_LD=$(KERNEL_DIR)/linker.ld
FLAGS=-T $(LINKER_LD) -ffreestanding -nostdlib -g -o $(TARGET_DIR)/os -mcmodel=medany
default: 
	riscv64-unknown-elf-gcc $(FILE) $(FLAGS)
	riscv64-unknown-elf-objcopy $(TARGET_DIR)/os --strip-all -O binary $(TARGET_DIR)/os.bin
	qemu-system-riscv64 -machine virt -nographic -bios bootloader/rustsbi-qemu.bin -device loader,file=$(TARGET_DIR)/os.bin,addr=0x80200000