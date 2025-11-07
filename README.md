# RISCV-HYP-Tests for NHV5

## About 

Testing is done by executing bare metal applications, which *set up the environment at different privilege levels*, *execute the testing code or trigger traps* and *check the execution results and processor status* to determine if specific features of the target platform is compliant with the hypervisor extension specification.

## Building
We assume that the RISC-V toolchain is installed and is added to the PATH.

### Target platform

The target platform on which the test will run should be specified by setting the PLAT environment variables. And the platforms that are currently supported are shown in the table below:

| Platform | ${PLAT} Parameters |
| - | - |
| *NEMU* | `nemu` |
| *Spike* | `spike` |
| *QEMU* | `qemu` |
| *Rocket Chip FPGA* | `rocket_fpga` |

### Output level

The output level can be specified via the `LOG_LEVEL` environment variable (default is `LOG_INFO`). 

Options include:
`LOG_NONE`, `LOG_ERROR`, `LOG_INFO`, `LOG_DETAIL`, `LOG_WARNING`, `LOG_VERBOSE`, `LOG_DEBUG`

### Compile

The build process is as follows:

```bash
$ git clone https://github.com/wuyl10/riscv-hyp-tests-nhv5
$ cd riscv-hyp-tests-nhv5
$ make PLAT=? LOG_LEVEL=? CROSS_COMPILE=riscv64-unknown-elf- 
```

The compilation result is located at *build/*`${PLAT}`*/rvh_test.elf* or *rvh_test.bin*.


## Project Structure

```
riscv-hyp-tests-nhv5/
├── inc/                          # Header files
│   ├── page_tables.h            # Static page table definitions
│   ├── dynamic_page_tables.h    # Dynamic page table API
│   ├── instructions.h           # Low-level instruction wrappers
│   ├── csrs.h                   # CSR definitions
│   └── rvh_test.h              # Test framework definitions
├── platform/                     # Platform-specific implementations
├── *.c                          # Test case implementations
├── dynamic_page_tables.c        # Dynamic PT implementation (Sv39/Sv48)
├── page_tables.c                # Static page table setup
├── boot.S                       # Boot assembly code
├── handlers.S                   # Exception/interrupt handlers
├── Makefile                     # Build system
└── linker.ld                    # Linker script (memory layout)
```

## Memory Layout

The linker script (`linker.ld`) defines the following memory layout:

```
Address         Section           Description
──────────────────────────────────────────────────────
0x80000000      .boot            Boot code
                .text            Program code
                .rodata          Read-only data
                .test_table      Test function table
                .data            Initialized data
                .tohost          HTIF communication (page-aligned)
0x80052000      .bss             Uninitialized data
0x801ff000      Stack start      Stack grows downward
0x802ff000      Stack top        1MB stack space
```


