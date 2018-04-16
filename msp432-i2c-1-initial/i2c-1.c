#include <stdlib.h>
#include "swtimer.h"
#include "hwtimer.h"
#include "display.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

const eUSCI_I2C_MasterConfig i2cConfig = {
        EUSCI_B_I2C_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        3000000,                                // SMCLK = 3MHz
        EUSCI_B_I2C_SET_DATA_RATE_400KBPS,      // Desired I2C Clock of 400khz
        0,                                      // No byte counter threshold
        EUSCI_B_I2C_NO_AUTO_STOP                // No Autostop
};

void InitI2C() {
    // Configure output pins
    // GPIO P6.5 UCB1 SCL 2nd peripheral
    GPIO_setAsPeripheralModuleFunctionInputPin(
            GPIO_PORT_P6,
            GPIO_PIN5,
            GPIO_PRIMARY_MODULE_FUNCTION);

    // GPIO P6.4 UCB1 SDA 2nd peripheral
    GPIO_setAsPeripheralModuleFunctionInputPin(
            GPIO_PORT_P6,
            GPIO_PIN4,
            GPIO_PRIMARY_MODULE_FUNCTION);
}

// I2C address of light sensor
#define OPT3001_SLAVE_ADDRESS   0x44

#define OPT3001_DEFAULT_CONFIG  0xC400
#define OPT3001_HIGH_THRESHOLD  0x2F00
#define OPT3001_LOW_THRESHOLD   0x0F00

// device register addresses
#define OPT3001_P_MAN_ID        0x7E
#define OPT3001_P_DEVICE_ID     0x7F
#define OPT3001_P_RESULT        0x00
#define OPT3001_P_CONFIG        0x01
#define OPT3001_P_LOW           0x02
#define OPT3001_P_HIGH          0x03

unsigned ReadLight() {
    // to be defined by you




















}


// I2C address of temperature sensor
#define TMP006_SLAVE_ADDRESS    0x40

// device register addresses
#define TMP006_P_MAN_ID         0xFE
#define TMP006_P_DEVICE_ID      0xFF
#define TMP006_P_TAMBIENT       0x01
#define TMP006_P_VOBJECT        0x00

unsigned ReadTemperature() {
    unsigned t = 0;

    // Master Configuration
    I2C_initMaster     (EUSCI_B1_BASE, &i2cConfig);
    I2C_setSlaveAddress(EUSCI_B1_BASE, TMP006_SLAVE_ADDRESS);
    I2C_enableModule   (EUSCI_B1_BASE);

    // Select TMP006_P_VOBJECT register
    I2C_setMode(EUSCI_B1_BASE, EUSCI_B_I2C_TRANSMIT_MODE);
    I2C_masterSendSingleByte(EUSCI_B1_BASE, TMP006_P_VOBJECT);

    // Receive DEVICE ID data
    I2C_masterReceiveStart(EUSCI_B1_BASE);
    while(!(I2C_getInterruptStatus(EUSCI_B1_BASE, EUSCI_B_I2C_RECEIVE_INTERRUPT0)));
    t = I2C_masterReceiveMultiByteNext(EUSCI_B1_BASE);
    t = (t << 8) | I2C_masterReceiveMultiByteFinish(EUSCI_B1_BASE);

    return t;
}

int main(void) {
    unsigned t;

    InitTimer();
    InitDisplay();
    InitI2C();

    tSWTimer oneshot1000ms;
    InitSWTimer (&oneshot1000ms, TIMER32_1_BASE, 3000000);
    StartSWTimer(&oneshot1000ms);

    // INT connection
    GPIO_setAsInputPin(GPIO_PORT_P4, GPIO_PIN6);

    // RED LED
    GPIO_setAsOutputPin    (GPIO_PORT_P2,    GPIO_PIN6);
    GPIO_setOutputLowOnPin (GPIO_PORT_P2,    GPIO_PIN6);

    while (1) {

        if (GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN6) == GPIO_INPUT_PIN_HIGH) {
            GPIO_setOutputHighOnPin (GPIO_PORT_P2, GPIO_PIN6);
        } else {
            GPIO_setOutputLowOnPin (GPIO_PORT_P2,  GPIO_PIN6);
        }

        if (SWTimerOneShotExpired(&oneshot1000ms)) {
            t = ReadTemperature();
            DisplayTemperature(t);
            t = ReadLight();
            DisplayLight(t);
            StartSWTimer(&oneshot1000ms);
        }

    }
}
