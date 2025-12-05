# 1 "modules/storage.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 295 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include/language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "modules/storage.c" 2
# 1 "modules/storage.h" 1



# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 1 3



# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/musl_xc8.h" 1 3
# 5 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 2 3
# 26 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 1 3
# 133 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef unsigned short uintptr_t;
# 148 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef short intptr_t;
# 164 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef signed char int8_t;




typedef short int16_t;




typedef __int24 int24_t;




typedef long int32_t;





typedef long long int64_t;
# 194 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef long long intmax_t;





typedef unsigned char uint8_t;




typedef unsigned short uint16_t;




typedef __uint24 uint24_t;




typedef unsigned long uint32_t;





typedef unsigned long long uint64_t;
# 235 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef unsigned long long uintmax_t;
# 27 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 2 3

typedef int8_t int_fast8_t;

typedef int64_t int_fast64_t;


typedef int8_t int_least8_t;
typedef int16_t int_least16_t;

typedef int24_t int_least24_t;
typedef int24_t int_fast24_t;

typedef int32_t int_least32_t;

typedef int64_t int_least64_t;


typedef uint8_t uint_fast8_t;

typedef uint64_t uint_fast64_t;


typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;

typedef uint24_t uint_least24_t;
typedef uint24_t uint_fast24_t;

typedef uint32_t uint_least32_t;

typedef uint64_t uint_least64_t;
# 148 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/stdint.h" 1 3
typedef int16_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
# 149 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 2 3
# 5 "modules/storage.h" 2


typedef struct {
    uint8_t h, m, s;
    uint8_t T;
    uint8_t L;
} rec_t;


void STORAGE_Init(void);
void STORAGE_Reset(void);
void STORAGE_Update(uint8_t T, uint8_t L,
                    uint8_t h, uint8_t m, uint8_t s);
void STORAGE_Reset(void);

rec_t STORAGE_GetTmax(void);
rec_t STORAGE_GetTmin(void);
rec_t STORAGE_GetLmax(void);
rec_t STORAGE_GetLmin(void);
# 2 "modules/storage.c" 2
# 1 "mcc_generated_files/memory.h" 1
# 54 "mcc_generated_files/memory.h"
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdbool.h" 1 3
# 55 "mcc_generated_files/memory.h" 2
# 99 "mcc_generated_files/memory.h"
uint16_t FLASH_ReadWord(uint16_t flashAddr);
# 128 "mcc_generated_files/memory.h"
void FLASH_WriteWord(uint16_t flashAddr, uint16_t *ramBuf, uint16_t word);
# 164 "mcc_generated_files/memory.h"
int8_t FLASH_WriteBlock(uint16_t writeAddr, uint16_t *flashWordArray);
# 189 "mcc_generated_files/memory.h"
void FLASH_EraseBlock(uint16_t startAddr);
# 222 "mcc_generated_files/memory.h"
void DATAEE_WriteByte(uint16_t bAdd, uint8_t bData);
# 248 "mcc_generated_files/memory.h"
uint8_t DATAEE_ReadByte(uint16_t bAdd);
# 3 "modules/storage.c" 2
# 12 "modules/storage.c"
static rec_t r_tmax, r_tmin, r_lmax, r_lmin;


static void save_rec(uint8_t addr, rec_t r)
{
    DATAEE_WriteByte(addr, r.h);
    DATAEE_WriteByte(addr + 1, r.m);
    DATAEE_WriteByte(addr + 2, r.s);
    DATAEE_WriteByte(addr + 3, r.T);
    DATAEE_WriteByte(addr + 4, r.L);
}


static rec_t load_rec(uint8_t addr)
{
    rec_t r;
    r.h = DATAEE_ReadByte(addr);
    r.m = DATAEE_ReadByte(addr + 1);
    r.s = DATAEE_ReadByte(addr + 2);
    r.T = DATAEE_ReadByte(addr + 3);
    r.L = DATAEE_ReadByte(addr + 4);
    return r;
}


static uint8_t invalid(rec_t r)
{
    return (r.h == 0xFF || r.m == 0xFF || r.s == 0xFF ||
            r.T == 0xFF || r.L == 0xFF);
}


static void set_defaults(void)
{
    r_tmax = (rec_t){0,0,0, 0,0};
    r_tmin = (rec_t){0,0,0, 50,0};

    r_lmax = (rec_t){0,0,0, 0,0};
    r_lmin = (rec_t){0,0,0, 0,3};

    save_rec(0, r_tmax);
    save_rec(5, r_tmin);
    save_rec(10, r_lmax);
    save_rec(15, r_lmin);

    DATAEE_WriteByte(100, 0xA5);
}


void STORAGE_Init(void)
{
    uint8_t m = DATAEE_ReadByte(100);

    if (m != 0xA5)
    {
        set_defaults();
        return;
    }

    r_tmax = load_rec(0);
    r_tmin = load_rec(5);
    r_lmax = load_rec(10);
    r_lmin = load_rec(15);

    if (invalid(r_tmax) || invalid(r_tmin) ||
        invalid(r_lmax) || invalid(r_lmin))
    {
        set_defaults();
    }
}


void STORAGE_Reset(void)
{
    set_defaults();
}


void STORAGE_Update(uint8_t T, uint8_t L,
                    uint8_t h, uint8_t m, uint8_t s)
{
    rec_t r = {h,m,s, T,L};

    if (T > r_tmax.T) { r_tmax = r; save_rec(0, r_tmax); }
    if (T < r_tmin.T) { r_tmin = r; save_rec(5, r_tmin); }
    if (L > r_lmax.L) { r_lmax = r; save_rec(10, r_lmax); }
    if (L < r_lmin.L) { r_lmin = r; save_rec(15, r_lmin); }
}

rec_t STORAGE_GetTmax(void){ return r_tmax; }
rec_t STORAGE_GetTmin(void){ return r_tmin; }
rec_t STORAGE_GetLmax(void){ return r_lmax; }
rec_t STORAGE_GetLmin(void){ return r_lmin; }
