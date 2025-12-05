#include "buttons.h"
#include "ui.h"
#include "system.h"
#include <xc.h>

uint8_t S1_pressed = 0;
uint8_t S2_pressed = 0;

void buttons_update(void)
{
    static uint8_t prev_S1 = 1;  
    static uint8_t prev_S2 = 1;

    uint8_t now_S1 = PORTBbits.RB4;   // Active Low 
    uint8_t now_S2 = PORTCbits.RC5;

    // ========= S1 (RB4) =========
    if (prev_S1 == 1 && now_S1 == 0)   // Falling Edge 
    {
        ui_next_state();
    }
    S1_pressed = (now_S1 == 0);

    // ========= S2 (RC5) =========
    if (prev_S2 == 1 && now_S2 == 0)   // Falling Edge
    {
        ui_select();
    }
    S2_pressed = (now_S2 == 0);


    prev_S1 = now_S1;
    prev_S2 = now_S2;
}
