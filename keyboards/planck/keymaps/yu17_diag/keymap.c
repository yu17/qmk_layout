/* yu17_diag - hardware check build of the yu17 Planck keymap.
 *
 * Same layout as yu17 (the keymap is included below), except that the LED
 * test key (Settings + L) runs a check and TYPES the result as text. Open a
 * text editor first. Each press moves to the next step:
 *
 *   1. report: LED data driver status, A1 pin probe, unused pins, RGB state;
 *      afterwards A1 is held HIGH (3.3 V) for measuring with a multimeter
 *   2. A1 held LOW (0 V)
 *   3. A1 back to normal LED data
 *
 * Unplugging also restores normal operation. See readme.md for how to read
 * the report.
 */
#define process_record_user yu17_process_record_user
#include "../yu17/keymap.c"
#undef process_record_user

#include <stdio.h>

#define DIAG_PIN A1 // WS2812 data out (TIM2_CH2, AF1)

static uint8_t diag_step = 0; // 0 = normal data, 1 = held high, 2 = held low

static void diag_send(const char *s) {
    send_string_with_delay(s, 5);
}

// Settle time after changing the pin mode: internal pull ~40 kOhm x trace
// and LED input capacitance is well under 1 us; wait far longer.
static bool diag_read_settled(void) {
    wait_us(500);
    return gpio_read_pin(DIAG_PIN) != 0;
}

static void diag_report(void) {
    char buf[96];

    // 1. Is the WS2812 driver running? Pin in alternate-function mode on
    //    AF1 (TIM2_CH2), timer counting, circular DMA moving.
    uint32_t moder = (GPIOA->MODER >> (1 * 2)) & 3;  // 2 = alternate function
    uint32_t af    = (GPIOA->AFRL >> (1 * 4)) & 15; // 1 = TIM2
    uint32_t cnt1 = TIM2->CNT, dma1 = DMA1_Channel2->CNDTR;
    wait_us(50);
    uint32_t cnt2 = TIM2->CNT, dma2 = DMA1_Channel2->CNDTR;
    bool tim_on = (TIM2->CR1 & TIM_CR1_CEN) && cnt1 != cnt2;
    bool dma_on = (DMA1_Channel2->CCR & DMA_CCR_EN) && dma1 != dma2;
    snprintf(buf, sizeof buf, "\nPLANCK DIAG\nDRIVER pinmode=%lu af=%lu timer=%s dma=%s\n", (unsigned long)moder, (unsigned long)af, tim_on ? "running" : "STOPPED", dma_on ? "running" : "STOPPED");
    diag_send(buf);

    // 2. Probe the data line with the internal pulls and by driving it.
    gpio_set_pin_input_high(DIAG_PIN);
    int pu = diag_read_settled();
    gpio_set_pin_input_low(DIAG_PIN);
    int pd = diag_read_settled();
    gpio_set_pin_output_push_pull(DIAG_PIN);
    gpio_write_pin_high(DIAG_PIN);
    int oh = diag_read_settled();
    gpio_write_pin_low(DIAG_PIN);
    int ol = diag_read_settled();

    const char *verdict;
    if (!oh || ol) {
        verdict = "PIN CANNOT DRIVE LINE - hard short or damaged MCU pin";
    } else if (pu && !pd) {
        verdict = "no short - line floats normally (fault is further out - LED or wiring)";
    } else if (!pu && !pd) {
        verdict = "line pulled LOW - short or leak to GND, or LEDs without 5V";
    } else if (pu && pd) {
        verdict = "line pulled HIGH - short or leak to a supply";
    } else {
        verdict = "inconsistent";
    }
    snprintf(buf, sizeof buf, "A1 pullup=%d pulldown=%d drivehigh=%d drivelow=%d\n", pu, pd, oh, ol);
    diag_send(buf);
    diag_send("A1 verdict ");
    diag_send(verdict);
    diag_send("\n");

#define R(pin) ((int)(gpio_read_pin(pin) != 0))
    // 3. Unused pins (DIP switch and encoder pads, pulled up by QMK):
    //    1 = open, which is expected when nothing is fitted.
    snprintf(buf, sizeof buf, "PADS dip B14=%d A15=%d A0=%d B9=%d enc B12=%d B13=%d\n", R(B14), R(A15), R(A0), R(B9), R(B12), R(B13));
    diag_send(buf);

    // 4. Saved underglow state (brightness 0 would also look dark).
    snprintf(buf, sizeof buf, "RGB enabled=%d mode=%d hue=%d sat=%d val=%d\n", (int)rgblight_is_enabled(), rgblight_get_mode(), rgblight_get_hue(), rgblight_get_sat(), rgblight_get_val());
    diag_send(buf);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode != LED_TST) {
        return yu17_process_record_user(keycode, record);
    }
    if (!record->event.pressed) {
        return false;
    }
    switch (diag_step) {
        case 0:
            diag_report(); // leaves A1 driven low
            gpio_write_pin_high(DIAG_PIN);
            diag_send("A1 now held HIGH - measure LED 1 DIN to GND (expect 3.3V). Press again for LOW\n");
            diag_step = 1;
            break;
        case 1:
            gpio_write_pin_low(DIAG_PIN);
            diag_send("A1 now held LOW (expect 0V). Press again to restore LED data\n");
            diag_step = 2;
            break;
        default:
            palSetLineMode(DIAG_PIN, PAL_MODE_ALTERNATE(1) | PAL_OUTPUT_TYPE_PUSHPULL | PAL_OUTPUT_SPEED_HIGHEST | PAL_PUPDR_FLOATING);
            diag_send("A1 back to LED data\n");
            diag_step = 0;
            break;
    }
    return false;
}

#ifdef DIAG_HOLD_HIGH
/* No-keys variant (build with -e EXTRAFLAGS=-DDIAG_HOLD_HIGH): 1.5 s after
 * power-up, A1 is switched from LED data to a steady HIGH (3.3 V) and stays
 * there, for measuring the data line on a bare board with a multimeter. */
void housekeeping_task_user(void) {
    static bool done = false;
    if (!done && timer_read32() > 1500) {
        gpio_set_pin_output_push_pull(DIAG_PIN);
        gpio_write_pin_high(DIAG_PIN);
        done = true;
    }
}
#endif
