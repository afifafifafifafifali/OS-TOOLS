#include "idt.h"
#include "utils.h"
#include "types.h"

void set_idt_gate(int n, uint32 handler) {
    idt[n].low_offset = low_16(handler);
    idt[n].sel = KERNEL_CS;
    idt[n].always0 = 0;
    idt[n].flags = 0x8E; 
    idt[n].high_offset = high_16(handler);
}

void set_idt() {
    idt_reg.base = (uint64) &idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    __asm__ __volatile__("lidt (%0)" : : "r" (&idt_reg)); //previously lidtl
}

/*
void set_idt() {
    idt_reg.base = (uint64_t)&idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    __asm__ __volatile__("lidt (%0)" : : "r"(&idt_reg));
}
 oai code */