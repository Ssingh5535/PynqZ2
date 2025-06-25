#include "xparameters.h"
#include "xgpio.h"
#include "xil_printf.h"

#define GPIO0_BASE      XPAR_XGPIO_0_BASEADDR

// If GPIO (chan 1) is LEDs, and GPIO2 (chan 2) is Buttons:
#define LED_CHAN        1
#define BTN_CHAN        2

// And SW is still on GPIO1 (the single-channel AXI_GPIO_1):
#define SW_GPIO_BASE    XPAR_XGPIO_1_BASEADDR
#define SW_CHAN         1  // only channel of that block

int main()
{
    XGpio gpio0;
    XGpio gpioSw;

    // Init both blocks:
    XGpio_Initialize(&gpio0,    GPIO0_BASE);
    XGpio_Initialize(&gpioSw,   SW_GPIO_BASE);

    // Set directions:
    XGpio_SetDataDirection(&gpio0, LED_CHAN, 0x0);  // LEDs = outputs
    XGpio_SetDataDirection(&gpio0, BTN_CHAN, 0xF);  // Buttons = inputs (4 bits)
    XGpio_SetDataDirection(&gpioSw,  SW_CHAN,  0x3);  // SW0–1 = inputs

    xil_printf("GPIO loop (swapped channels) starting...\r\n");

    u32 prev_buttons = 0xFF, prev_sw = 0xFF;

    while (1) {
        u32 btns = XGpio_DiscreteRead(&gpio0, BTN_CHAN) & 0xF;
        u32 sw0  = XGpio_DiscreteRead(&gpioSw,  SW_CHAN) & 0x1;

        if (btns != prev_buttons || sw0 != prev_sw) {
            // Compute LEDs (direct or reversed)
            u32 leds = sw0
                       ? btns
                       : ((btns & 1)<<3) | (((btns>>1)&1)<<2)
                         | (((btns>>2)&1)<<1) | (((btns>>3)&1)<<0);

            // Write them out on the LED channel:
            XGpio_DiscreteWrite(&gpio0, LED_CHAN, leds);

            // Print what we saw:
            xil_printf("BTN=0x%X  SW0=%d  LED=0x%X\r\n", btns, sw0, leds);

            prev_buttons = btns;
            prev_sw      = sw0;
        }

        // small delay to debounce/slow prints
        for (volatile int i = 0; i < 100000; i++);
    }

    return 0;
}
