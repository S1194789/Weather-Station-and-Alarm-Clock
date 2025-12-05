#include <xc.h>
#include "system.h"
#include "clock.h"
#include "buttons.h"
#include "sensors.h"
#include "alarms.h"
#include "ui.h"
#include "i2c.h"
#include "lcd.h"
#include "storage.h"


/*
                         Main application
 */


void main(void)
{
    // initialize the device
    system_init();   
    ui_init();
    sensors_init();
    STORAGE_Init();

    while (1)
    {
        if(system.flags.one_second)
        {
            system.flags.one_second = 0;
            clock_update_1s();
            sensors_update();
            STORAGE_Update(temperature_value,
               luminosity_value,
               system.clock.hour,
               system.clock.min,
               system.clock.sec);
        }    
        buttons_update();
        alarms_update();
        ui_update();
       
       

        

    }
}
/**
 End of File
*/