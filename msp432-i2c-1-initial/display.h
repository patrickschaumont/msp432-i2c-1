#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <ti/grlib/grlib.h>

void InitDisplay();
void DisplayTemperature(unsigned t);
void DisplayLight      (unsigned t);

#endif /* DISPLAY_H_ */
