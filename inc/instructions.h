#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <util.h>

#define INS_OPCODE  BIT_MASK(0, 7)
#define INS_RD      BIT_MASK(7, 5)
#define INS_FUNCT3  BIT_MASK(12, 3)
#define INS_RS1     BIT_MASK(15, 5)
#define INS_RS2     BIT_MASK(20, 5)
#define INS_FUNCT7  BIT_MASK(25, 7)

#define INS_C_OPCODE        BIT_MASK(0, 2)  
#define INS_C_FUNCT3_OFF    (13)
#define INS_C_FUNCT3_LEN    (3)
#define INS_C_FUNCT3_MASK   BIT_MASK(INS_C_FUNCT3_OFF, INS_C_FUNCT3_LEN)
#define INS_C_FUNCT3        INS_C_FUNCT3_MASK
#define INS_C_RS1_OFF       (7)
#define INS_C_RS1_LEN       (3)
#define INS_C_RS1_MASK      BIT_MASK(INS_C_RS1_OFF, INS_C_RS1_LEN)
#define INS_C_RDRS2_OFF     (2)
#define INS_C_RDRS2_LEN     (3)
#define INS_C_RDRS2_MASK    BIT_MASK(INS_C_RDRS2_OFF, INS_C_RDRS2_LEN)
#define INS_C_IMM0_OFF      (5)
#define INS_C_IMM0_LEN      (2)
#define INS_C_IMM0_MASK     BIT_MASK(INS_C_IMM0_OFF, INS_C_IMM0_LEN)
#define INS_C_IMM1_OFF      (10)
#define INS_C_IMM1_LEN      (3)
#define INS_C_IMM1_MASK     BIT_MASK(INS_C_IMM1_OFF, INS_C_IMM1_LEN)

#define INS_COMPRESSED(ins) (((ins) & INS_C_OPCODE) != 0b11)

#ifndef __ASSEMBLER__

#define CSR_STR(s) _CSR_STR(s)
#define _CSR_STR(s) #s

#define CSRR(csr)                                     \
    ({                                                \
        uint64_t _temp;                               \
        asm volatile("csrr  %0, " CSR_STR(csr) "\n\r" \
                     : "=r"(_temp)::"memory");        \
        _temp;                                        \
    })

#define CSRW(csr, rs) \
    asm volatile("csrw  " CSR_STR(csr) ", %0\n\r" ::"rK"(rs) : "memory");

#define CSRS(csr, rs) \
    asm volatile("csrs  " CSR_STR(csr) ", %0\n\r" ::"rK"(rs) : "memory");

#define CSRC(csr, rs) \
    asm volatile("csrc  " CSR_STR(csr) ", %0\n\r" ::"rK"(rs) : "memory");

#define RDCYCLE()                                     \
    ({                                                \
        uint64_t _temp;                               \
        asm volatile("rdcycle  %0\n\r"                \
                     : "=r"(_temp)::"memory");        \
        _temp;                                        \
    })

#define RDTIME()                                     \
    ({                                                \
        uint64_t _temp;                               \
        asm volatile("rdtime  %0\n\r"                \
                     : "=r"(_temp)::"memory");        \
        _temp;                                        \
    })

#define RDINSTRET()                                     \
    ({                                                \
        uint64_t _temp;                               \
        asm volatile("rdinstret  %0\n\r"                \
                     : "=r"(_temp)::"memory");        \
        _temp;                                        \
    })


#define GET_PC()                                      \
    ({                                                \
        uint64_t _temp;                               \
        asm volatile(                                 \
            "auipc %0, 0\n\t"                         \
            "addi  %0, %0, 0\n\t"                     \
            : "=r"(_temp)                             \
            :: "memory");                             \
        _temp;                                        \
    })

