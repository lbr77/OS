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


default: hello_world.bin os.bin run

clean:
	rm -rf $(TARGET_DIR)

mkdir:
	mkdir -p $(TARGET_DIR)
	
$(TARGET_DIR)/%.bin: $(USER_DIR)/bin/%.c mkdir
	riscv64-unknown-elf-gcc $< $(FLAGS) -T $(USER_LINKER) -o $(TARGET_DIR)/$(basename $(notdir $<))
	riscv64-unknown-elf-objcopy $(TARGET_DIR)/$(basename $(notdir $<)) --strip-all -O binary $(TARGET_DIR)/$(basename $(notdir $<)).bin

os.bin: mkdir
	riscv64-unknown-elf-gcc $(KERNEL_FILE) $(FLAGS) -T $(KERNEL_LINKER) -o $(TARGET_DIR)/os
	riscv64-unknown-elf-objcopy $(TARGET_DIR)/os --strip-all -O binary $(TARGET_DIR)/os.bin
	
hello_world.bin: mkdir
	riscv64-unknown-elf-gcc $(USER_FILE) $(FLAGS) -T $(USER_LINKER) -o $(TARGET_DIR)/hello_world
	riscv64-unknown-elf-objcopy $(TARGET_DIR)/hello_world --strip-all -O binary $(TARGET_DIR)/hello_world.bin
kill:
	killall qemu-system-riscv64

run: hello_world.bin os.bin
	qemu-system-riscv64 -machine virt -nographic -cpu rv64 -bios bootloader/rustsbi-qemu.bin -device loader,file=$(TARGET_DIR)/os.bin,addr=0x80200000 

debug: hello_world.bin os.bin
	qemu-system-riscv64 -machine virt -nographic -cpu rv64 -bios bootloader/rustsbi-qemu.bin -device loader,file=$(TARGET_DIR)/os.bin,addr=0x80200000 -s -S