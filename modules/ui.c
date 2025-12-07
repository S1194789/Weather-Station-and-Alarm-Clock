#include "ui.h"
#include "system.h"
#include "lcd.h"
#include <stdio.h>
#include "i2c.h"
#include "sensors.h"
#include "storage.h"

#define _XTAL_FREQ 4000000
#define TINA_SEC 10   


extern unsigned char temperature_value;
extern unsigned char luminosity_value;

static void ui_normal(void);
static void ui_time_update(void);

ui_state_t ui_state = UI_NORMAL;
static uint8_t alarm_edit_step = 0;   // 0=h, 1=m, 2=s
static uint8_t record_page = 0;

// -----------------------------------
// UI init
// -----------------------------------
void ui_init(void)
{
    OpenI2C();
    LCDinit();
    __delay_ms(100);

    LCDcmd(0x01);
    LCDpos(0, 0);
    LCDstr("Loading...");
    __delay_ms(300);

    ui_normal();
}

// -----------------------------------
// Normal screen
// -----------------------------------
static void ui_normal(void)
{
    LCDcmd(0x01);
    LCDcmd(0x0C);      // cursor off
}

// -----------------------------------
// Update time + status line
// -----------------------------------
static void ui_time_update(void)
{
    char buf[17];

    char cC = system.alarms.active_C ? 'C' : ' ';
    char cT = system.alarms.active_T ? 'T' : ' ';
    char cL = system.alarms.active_L ? 'L' : ' ';
    char cA = system.alarms.enabled  ? 'A' : 'a';

    if (ui_state == UI_CFG_RESET)
    sprintf(buf, "%02d:%02d:%02d %c%c%c %c R",
            system.clock.hour,
            system.clock.min,
            system.clock.sec,
            cC, cT, cL, cA);
    else
        sprintf(buf, "%02d:%02d:%02d %c%c%c %c ",
                system.clock.hour,
                system.clock.min,
                system.clock.sec,
                cC, cT, cL, cA);


    LCDpos(0, 0);
    LCDstr(buf);

    LCDpos(1, 0);

    switch (ui_state)
    {
        case UI_NORMAL:
            sprintf(buf, "%02dC    L%d", temperature_value, luminosity_value);
            break;

        case UI_CFG_HOUR:
        case UI_CFG_MIN:
        case UI_CFG_SEC:
            sprintf(buf, "Set clock time   ");
            break;

        case UI_CFG_C:
            sprintf(buf, "AL %02d:%02d:%02d   ",
                    system.alarms.clk_h,
                    system.alarms.clk_m,
                    system.alarms.clk_s);
            break;

        case UI_CFG_T:
            sprintf(buf, "Tthr=%02d C       ", system.alarms.temp_thr);
            break;

        case UI_CFG_L:
            sprintf(buf, "Lthr=%d (0-3)     ", system.alarms.lum_thr);
            break;

        case UI_CFG_ALARM_EN:
            sprintf(buf, system.alarms.enabled ? "Alarms: ON       "
                                               : "Alarms: OFF      ");
            break;

        case UI_CFG_RESET:
            sprintf(buf, "Reset records S2 ");
            break;
    }

    LCDstr(buf);
}

// -----------------------------------
// S1 ? next menu step
// -----------------------------------
void ui_next_state(void)
{
    switch(ui_state)
    {
        case UI_NORMAL:
            ui_state = UI_CFG_HOUR;
            LCDcmd(0x0F);
            LCDpos(0,1);
            break;

        case UI_CFG_HOUR:
            ui_state = UI_CFG_MIN;
            LCDpos(0,4);
            break;

        case UI_CFG_MIN:
            ui_state = UI_CFG_SEC;
            LCDpos(0,7);
            break;

        case UI_CFG_SEC:
            ui_state = UI_CFG_C;
            alarm_edit_step = 0;
            LCDpos(1,4);       // alarm hour position
            break;

        case UI_CFG_C:   // ALARM TIME EDIT (3 steps)
            if (alarm_edit_step < 2)
            {
                alarm_edit_step++;  // move to m ? s
                if (alarm_edit_step == 1) LCDpos(1,7);
                if (alarm_edit_step == 2) LCDpos(1,10);
            }
            else
            {
                // finished h,m,s ? move to next config
                alarm_edit_step = 0;
                ui_state = UI_CFG_T;
                LCDpos(0,10);
            }
            break;

        case UI_CFG_T:
            ui_state = UI_CFG_L;
            LCDpos(0,11);
            break;

        case UI_CFG_L:
            ui_state = UI_CFG_ALARM_EN;
            LCDpos(0,13);
            break;

        case UI_CFG_ALARM_EN:
            ui_state = UI_CFG_RESET;
            LCDpos(1,0);
            break;

        case UI_CFG_RESET:
        default:
            ui_state = UI_NORMAL;
            ui_normal();
            break;
    }
}

