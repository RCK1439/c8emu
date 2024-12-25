#ifndef CPU_H
#define CPU_H

#include <stdint.h>

void cpu_init(void);

void cpu_step(void);

void cpu_set_key(uint8_t key, uint8_t val);

void cpu_draw_buffer(void);

#endif /* CPU_H */