// static inline void jalr_direct(uintptr_t addr) {
//     asm volatile(
//         "mv x1, %0\n\t" // 将虚拟地址存储到寄存器 x1
//         ".insn i 0x67, 0, x0, x1, 0\n\t"  // 使用 x1 中的地址生成 jalr 指令
//         : // 无输出操作数
//         : "r"(addr)  // 输入操作数：虚拟地址
//         : "x1", "memory"  // 通知编译器该内联汇编可能修改 x1 寄存器和内存
//     );
// }



static inline void sfence(){
    asm volatile ("sfence.vma \n\t");
}

static inline void fence_iorw_iorw(){
    asm volatile ("fence iorw,iorw \n\t");
}


static inline void mnret()
{
    asm volatile(
        // .insn r <opcode>, <funct3>, <funct7>, <rd>, <rs1>, <rs2>
        // mnret 对应的关键字段是：
        //   opcode = 0x73
        //   funct7 = 0x38 (binary 0111000)
        //   rs2    = 2    (binary 00010)
        //   rs1    = 0
        //   funct3 = 0
        //   rd     = 0
        ".insn r 0x73, 0, 0x38, x0, x0, x2\n\t"
        ::: "memory");
}

static inline void sfence_vma(){
    asm volatile(
        ".insn r 0x73, 0x0, 0x09, x0, x0, x0\n\t"
        ::: "memory");
}

// sfence.vma with configurable rs1 (virtual address) and rs2 (ASID)
// Use (uintptr_t)-1 for vaddr or (uint64_t)-1 for asid to specify x0 register
// This allows flushing actual address 0 or ASID 0
// Examples:
//   sfence_vma_rs(-1, -1)     -> sfence.vma x0, x0  (flush all)
//   sfence_vma_rs(0x1000, -1) -> sfence.vma a0, x0  (flush addr 0x1000, all ASIDs)
//   sfence_vma_rs(-1, 5)      -> sfence.vma x0, a1  (flush all addrs, ASID 5)
//   sfence_vma_rs(0, 0)       -> sfence.vma a0, a1  (flush addr 0, ASID 0)
static inline void sfence_vma_rs(uintptr_t vaddr, uint64_t asid){
    if (vaddr == (uintptr_t)-1 && asid == (uint64_t)-1) {
        // Use x0, x0 to flush all TLB entries
        asm volatile("sfence.vma x0, x0\n\t" ::: "memory");
    } else if (vaddr == (uintptr_t)-1) {
        // Flush all addresses for specific ASID
        register uint64_t a1 asm("a1") = asid;
        asm volatile("sfence.vma x0, %0\n\t" :: "r"(a1) : "memory");
    } else if (asid == (uint64_t)-1) {
        // Flush specific address for all ASIDs
        register uintptr_t a0 asm("a0") = vaddr;
        asm volatile("sfence.vma %0, x0\n\t" :: "r"(a0) : "memory");
    } else {
        // Flush specific address for specific ASID
        register uintptr_t a0 asm("a0") = vaddr;
        register uint64_t a1 asm("a1") = asid;
        asm volatile("sfence.vma %0, %1\n\t" :: "r"(a0), "r"(a1) : "memory");
    }
}


static inline void Sinval_vma(){
    asm volatile(
        ".insn r 0x73, 0x0, 0x0B, x0, x0, x0\n\t"
        ::: "memory");
}

static inline void hinval_gvma(){
    asm volatile(
        ".insn r 0x73, 0x0, 0x33, x0, x0, x0\n\t"
        ::: "memory");
}


static inline void hinval_vvma(){
    asm volatile(
        ".insn r 0x73, 0x0, 0x13, x0, x0, x0\n\t"
        ::: "memory");
}

static inline void jal(uintptr_t vaddr) {
    // 使用内联汇编将传入的vaddr保存到寄存器x10，然后跳转到该地址
    asm volatile(
        "mv x10, %0\n\t"  // 将vaddr的值移动到x10寄存器
        "jalr x0, x10, 0\n\t"  // 从x10寄存器的地址跳转，偏移量为0
        :  // 输出列表（这里没有输出）
        : "r" (vaddr)  // 输入列表，将vaddr作为输入操作数
        : "x10", "memory");  // 破坏列表，x10寄存器和内存可能会被这条汇编指令修改
}


