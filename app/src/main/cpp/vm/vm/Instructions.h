#pragma once

#include <cstddef>

class CPU;

// ==================== 指令处理函数声明 ====================
void invalid(size_t pc, CPU& cpu);
void nop(size_t pc, CPU& cpu);
void hlt(size_t pc, CPU& cpu);

// 数据搬运 MOV
void mov_rr(size_t pc, CPU& cpu);
void mov_rv(size_t pc, CPU& cpu);
void mov_rm(size_t pc, CPU& cpu);
void mov_mr(size_t pc, CPU& cpu);

// 加载 LEA / LDS / LES
void lea_rv(size_t pc, CPU& cpu);
void lds_rv(size_t pc, CPU& cpu);
void les_rv(size_t pc, CPU& cpu);

// 交换 XCHG
void xchg_rr(size_t pc, CPU& cpu);
void xchg_rm(size_t pc, CPU& cpu);

// 算术 ADD
void add_rr(size_t pc, CPU& cpu);
void add_rv(size_t pc, CPU& cpu);
void add_rm(size_t pc, CPU& cpu);
void add_mr(size_t pc, CPU& cpu);

// ADC
void adc_rr(size_t pc, CPU& cpu);
void adc_rv(size_t pc, CPU& cpu);
void adc_rm(size_t pc, CPU& cpu);

// SUB
void sub_rr(size_t pc, CPU& cpu);
void sub_rv(size_t pc, CPU& cpu);
void sub_rm(size_t pc, CPU& cpu);
void sub_mr(size_t pc, CPU& cpu);

// SBB
void sbb_rr(size_t pc, CPU& cpu);
void sbb_rv(size_t pc, CPU& cpu);
void sbb_rm(size_t pc, CPU& cpu);

// INC / DEC
void inc_r(size_t pc, CPU& cpu);
void dec_r(size_t pc, CPU& cpu);

// MUL / IMUL
void mul_r(size_t pc, CPU& cpu);
void imul_r(size_t pc, CPU& cpu);
void imul_rr(size_t pc, CPU& cpu);
void imul_rv(size_t pc, CPU& cpu);

// DIV / IDIV
void div_r(size_t pc, CPU& cpu);
void idiv_r(size_t pc, CPU& cpu);

// 逻辑 AND
void and_rr(size_t pc, CPU& cpu);
void and_rv(size_t pc, CPU& cpu);
void and_rm(size_t pc, CPU& cpu);
void and_mr(size_t pc, CPU& cpu);

// OR
void or_rr(size_t pc, CPU& cpu);
void or_rv(size_t pc, CPU& cpu);
void or_rm(size_t pc, CPU& cpu);
void or_mr(size_t pc, CPU& cpu);

// XOR
void xor_rr(size_t pc, CPU& cpu);
void xor_rv(size_t pc, CPU& cpu);
void xor_rm(size_t pc, CPU& cpu);
void xor_mr(size_t pc, CPU& cpu);

// NOT / NEG
void not_r(size_t pc, CPU& cpu);
void neg_r(size_t pc, CPU& cpu);

// 移位 SHL / SHR / SAL / SAR
void shl_r(size_t pc, CPU& cpu);
void shr_r(size_t pc, CPU& cpu);
void sal_r(size_t pc, CPU& cpu);
void sar_r(size_t pc, CPU& cpu);

// 循环移位 ROL / ROR / RCL / RCR
void rol_r(size_t pc, CPU& cpu);
void ror_r(size_t pc, CPU& cpu);
void rcl_r(size_t pc, CPU& cpu);
void rcr_r(size_t pc, CPU& cpu);

// 比较 CMP
void cmp_rr(size_t pc, CPU& cpu);
void cmp_rv(size_t pc, CPU& cpu);
void cmp_rm(size_t pc, CPU& cpu);
void cmp_mr(size_t pc, CPU& cpu);

// TEST
void test_rr(size_t pc, CPU& cpu);
void test_rv(size_t pc, CPU& cpu);
void test_rm(size_t pc, CPU& cpu);

// 栈操作 PUSH / POP
void push_r(size_t pc, CPU& cpu);
void push_v(size_t pc, CPU& cpu);
void pop_r(size_t pc, CPU& cpu);
void pusha(size_t pc, CPU& cpu);
void popa(size_t pc, CPU& cpu);
void pushf(size_t pc, CPU& cpu);
void popf(size_t pc, CPU& cpu);

