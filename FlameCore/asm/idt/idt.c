// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//           /======================\\`
//           | /==================\`||;
//           | |        ||        |;||;
//           | |   ,,   ||   ,,   |;||;
//           | |   ||   ||   ||   |;||;
//           | |   ||   ||   ||   |;||;
//           | |   ||   ``   ||   |;||;
//           | |   ||        ||   |;||;
//           | \––––––––––––––––––/´||;
//           \––––––––––––––––––––––//,
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       COPYRIGHT (c) 2025 MYTHOS PROJECT
//              ALL RIGHTS RESERVED
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <asm/idt/idt.h>
#include <asm/hal/io.h>
#include <std/string.h>
#include <gui/printk.h>

static IDTEntry idt[IDT_ENTRIES];
static IDTPtr idt_ptr;
static void (*interrupt_handlers[IDT_ENTRIES])(InterruptFrame*);

// 通用的中断处理函数（被汇编代码调用）
void interrupt_handler(InterruptFrame* frame) {
    if (frame->interrupt_number < IDT_ENTRIES && 
        interrupt_handlers[frame->interrupt_number]) {
        interrupt_handlers[frame->interrupt_number](frame);
    } else {
        printk("Unhandled interrupt: %d\n", (int)frame->interrupt_number);
    }
}

// 设置IDT条目
static void set_idt_entry(uint8_t vector, void (*handler)(), uint16_t selector, uint8_t type_attr) {
    uint64_t offset = (uint64_t)handler;
    
    idt[vector].offset_low = offset & 0xFFFF;
    idt[vector].selector = selector;
    idt[vector].ist = 0;
    idt[vector].type_attributes = type_attr;
    idt[vector].offset_mid = (offset >> 16) & 0xFFFF;
    idt[vector].offset_high = (offset >> 32) & 0xFFFFFFFF;
    idt[vector].zero = 0;
}

// 注册中断处理函数
void register_interrupt_handler(uint8_t vector, void (*handler)(InterruptFrame*)) {
    if (vector < IDT_ENTRIES) {
        interrupt_handlers[vector] = handler;
    }
}

// 异常处理函数实现
void divide_error_handler(InterruptFrame* frame) {
    printk("Divide error at RIP: 0x%x\n", (uint32_t)frame->rip);
    halt();
}

void double_fault_handler(InterruptFrame* frame) {
    printk("Double fault! System halted.\n");
    halt();
}

void page_fault_handler(InterruptFrame* frame) {
    uint64_t fault_address;
    __asm__ volatile("mov %%cr2, %0" : "=r" (fault_address));
    
    printk("Page fault at address: 0x%x\n", (uint32_t)fault_address);
    printk("Error code: 0x%x, RIP: 0x%x\n", (uint32_t)frame->error_code, (uint32_t)frame->rip);
}

// ==================== 内联汇编中断处理程序 ====================

// 除零错误处理程序 (中断0)
void divide_error_handler_asm(void) {
    __asm__ volatile(
        // 保存所有寄存器
        "push %%rax\n\t"
        "push %%rbx\n\t"
        "push %%rcx\n\t"
        "push %%rdx\n\t"
        "push %%rsi\n\t"
        "push %%rdi\n\t"
        "push %%rbp\n\t"
        "push %%r8\n\t"
        "push %%r9\n\t"
        "push %%r10\n\t"
        "push %%r11\n\t"
        "push %%r12\n\t"
        "push %%r13\n\t"
        "push %%r14\n\t"
        "push %%r15\n\t"
        
        // 设置中断号和错误码
        "mov $0, %%rax\n\t"          // 中断号=0
        "mov $0, %%rbx\n\t"          // 错误码=0（除零错误无错误码）
        "jmp common_interrupt_handler\n\t"
        
        // 内联汇编标签
        ".global divide_error_handler_asm_end\n\t"
        "divide_error_handler_asm_end:\n\t"
        "nop\n\t"
        : : : "memory"
    );
}