/*
0x73：操作码opcode
0x31： funct7字段，是一个7位的函数码，用于指定具体的RISC-V指令。
0x0： funct3字段，是一个3位的函数码。
x0, x0, x0：分别是rs1、rs2和rd寄存器参数，这里都是x0，表示使用寄存器x0（在RISC-V中通常用作临时寄存器）。
\n\t是换行符和制表符，用于格式化汇编代码。

        ::: "memory");
这是内联汇编的输出操作。:::表示没有输入操作数。
"memory"是一个clobber列表，告诉编译器这个汇编代码可能会修改所有的内存位置，因此编译器需要确保在这条汇编指令前后的内存操作不会和它重叠。
*/
static inline void hfence_gvma() {
    asm volatile(
        ".insn r 0x73, 0x0, 0x31, x0, x0, x0\n\t"
        ::: "memory");
}

// hfence.gvma with configurable rs1 (guest physical address) and rs2 (VMID)
// Use (uintptr_t)-1 for gaddr or (uint64_t)-1 for vmid to specify x0 register
static inline void hfence_gvma_rs(uintptr_t gaddr, uint64_t vmid){
    if (gaddr == (uintptr_t)-1 && vmid == (uint64_t)-1) {
        asm volatile("hfence.gvma x0, x0\n\t" ::: "memory");
    } else if (gaddr == (uintptr_t)-1) {
        register uint64_t a1 asm("a1") = vmid;
        asm volatile("hfence.gvma x0, %0\n\t" :: "r"(a1) : "memory");
    } else if (vmid == (uint64_t)-1) {
        register uintptr_t a0 asm("a0") = gaddr;
        asm volatile("hfence.gvma %0, x0\n\t" :: "r"(a0) : "memory");
    } else {
        register uintptr_t a0 asm("a0") = gaddr;
        register uint64_t a1 asm("a1") = vmid;
        asm volatile("hfence.gvma %0, %1\n\t" :: "r"(a0), "r"(a1) : "memory");
    }
}

static inline void hfence_vvma() {
    asm volatile(
        ".insn r 0x73, 0x0, 0x11, x0, x0, x0\n\t"
        ::: "memory");
}

// hfence.vvma with configurable rs1 (virtual address) and rs2 (ASID)
// Use (uintptr_t)-1 for vaddr or (uint64_t)-1 for asid to specify x0 register
static inline void hfence_vvma_rs(uintptr_t vaddr, uint64_t asid){
    if (vaddr == (uintptr_t)-1 && asid == (uint64_t)-1) {
        asm volatile("hfence.vvma x0, x0\n\t" ::: "memory");
    } else if (vaddr == (uintptr_t)-1) {
        register uint64_t a1 asm("a1") = asid;
        asm volatile("hfence.vvma x0, %0\n\t" :: "r"(a1) : "memory");
    } else if (asid == (uint64_t)-1) {
        register uintptr_t a0 asm("a0") = vaddr;
        asm volatile("hfence.vvma %0, x0\n\t" :: "r"(a0) : "memory");
    } else {
        register uintptr_t a0 asm("a0") = vaddr;
        register uint64_t a1 asm("a1") = asid;
        asm volatile("hfence.vvma %0, %1\n\t" :: "r"(a0), "r"(a1) : "memory");
    }
}

static inline void INVALID_INSTR() {
    asm volatile(
        ".insn r 0x0F, 0x2, 0x01, x0, x0, x0\n\t"
        ::: "memory");
}

// static inline void invalid_instruction() {
//     asm volatile(
//         ".insn r 0xff, 0x0, 0x11, x0, x0, x0\n\t"
//         ::: "memory");
// }

