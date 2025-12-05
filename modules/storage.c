#include "storage.h"
#include "memory.h"

#define A_MAGIC   100
#define MAGIC_VAL 0xA5

#define A_TMAX  0
#define A_TMIN  5
#define A_LMAX 10
#define A_LMIN 15

static rec_t r_tmax, r_tmin, r_lmax, r_lmin;

// --- write ---
static void save_rec(uint8_t addr, rec_t r)
{
    DATAEE_WriteByte(addr,     r.h);
    DATAEE_WriteByte(addr + 1, r.m);
    DATAEE_WriteByte(addr + 2, r.s);
    DATAEE_WriteByte(addr + 3, r.T);
    DATAEE_WriteByte(addr + 4, r.L);
}

// --- read ---
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

// --- invalid (0xFF) detect ---
static uint8_t invalid(rec_t r)
{
    return (r.h == 0xFF || r.m == 0xFF || r.s == 0xFF ||
            r.T == 0xFF || r.L == 0xFF);
}

// --- defaults ---
static void set_defaults(void)
{
    r_tmax = (rec_t){0,0,0, 0,0};
    r_tmin = (rec_t){0,0,0, 50,0};

    r_lmax = (rec_t){0,0,0, 0,0};
    r_lmin = (rec_t){0,0,0, 0,3};

    save_rec(A_TMAX, r_tmax);
    save_rec(A_TMIN, r_tmin);
    save_rec(A_LMAX, r_lmax);
    save_rec(A_LMIN, r_lmin);

    DATAEE_WriteByte(A_MAGIC, MAGIC_VAL);
}

// --- init ---
void STORAGE_Init(void)
{
    uint8_t m = DATAEE_ReadByte(A_MAGIC);

    if (m != MAGIC_VAL)
    {
        set_defaults();
        return;
    }

    r_tmax = load_rec(A_TMAX);
    r_tmin = load_rec(A_TMIN);
    r_lmax = load_rec(A_LMAX);
    r_lmin = load_rec(A_LMIN);

    if (invalid(r_tmax) || invalid(r_tmin) ||
        invalid(r_lmax) || invalid(r_lmin))
    {
        set_defaults();
    }
}

// --- reset ---
void STORAGE_Reset(void)
{
    set_defaults();
}

// --- update ---
void STORAGE_Update(uint8_t T, uint8_t L,
                    uint8_t h, uint8_t m, uint8_t s)
{
    rec_t r = {h,m,s, T,L};

    if (T > r_tmax.T) { r_tmax = r; save_rec(A_TMAX, r_tmax); }
    if (T < r_tmin.T) { r_tmin = r; save_rec(A_TMIN, r_tmin); }
    if (L > r_lmax.L) { r_lmax = r; save_rec(A_LMAX, r_lmax); }
    if (L < r_lmin.L) { r_lmin = r; save_rec(A_LMIN, r_lmin); }
}

rec_t STORAGE_GetTmax(void){ return r_tmax; }
rec_t STORAGE_GetTmin(void){ return r_tmin; }
rec_t STORAGE_GetLmax(void){ return r_lmax; }
rec_t STORAGE_GetLmin(void){ return r_lmin; }