// 控制流 JMP / Jcc
void jmp_v(size_t pc, CPU& cpu);
void jmp_r(size_t pc, CPU& cpu);
void jmp_m(size_t pc, CPU& cpu);
void jz_v(size_t pc, CPU& cpu);
void jnz_v(size_t pc, CPU& cpu);
void jc_v(size_t pc, CPU& cpu);
void jnc_v(size_t pc, CPU& cpu);
void jo_v(size_t pc, CPU& cpu);
void jno_v(size_t pc, CPU& cpu);
void js_v(size_t pc, CPU& cpu);
void jns_v(size_t pc, CPU& cpu);
void jp_v(size_t pc, CPU& cpu);
void jnp_v(size_t pc, CPU& cpu);
void ja_v(size_t pc, CPU& cpu);
void jae_v(size_t pc, CPU& cpu);
void jb_v(size_t pc, CPU& cpu);
void jbe_v(size_t pc, CPU& cpu);
void jg_v(size_t pc, CPU& cpu);
void jge_v(size_t pc, CPU& cpu);
void jl_v(size_t pc, CPU& cpu);
void jle_v(size_t pc, CPU& cpu);

// CALL / RET / INT
void call_v(size_t pc, CPU& cpu);
void call_r(size_t pc, CPU& cpu);
void ret(size_t pc, CPU& cpu);
void ret_v(size_t pc, CPU& cpu);
void iret(size_t pc, CPU& cpu);
void int_v(size_t pc, CPU& cpu);
void into(size_t pc, CPU& cpu);

// 字符串操作
void movsb(size_t pc, CPU& cpu);
void movsw(size_t pc, CPU& cpu);
void stosb(size_t pc, CPU& cpu);
void stosw(size_t pc, CPU& cpu);
void lodsb(size_t pc, CPU& cpu);
void lodsw(size_t pc, CPU& cpu);
void cmpsb(size_t pc, CPU& cpu);
void cmpsw(size_t pc, CPU& cpu);
void scasb(size_t pc, CPU& cpu);
void scasw(size_t pc, CPU& cpu);

// I/O 操作
void in_al(size_t pc, CPU& cpu);
void in_ax(size_t pc, CPU& cpu);
void out_al(size_t pc, CPU& cpu);
void out_ax(size_t pc, CPU& cpu);

// 标志位操作
void clc(size_t pc, CPU& cpu);
void stc(size_t pc, CPU& cpu);
void cmc(size_t pc, CPU& cpu);
void cld(size_t pc, CPU& cpu);
void std_ins(size_t pc, CPU& cpu);  // 改名避免与std冲突
void cli(size_t pc, CPU& cpu);
void sti(size_t pc, CPU& cpu);

// BCD调整
void daa(size_t pc, CPU& cpu);
void das(size_t pc, CPU& cpu);
void aaa(size_t pc, CPU& cpu);
void aas(size_t pc, CPU& cpu);
void aam(size_t pc, CPU& cpu);
void aad(size_t pc, CPU& cpu);

// 扩展指令
void cbw(size_t pc, CPU& cpu);
void cwd(size_t pc, CPU& cpu);
void lahf(size_t pc, CPU& cpu);
void sahf(size_t pc, CPU& cpu);
void xlat(size_t pc, CPU& cpu);

// 系统指令
void wait_ins(size_t pc, CPU& cpu);  // 改名避免与系统wait冲突
void lock_ins(size_t pc, CPU& cpu);  // 改名避免与系统lock冲突

void call_builtin(size_t pc, CPU& cpu);

// ==================== 栈帧扩展指令（step-6） ====================
void mov_rbp(size_t pc, CPU& cpu);    // mov rX, [bp+off]  (0xC1)
void mov_bpr(size_t pc, CPU& cpu);    // mov [bp+off], rX (0xC2)
void pushbp(size_t pc, CPU& cpu);     // push bp          (0xC5)
void movbp_sp(size_t pc, CPU& cpu);   // mov bp, sp       (0xC6)
void movsp_bp(size_t pc, CPU& cpu);   // mov sp, bp       (0xC7)
void popbp(size_t pc, CPU& cpu);      // pop bp           (0xC8)
void addsp_imm(size_t pc, CPU& cpu);  // add sp, N        (0xC9)
void subsp_imm(size_t pc, CPU& cpu);  // sub sp, N        (0xCA)

// ==================== 指针扩展指令（step-8） ====================
void ind_load(size_t pc, CPU& cpu);   // mov rX, [rY]     (0xC3)
void ind_store(size_t pc, CPU& cpu);  // mov [rY], rX     (0xC4)
void bp_to_r(size_t pc, CPU& cpu);    // mov rX, bp       (0xCB)

// ==================== 跨模块跳转（step-10） ====================
void gotomodule_ins(size_t pc, CPU& cpu);  // gotomodule rX, argc  (0xCC)

// ==================== 参数结构体约定（step-16） ====================
void sp_to_r(size_t pc, CPU& cpu);    // mov rX, sp       (0xCD)
