#ifndef CPU_H
#define CPU_H

#include <stdint.h>

void InitCPU(void);

void StepCPU(void);

void SetCPUKey(uint8_t key, uint8_t val);

void DrawCPUBuffer(void);

#endif /* CPU_H */

