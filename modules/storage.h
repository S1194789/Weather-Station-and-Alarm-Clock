#ifndef STORAGE_H
#define STORAGE_H

#include <stdint.h>

// ---- Record struct ----
typedef struct {
    uint8_t h, m, s;  // time
    uint8_t T;        // temp
    uint8_t L;        // light
} rec_t;

// ---- API ----
void STORAGE_Init(void);
void STORAGE_Reset(void);
void STORAGE_Update(uint8_t T, uint8_t L,
                    uint8_t h, uint8_t m, uint8_t s);
void STORAGE_Reset(void);

rec_t STORAGE_GetTmax(void);
rec_t STORAGE_GetTmin(void);
rec_t STORAGE_GetLmax(void);
rec_t STORAGE_GetLmin(void);

#endif