static inline void sret() {
    asm volatile(
        "sret\n\t"
        ::: "memory");
}

static inline void mret() {
    asm volatile(
        "mret\n\t"
        ::: "memory");
}

static inline void dret() {
    asm volatile(
        "dret\n\t"
        ::: "memory");
}

static inline void ecall_code() {
    asm volatile(
        "ecall\n\t"
        ::: "memory");
}

static inline void cbo_inval(uintptr_t addr) {
    asm volatile(".insn i 0x0f, 0x2, x0, %0, 0"
                 :: "r"(addr) : "memory");
}

static inline void cbo_clean(uintptr_t addr) {
    asm volatile(".insn i 0x0f, 0x2, x0, %0, 1"
                 :: "r"(addr) : "memory");
}

static inline void cbo_flush(uintptr_t addr) {
    asm volatile(".insn i 0x0f, 0x2, x0, %0, 2"
                 :: "r"(addr) : "memory");
}

static inline void cbo_zero(uintptr_t addr) {
    asm volatile(".insn i 0x0f, 0x2, x0, %0, 4"
                 :: "r"(addr) : "memory");
}


static inline void prefetch_r(uintptr_t addr) {
    asm volatile(
        ".insn i 0x13, 0x6, x0, %0, 1\n\t"
        :: "r"(addr) : "memory");
}

// prefetch.w - Prefetch for write (offset=1 in imm[11:0])
static inline void prefetch_w(uintptr_t addr) {
    asm volatile(
        ".insn i 0x13, 0x6, x0, %0, 3\n\t"
        :: "r"(addr) : "memory");
}

// prefetch.i - Prefetch for instruction (offset=3 in imm[11:0])
static inline void prefetch_i(uintptr_t addr) {
    asm volatile(
        ".insn i 0x13, 0x6, x0, %0, 0\n\t"
        :: "r"(addr) : "memory");
}



static inline void ebreak() {
    asm volatile ("ebreak");
}

static inline void hfence() {
    hfence_vvma();
    hfence_gvma();
}

static inline uint64_t hlvb(uintptr_t addr){
    uint64_t value;
    asm volatile(
        ".insn r 0x73, 0x4, 0x30, %0, %1, x0\n\t"
        : "=r"(value): "r"(addr) : "memory");
    return value;
}

static inline uint64_t hlvbu(uintptr_t addr){
    uint64_t value;
    asm volatile(
        ".insn r 0x73, 0x4, 0x30, %0, %1, x1\n\t"
        : "=r"(value): "r"(addr) : "memory");
    return value;
}


static inline uint64_t hlvh(uintptr_t addr){
    uint64_t value;
    asm volatile(
        ".insn r 0x73, 0x4, 0x32, %0, %1, x0\n\t"
        : "=r"(value): "r"(addr) : "memory");
    return value;
}

static inline uint64_t hlvhu(uintptr_t addr){
    uint64_t value;
    asm volatile(
        ".insn r 0x73, 0x4, 0x32, %0, %1, x1\n\t"
        : "=r"(value): "r"(addr) : "memory");
    return value;
}

static inline uint64_t hlvxhu(uintptr_t addr){
    uint64_t value;
    asm volatile(
        ".insn r 0x73, 0x4, 0x32, %0, %1, x3\n\t"
        : "=r"(value): "r"(addr) : "memory");
    return value;
}

static inline uint64_t hlvw(uintptr_t addr){
    uint64_t value;
    asm volatile(
        ".insn r 0x73, 0x4, 0x34, %0, %1, x0\n\t"
        : "=r"(value): "r"(addr) : "memory");
    return value;
}

static inline uint64_t hlvwu(uintptr_t addr){
    uint64_t value;
    asm volatile(
        ".insn r 0x73, 0x4, 0x34, %0, %1, x1\n\t"
        : "=r"(value): "r"(addr) : "memory");
    return value;
}

