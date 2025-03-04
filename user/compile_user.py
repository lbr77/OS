import os
import re

BASE_ADDRESS = 0x80400000
APP_SIZE = 0x20000
app_id = 0
linker = "user/linker.ld"
linker_asm = "kernel/asm/link_app.S"
libs = os.listdir("user")
libs = [lib for lib in libs if os.path.isfile(f"user/{lib}") and lib.endswith(".c")]
libs.sort()
libs = " ".join([f"user/{lib}" for lib in libs])
executables = os.listdir("user/executable")
executables.sort()
section_0  = f"""
    .section .data
    .global _num_app
    .align 3
_num_app:
    .quad {len(executables)}
"""
sections = []
for executable in executables:
    name = executable.split(".")[0]
    linker_file = open(linker).read()
    # print()
    open(linker, "w").write(re.sub(r"0x[0-9a-fA-F]+", hex(BASE_ADDRESS + APP_SIZE * app_id), linker_file))
    os.system(f"riscv64-unknown-elf-gcc -T {linker} -o target/{name} user/executable/{executable} {libs} -ffreestanding -nostdlib -O2 -g -mcmodel=medany -march=rv64gc_zifencei")
    os.system(f"riscv64-unknown-elf-objcopy -O binary target/{name} target/{name}.bin")
    sections.append(f"""    .section .data
    .global app_{app_id}_start
    .global app_{app_id}_end
app_{app_id}_start:
    .incbin "target/{name}.bin"
app_{app_id}_end:
""")
    section_0 += f"    .quad app_{app_id}_start\n"
    app_id += 1
section_0 += f"    .quad app_{app_id - 1}_end\n\n"

open(linker_asm, "w").write(section_0 + "\n".join(sections))