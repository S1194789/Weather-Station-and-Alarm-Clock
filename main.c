#include <xc.h>
#include "system.h"
#include "clock.h"
#include "buttons.h"
#include "sensors.h"
#include "alarms.h"
#include "ui.h"
#include "i2c.h"
#include "lcd.h"


/*
                         Main application
 */


void main(void)
{
    // initialize the device
    system_init();   
    ui_init(); 

    while (1)
    {
        if(system.flags.one_second)
        {
            system.flags.one_second = 0;
            clock_update_1s();
        }    
        buttons_update();
        sensors_update();
        alarms_update();
        ui_update();
        

    }
}
/**
 End of File
*/