static inline uint64_t hlvxwu(uintptr_t addr){
    uint64_t value;
    asm volatile(
        ".insn r 0x73, 0x4, 0x34, %0, %1, x3\n\t"
        : "=r"(value): "r"(addr) : "memory");
    return value;
}

static inline uint64_t hlvd(uintptr_t addr){
    uint64_t value;
    asm volatile(
        ".insn r 0x73, 0x4, 0x36, %0, %1, x0\n\t"
        : "=r"(value): "r"(addr) : "memory");
    return value;
}

static inline uint64_t hsvb(uintptr_t addr, uint64_t value){
    asm volatile(
        ".insn r 0x73, 0x4, 0x31, x0, %1, %0\n\t"
        :: "r"(value), "r"(addr) : "memory");
    return value;
}

static inline uint64_t hsvh(uintptr_t addr, uint64_t value){
    asm volatile(
        ".insn r 0x73, 0x4, 0x33, x0, %1, %0\n\t"
        :: "r"(value), "r"(addr): "memory");
    return value;
}

static inline uint64_t hsvw(uintptr_t addr, uint64_t value){
    asm volatile(
        ".insn r 0x73, 0x4, 0x35, x0, %1, %0\n\t"
        :: "r"(value), "r"(addr): "memory");
    return value;
}

static inline uint64_t hsvd(uintptr_t addr, uint64_t value){
    asm volatile(
        ".insn r 0x73, 0x4, 0x37, x0, %1, %0\n\t"
        :: "r"(value), "r"(addr) : "memory");
    return value;
}

static inline void wfi() {
    asm ("wfi" ::: "memory");
}

#define LOAD_INSTRUCTION(name, instruction, type) \
    static inline type name(uintptr_t addr){ \
        type value; \
        asm volatile( \
            ".option push\n\t" \
            ".option norvc\n\t" \
            instruction " %0, 0(%1)\n\t" \
            ".option pop\n\t" \
            : "=r"(value) : "r"(addr) : "memory"); \
        return value; \
    }

LOAD_INSTRUCTION(lb, "lb", uint8_t);
LOAD_INSTRUCTION(lbu, "lbu", uint8_t);
LOAD_INSTRUCTION(lh, "lh", uint16_t);
LOAD_INSTRUCTION(lhu, "lhu", uint16_t); 
LOAD_INSTRUCTION(lw, "lw", uint32_t); 
LOAD_INSTRUCTION(lwu, "lwu", uint32_t);
LOAD_INSTRUCTION(ld, "ld", uint64_t);




#define STORE_INSTRUCTION(name, instruction, type) \
    static inline void name(uintptr_t addr, type value){ \
        asm volatile( \
            ".option push\n\t" \
            ".option norvc\n\t" \
            instruction " %0, 0(%1)\n\t" \
            ".option pop\n\t" \
            :: "r"(value), "r"(addr):  "memory" \
        ); \
    }

STORE_INSTRUCTION(sb, "sb", uint8_t);
STORE_INSTRUCTION(sh, "sh", uint16_t);
STORE_INSTRUCTION(sw, "sw", uint32_t);
STORE_INSTRUCTION(sd, "sd", uint64_t);

/**
 * For compressed instructions there is no constraint to guarantee
 * the asm extended chosen registers are part of the subset allowed
 * so we have to make sure.
 */

static inline uint64_t c_lw(uintptr_t addr){
    register uint64_t _addr asm("a0") = addr;
    register uint64_t value asm("a1");
    asm volatile(
        "c.lw    %0, 0(%1)\n\t"
        :"=r"(value): "r"(_addr) : "memory"
    );
    return value;
}

static inline uint64_t c_ld(uintptr_t addr){
    register uint64_t _addr asm("a0") = addr;
    register uint64_t value asm("a1");
    asm volatile(
        "c.ld    %0, 0(%1)\n\t"
        :"=r"(value): "r"(_addr) : "memory"
    );
    return value;
}

