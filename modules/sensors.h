#ifndef SENSORS_H
#define SENSORS_H

extern unsigned char temperature_value;
extern unsigned char luminosity_value;

void sensors_init(void);
void sensors_update(void);
unsigned char readTC74(void);

#endif
