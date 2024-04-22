#include "cpu.h"

#include <stdint.h>

#define NUM_REGISTERS 16
#define STACK_SIZE 32

#define SCREEN_ADDRESS 0x0F00
#define STACK_POINTER_ADDRESS 0x0FA0
#define PROGRAM_COUNTER_ADDRESS 0x0200

typedef struct cpu_context_s {
    uint8_t v[NUM_REGISTERS];
    
    uint16_t i;
    uint16_t sp;
    uint16_t pc;

    uint8_t dt;
    uint8_t st;
} cpu_context_t;

static cpu_context_t ctx;

void cpu_init(void)
{
    ctx = (cpu_context_t) { 0 };
    ctx.sp = STACK_POINTER_ADDRESS;
    ctx.pc = PROGRAM_COUNTER_ADDRESS;
}

void cpu_run(void)
{

}