static inline void c_sw(uintptr_t addr, uint64_t value){
    register uint64_t _addr asm("a0") = addr;
    register uint64_t _value asm("a1") = value;
    asm volatile(
        "c.sw    %0, 0(%1)\n\t"
        :: "r"(_value), "r"(_addr): "memory"
    );
}

static inline void c_sd(uintptr_t addr, uint64_t value){
    register uint64_t _addr asm("a0") = addr;
    register uint64_t _value asm("a1") = value;
    asm volatile(
        "c.sd    %0, 0(%1)\n\t"
        :: "r"(_value), "r"(_addr): "memory"
    );
}

static inline uint32_t lr_w(uintptr_t addr){
    uint32_t value;
    asm volatile(
        "lr.w %0, 0(%1)\n\t"
        : "=r"(value) : "r"(addr): "memory"
    );
    return value;
}


static inline uint32_t lr_d(uintptr_t addr){
    uint32_t value;
    asm volatile(
        "lr.d %0, 0(%1)\n\t"
        : "=r"(value) : "r"(addr): "memory"
    );
    return value;
}




static inline uint32_t sc_w(uintptr_t addr, uint64_t value){
    asm volatile(
        "sc.w    %0, %0, 0(%1)\n\t"
        : "+r"(value) : "r"(addr): "memory"
    );
    return value;
}

static inline uint32_t sc_d(uintptr_t addr, uint64_t value){
    asm volatile(
        "sc.d    %0, %0, 0(%1)\n\t"
        : "+r"(value) : "r"(addr): "memory"
    );
    return value;
}


#define AMO_INSTRUCTION(name, instruction, type) \
    static inline type name(uintptr_t addr, type value){ \
        asm volatile( \
            instruction " %0, %0, 0(%1)\n\t" \
            : "+r"(value) : "r"(addr): "memory" \
        ); \
        return value; \
    }

AMO_INSTRUCTION(amoswap_w, "amoswap.w", uint32_t);
AMO_INSTRUCTION(amoadd_w, "amoadd.w", uint32_t);
AMO_INSTRUCTION(amoxor_w, "amoxor.w", uint32_t);
AMO_INSTRUCTION(amoand_w, "amoand.w", uint32_t);
AMO_INSTRUCTION(amoor_w, "amoor.w", uint32_t);
AMO_INSTRUCTION(amomin_w, "amomin.w", uint32_t);
AMO_INSTRUCTION(amomax_w, "amomax.w", uint32_t);
AMO_INSTRUCTION(amominu_w, "amominu.w", uint32_t);
AMO_INSTRUCTION(amomaxu_w, "amomaxu.w", uint32_t);
AMO_INSTRUCTION(amoswap_d, "amoswap.d", uint64_t);
AMO_INSTRUCTION(amoadd_d, "amoadd.d", uint64_t);
AMO_INSTRUCTION(amoxor_d, "amoxor.d", uint64_t);
AMO_INSTRUCTION(amoand_d, "amoand.d", uint64_t);
AMO_INSTRUCTION(amoor_d, "amoor.d", uint64_t);
AMO_INSTRUCTION(amomin_d, "amomin.d", uint64_t);
AMO_INSTRUCTION(amomax_d, "amomax.d", uint64_t);
AMO_INSTRUCTION(amominu_d, "amominu.d", uint64_t);
AMO_INSTRUCTION(amomaxu_d, "amomaxu.d", uint64_t);


static inline void jmp(uintptr_t addr){
     asm volatile (
        "jalr x0, %0"
        : /* no output */
        : "r" (addr)
    );
}



static inline int* add_address(uintptr_t addr, int offset) {
    int* result;
    asm volatile (
        "add %0, %1, %2"
        : "=r" (result)
        : "r" (addr), "r" (offset)
    );
    return result;
}





void random_m_instruction();

#endif /* __ASSEMBLER__ */

#endif /* INSTRUCTIONS_H */