// -----------------------------------
// S2 ? increment / actions / records
// -----------------------------------
void ui_select(void)
{
    // -------------------------------
    // Always reset records timeout
    // -------------------------------
    system.records_timer = 0;

    // -------------------------------
    // RESET (S2 confirms reset)
    // -------------------------------
    if (ui_state == UI_CFG_RESET)
    {
        STORAGE_Reset();

        LCDcmd(0x01);
        LCDpos(0,0);
        LCDstr("Reset Done");
        __delay_ms(150);

        ui_state = UI_NORMAL;
        system.mode = MODE_NORMAL;
        ui_normal();
        return;
    }

    // -------------------------------
    // RECORDS MODE page flip
    // -------------------------------
    if (system.mode == MODE_RECORDS)
    {
        if (record_page == 0)
            record_page = 1;
        else
        {
            record_page = 0;
            system.mode = MODE_NORMAL;
            ui_state = UI_NORMAL;
            ui_normal();
        }
        return;
    }

    // -------------------------------
    // NORMAL ? enter RECORDS MODE
    // -------------------------------
    if (ui_state == UI_NORMAL && system.mode == MODE_NORMAL)
    {
        system.mode = MODE_RECORDS;
        record_page = 0;
        ui_normal();
        return;
    }

    // -------------------------------
    // CONFIG MODE ? increment values
    // -------------------------------
    switch(ui_state)
    {
        case UI_CFG_HOUR:
            system.clock.hour = (system.clock.hour + 1) % 24;
            break;

        case UI_CFG_MIN:
            system.clock.min = (system.clock.min + 1) % 60;
            break;

        case UI_CFG_SEC:
            system.clock.sec = (system.clock.sec + 1) % 60;
            break;

        case UI_CFG_C:
            // alarm time editing
            if (alarm_edit_step == 0)
                system.alarms.clk_h = (system.alarms.clk_h + 1) % 24;
            else if (alarm_edit_step == 1)
                system.alarms.clk_m = (system.alarms.clk_m + 1) % 60;
            else if (alarm_edit_step == 2)
                system.alarms.clk_s = (system.alarms.clk_s + 1) % 60;
            break;

        case UI_CFG_T: 
            system.alarms.temp_thr = (system.alarms.temp_thr + 1) % 51;
            break;

        case UI_CFG_L:
            system.alarms.lum_thr = (system.alarms.lum_thr + 1) % 4;
            break;

        case UI_CFG_ALARM_EN:
            system.alarms.enabled ^= 1;
            break;

        default:
            break;
    }
}


// -----------------------------------
// Periodic UI update
// -----------------------------------
void ui_update(void)
{
    // -------------------------------------
    // RECORDS MODE ? show records + timeout
    // -------------------------------------
    if (system.mode == MODE_RECORDS)
    {
        ui_show_records(record_page);

        // Timeout counter (TINA)
        if (system.flags.one_second)
        {
            system.records_timer++;

            if (system.records_timer >= TINA_SEC)
            {
                system.mode = MODE_NORMAL;
                ui_state   = UI_NORMAL;
                ui_normal();
                system.records_timer = 0;
            }
        }
        return;
    }

    // -------------------------------------
    // Normal or Config Mode
    // -------------------------------------
    ui_time_update();

    // Keep cursor position
    switch(ui_state)
    {
        case UI_CFG_HOUR: LCDpos(0,1); break;
        case UI_CFG_MIN:  LCDpos(0,4); break;
        case UI_CFG_SEC:  LCDpos(0,7); break;

        case UI_CFG_C:
            switch (alarm_edit_step)
            {
                case 0: LCDpos(0,9);  break;
                case 1: LCDpos(1,4);  break;
                case 2: LCDpos(1,7);  break;
                case 3: LCDpos(1,10); break;
            }
            break;

        case UI_CFG_T: LCDpos(0,10); break;
        case UI_CFG_L: LCDpos(0,11); break;
        case UI_CFG_ALARM_EN: LCDpos(0,13); break;
        case UI_CFG_RESET: LCDpos(1,0); break;

        case UI_NORMAL:
        default:
            LCDcmd(0x0C); // cursor off
            break;
    }
}

// -----------------------------------
// Records screen
// -----------------------------------
void ui_show_records(uint8_t page)
{
    static uint8_t last_page = 0xFF;
    char buf[17];

    if (page != last_page)
    {
        LCDcmd(0x01);   // clear
        last_page = page;
    }

    if (page == 0)
    {
        rec_t tmax = STORAGE_GetTmax();
        rec_t tmin = STORAGE_GetTmin();

        LCDpos(0,0);
        sprintf(buf, "%02u:%02u:%02u %02dC L%u",
                tmax.h, tmax.m, tmax.s, tmax.T, tmax.L);
        LCDstr(buf);

        LCDpos(1,0);
        sprintf(buf, "%02u:%02u:%02u %02dC L%u",
                tmin.h, tmin.m, tmin.s, tmin.T, tmin.L);
        LCDstr(buf);
    }
    else
    {
        rec_t lmax = STORAGE_GetLmax();
        rec_t lmin = STORAGE_GetLmin();

        LCDpos(0,0);
        sprintf(buf, "%02u:%02u:%02u %02dC L%u",
                lmax.h, lmax.m, lmax.s, lmax.T, lmax.L);
        LCDstr(buf);

        LCDpos(1,0);
        sprintf(buf, "%02u:%02u:%02u %02dC L%u",
                lmin.h, lmin.m, lmin.s, lmin.T, lmin.L);
        LCDstr(buf);
    }
}
