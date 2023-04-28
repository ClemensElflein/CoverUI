#pragma once

#include <Automaton.h>

class Atm_virtual_led : public Machine {
 public:
  enum { IDLE, ON, START, BLINK_OFF, LOOP, DONE, OFF, WT_ON, WT_START };
  enum { EVT_ON_TIMER, EVT_OFF_TIMER, EVT_WT_TIMER, EVT_COUNTER, EVT_ON, EVT_OFF, EVT_BLINK, EVT_TOGGLE, EVT_TOGGLE_BLINK, ELSE, EVT_BRUP, EVT_BRDN }; // BRUP/BRDN pseudo
  enum { EVT_START = EVT_BLINK };

  Atm_virtual_led( void ) : Machine(){};
  Atm_virtual_led& begin( void );
  Atm_virtual_led& blink( void );
  Atm_virtual_led& blink( uint32_t duration );
  Atm_virtual_led& blink( uint32_t duration, uint32_t pause_duration, uint16_t repeat_count = ATM_COUNTER_OFF );
  Atm_virtual_led& pwm( uint16_t width, float freq = -1 );
  Atm_virtual_led& frequency( float freq );
  Atm_virtual_led& pause( uint32_t duration );
  Atm_virtual_led& fade( int fade );
  Atm_virtual_led& lead( uint32_t ms );
  Atm_virtual_led& repeat( uint16_t repeat );
  Atm_virtual_led& on( void );
  Atm_virtual_led& off( void );
  Atm_virtual_led& toggle( void );
  Atm_virtual_led& toggleBlink( void );
  Atm_virtual_led& start( void );
  Atm_virtual_led& trace(arduino::Stream&);
  Atm_virtual_led& onFinish( Machine& machine, int event = 0 );
  Atm_virtual_led& onFinish( atm_cb_push_t callback, int idx = 0 );

  Atm_virtual_led& onEventOn( atm_cb_push_t callback, int idx = 0 );
  Atm_virtual_led& onEventOff( atm_cb_push_t callback, int idx = 0 );

protected:
    atm_connector oneventon, oneventoff;

 private:
  enum { ENT_INIT, ENT_ON, ENT_OFF, EXT_CHAIN };
  uint16_t repeat_count;
  uint16_t width;
  float freq;
  atm_timer_millis on_timer, off_timer, lead_timer;
  atm_counter counter;
  atm_connector onfinish;

  int event( int id );
  void action( int id );
};
