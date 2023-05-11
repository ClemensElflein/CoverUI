#include "Atm_button_matrix.h"

Atm_button_matrix &Atm_button_matrix::begin(int p_power_source_pin, int attached_pin) {
    Atm_button::begin(attached_pin);

    // Reset pin to INPUT_PULLDOWN
    pinMode(attached_pin, INPUT_PULLDOWN);

    power_source_pin = p_power_source_pin;
    pinMode(p_power_source_pin, OUTPUT);
    digitalWrite(power_source_pin, LOW);

    return *this;
}

int Atm_button_matrix::event(int id) {
    int state;
    switch (id) {
        case EVT_LMODE:
            return counter_longpress.value > 0;
        case EVT_TIMER:
            return timer_debounce.expired(this);
        case EVT_DELAY:
            return timer_delay.expired(this);
        case EVT_REPEAT:
            return timer_repeat.expired(this);
        case EVT_AUTO:
            return timer_auto.expired(this);
        case EVT_PRESS:
            digitalWrite(power_source_pin, HIGH);
            delay(1);
            state = digitalRead(pin);
            digitalWrite(power_source_pin, LOW);
            return state;
        case EVT_RELEASE:
            digitalWrite(power_source_pin, HIGH);
            delay(1);
            state = digitalRead(pin);
            digitalWrite(power_source_pin, LOW);
            return !state;
        case EVT_COUNTER:
            return counter_longpress.expired();
    }
    return 0;
}
