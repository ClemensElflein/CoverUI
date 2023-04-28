#ifndef COVERUI_TEST_ATM_BUTTON_MATRIX_H
#define COVERUI_TEST_ATM_BUTTON_MATRIX_H

#include <Automaton.h>

class Atm_button_matrix : public Atm_button {

public:
    Atm_button_matrix(void) : Atm_button() {};

    Atm_button_matrix &begin(int p_power_source_pin, int attached_pin);

    int event(int id);

protected:
    short power_source_pin;

};


#endif //COVERUI_TEST_ATM_BUTTON_MATRIX_H