// 双重错误处理程序 (中断8)
void double_fault_handler_asm(void) {
    __asm__ volatile(
        // 保存所有寄存器（除了错误码已经在栈上）
        "push %%rax\n\t"
        "push %%rbx\n\t"
        "push %%rcx\n\t"
        "push %%rdx\n\t"
        "push %%rsi\n\t"
        "push %%rdi\n\t"
        "push %%rbp\n\t"
        "push %%r8\n\t"
        "push %%r9\n\t"
        "push %%r10\n\t"
        "push %%r11\n\t"
        "push %%r12\n\t"
        "push %%r13\n\t"
        "push %%r14\n\t"
        "push %%r15\n\t"
        
        // 设置中断号，错误码已经在栈上
        "mov $8, %%rax\n\t"          // 中断号=8
        "jmp common_interrupt_handler\n\t"
        
        ".global double_fault_handler_asm_end\n\t"
        "double_fault_handler_asm_end:\n\t"
        "nop\n\t"
        : : : "memory"
    );
}

// 页错误处理程序 (中断14)
void page_fault_handler_asm(void) {
    __asm__ volatile(
        // 保存所有寄存器（除了错误码已经在栈上）
        "push %%rax\n\t"
        "push %%rbx\n\t"
        "push %%rcx\n\t"
        "push %%rdx\n\t"
        "push %%rsi\n\t"
        "push %%rdi\n\t"
        "push %%rbp\n\t"
        "push %%r8\n\t"
        "push %%r9\n\t"
        "push %%r10\n\t"
        "push %%r11\n\t"
        "push %%r12\n\t"
        "push %%r13\n\t"
        "push %%r14\n\t"
        "push %%r15\n\t"
        
        // 设置中断号，错误码已经在栈上
        "mov $14, %%rax\n\t"         // 中断号=14
        "jmp common_interrupt_handler\n\t"
        
        ".global page_fault_handler_asm_end\n\t"
        "page_fault_handler_asm_end:\n\t"
        "nop\n\t"
        : : : "memory"
    );
}

// 通用的中断处理代码
__asm__ (
    ".text\n"
    "common_interrupt_handler:\n\t"
    
    // 此时栈布局：
    // r15, r14, r13, r12, r11, r10, r9, r8
    // rbp, rdi, rsi, rdx, rcx, rbx, rax
    // 错误码 (如果有)
    // 中断号
    // 原始栈顶
    
    // 构建InterruptFrame结构
    "mov %rsp, %rdi\n\t"           // 将栈指针作为第一个参数
    "call interrupt_handler\n\t"     // 调用C中断处理函数
    
    // 恢复寄存器
    "pop %r15\n\t"
    "pop %r14\n\t"
    "pop %r13\n\t"
    "pop %r12\n\t"
    "pop %r11\n\t"
    "pop %r10\n\t"
    "pop %r9\n\t"
    "pop %r8\n\t"
    "pop %rbp\n\t"
    "pop %rdi\n\t"
    "pop %rsi\n\t"
    "pop %rdx\n\t"
    "pop %rcx\n\t"
    "pop %rbx\n\t"
    "pop %rax\n\t"
    
    // 清理错误码和中断号
    "add $16, %rsp\n\t"
    
    // 中断返回
    "iretq\n\t"
);

// 初始化IDT
void initIDT(void) {
    // 初始化IDT指针
    idt_ptr.limit = sizeof(idt) - 1;
    idt_ptr.base = (uint64_t)&idt;
    
    // 清空中断处理函数数组
    memset(interrupt_handlers, 0, sizeof(interrupt_handlers));
    
    // 设置异常处理程序
    set_idt_entry(0, (void*)divide_error_handler_asm, 0x08, 0x8E);
    set_idt_entry(8, (void*)double_fault_handler_asm, 0x08, 0x8E);
    set_idt_entry(14, (void*)page_fault_handler_asm, 0x08, 0x8E);
    
    // 注册异常处理函数
    register_interrupt_handler(0, divide_error_handler);
    register_interrupt_handler(8, double_fault_handler);
    register_interrupt_handler(14, page_fault_handler);
    
    // 加载IDT
    __asm__ volatile("lidt %0" : : "m" (idt_ptr));
    
    printk("IDT initialized.\n");
}