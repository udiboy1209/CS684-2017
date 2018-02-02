#include "joystick.h"
#include "driverlib/uart.h"

uint32_t adcSamples[4];
char numx[5], numy[5];

void configJoystick() {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    GPIOPinTypeADC(JOY_GPIO_BASE, JOY_X | JOY_Y);

    ADCSequenceDisable(JOY_ADC_BASE, 1);

    ADCSequenceConfigure(JOY_ADC_BASE, 1, ADC_TRIGGER_TIMER, 0);
    ADCSequenceStepConfigure(JOY_ADC_BASE, 1, 0, JOY_CH_X);
    ADCSequenceStepConfigure(JOY_ADC_BASE, 1, 1, JOY_CH_Y);
    ADCSequenceStepConfigure(JOY_ADC_BASE, 1, 2, JOY_CH_X);
    ADCSequenceStepConfigure(JOY_ADC_BASE, 1, 3, JOY_CH_Y | ADC_CTL_IE | ADC_CTL_END);

    ADCIntRegister(JOY_ADC_BASE, 1, ADC0IntHandler);
    ADCIntEnable(JOY_ADC_BASE, 1);

    TimerConfigure(JOY_TIMER_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(JOY_TIMER_BASE, TIMER_A, SysCtlClockGet()/JOY_SAMPLE_RATE);
    TimerControlTrigger(JOY_TIMER_BASE, TIMER_A, true);

    ADCSequenceEnable(JOY_ADC_BASE, 1);
    TimerEnable(JOY_TIMER_BASE, TIMER_A);
}

void int2str(uint32_t num, char * str) {
    int8_t i;
    for(i = 4; i >= 0; i--) {
        str[i] = num%10 + 48;
        num = num/10;
    }
}

void ADC0IntHandler(void) {
    uint8_t i;

    ADCSequenceDataGet(JOY_ADC_BASE, 1, adcSamples);

    int2str(adcSamples[0], numx);
    int2str(adcSamples[1], numy);

    for(i = 0; i < 5; i++) { UARTCharPut(UART0_BASE, numx[i]); }
    UARTCharPut(UART0_BASE, ',');
    for(i = 0; i < 5; i++) { UARTCharPut(UART0_BASE, numy[i]); }
    UARTCharPut(UART0_BASE, '\n');
}
