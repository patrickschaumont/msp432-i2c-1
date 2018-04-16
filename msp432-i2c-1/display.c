#include "display.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"

Graphics_Context g_sContext;

void InitDisplay() {
    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);
    Graphics_initContext(&g_sContext,
                         &g_sCrystalfontz128x128,
                         &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_CYAN);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_PURPLE);
    Graphics_setFont(&g_sContext, &g_sFontCmtt16);
    Graphics_clearDisplay(&g_sContext);
}

void DisplayTemperature(unsigned t) {
    char tempstr[ 9] = "TMP: XXXX";
    char hexstr[16] = "0123456789ABCDEF";
    tempstr[5] = hexstr[(t >> 12) & 0xF];
    tempstr[6] = hexstr[(t >>  8) & 0xF];
    tempstr[7] = hexstr[(t >>  4) & 0xF];
    tempstr[8] = hexstr[(t      ) & 0xF];

    Graphics_drawStringCentered(&g_sContext, (int8_t *) tempstr, 9,  64,  50, OPAQUE_TEXT);
}

void DisplayLight(unsigned t) {
    char tempstr[ 9] = "LGT: XXXX";
    char hexstr[16] = "0123456789ABCDEF";
    tempstr[5] = hexstr[(t >> 12) & 0xF];
    tempstr[6] = hexstr[(t >>  8) & 0xF];
    tempstr[7] = hexstr[(t >>  4) & 0xF];
    tempstr[8] = hexstr[(t      ) & 0xF];

    Graphics_drawStringCentered(&g_sContext, (int8_t *) tempstr, 9,  64,  70, OPAQUE_TEXT);
}

