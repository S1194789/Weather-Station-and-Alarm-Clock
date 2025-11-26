/* ui.h */

#ifndef UI_H
#define UI_H

typedef enum {
    UI_NORMAL = 0,   // Normal mode

    // ---- CONFIG MODE: (PDF: hh,mm,ss, C,T,L, A, R) ----
    UI_CFG_HOUR,
    UI_CFG_MIN,
    UI_CFG_SEC,
    UI_CFG_C,        // clock alarm 
    UI_CFG_T,        // temp threshold
    UI_CFG_L,        // lux threshold
    UI_CFG_ALARM_EN, // A/a 
    UI_CFG_RESET     // reset
} ui_state_t;

extern ui_state_t ui_state;

void ui_init(void);
void ui_update(void);
void ui_next_state(void);   // S1
void ui_select(void);       // S2

#endif
