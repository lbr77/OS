KERNEL_DIR=kernel
USER_DIR=user
TARGET_DIR=target
KERNEL_FILE=$(shell find $(KERNEL_DIR) -name "*.c") $(shell find $(KERNEL_DIR) -name "*.S")
USER_ELF=$(wildcard $(USER_DIR)/bin/*.c)
USER_FILE=$(wildcard $(USER_DIR)/*.c) $(wildcard $(USER_DIR)/*.S)
KERNEL_LINKER=$(KERNEL_DIR)/linker.ld
USER_LINKER=$(USER_DIR)/linker.ld
FLAGS=-ffreestanding -nostdlib -O2 -g -mcmodel=medany -march=rv64gc_zifencei
USER_BASE_NAME=$(basename $(notdir $(USER_ELF)))
USER_BIN=$(addprefix $(TARGET_DIR)/,$(addsuffix .bin,$(USER_BASE_NAME)))
USER_ELF=$(addprefix $(TARGET_DIR)/,$(addsuffix .elf,$(USER_BASE_NAME)))


default: ubin os.bin run

clean:
	rm -rf $(TARGET_DIR)

mkdir:
	mkdir -p $(TARGET_DIR)
	
os.bin: mkdir
	riscv64-unknown-elf-gcc $(KERNEL_FILE) $(FLAGS) -T $(KERNEL_LINKER) -o $(TARGET_DIR)/os
	riscv64-unknown-elf-objcopy $(TARGET_DIR)/os --strip-all -O binary $(TARGET_DIR)/os.bin

kill:
	killall qemu-system-riscv64

run: ubin os.bin
	qemu-system-riscv64 -machine virt -nographic -cpu rv64 -bios bootloader/rustsbi-qemu.bin -device loader,file=$(TARGET_DIR)/os.bin,addr=0x80200000 

debug: ubin os.bin
	qemu-system-riscv64 -machine virt -nographic -cpu rv64 -bios bootloader/rustsbi-qemu.bin -device loader,file=$(TARGET_DIR)/os.bin,addr=0x80200000 -s -S


ubin:
	python3 user/compile_user.py