#include "Atm_virtual_led.hpp"

Atm_virtual_led& Atm_virtual_led::begin( void ) {
  // clang-format off
  static const state_t state_table[] PROGMEM = {
    /*               ON_ENTER    ON_LOOP    ON_EXIT  EVT_ON_TIMER  EVT_OFF_TIMER EVT_WT_TIMER EVT_COUNTER  EVT_ON  EVT_OFF  EVT_BLINK  EVT_TOGGLE  EVT_TOGGLE_BLINK   ELSE */
    /* IDLE      */  ENT_INIT, ATM_SLEEP,        -1,           -1,            -1,          -1,         -1,  WT_ON,      -1,  WT_START,         ON,         WT_START,    -1, // LED off
    /* ON        */    ENT_ON, ATM_SLEEP,        -1,           -1,            -1,          -1,         -1,     -1,     OFF,  WT_START,        OFF,              OFF,    -1, // LED on
    /* START     */    ENT_ON,        -1,        -1,    BLINK_OFF,            -1,          -1,         -1,  WT_ON,     OFF,        -1,        OFF,              OFF,    -1, // Start blinking
    /* BLINK_OFF */   ENT_OFF,        -1,        -1,           -1,          LOOP,          -1,         -1,  WT_ON,     OFF,        -1,        OFF,              OFF,    -1,
    /* LOOP      */        -1,        -1,        -1,           -1,            -1,          -1,       DONE,  WT_ON,     OFF,        -1,        OFF,              OFF, START,    
    /* DONE      */        -1,        -1, EXT_CHAIN,           -1,           OFF,          -1,         -1,  WT_ON,     OFF,  WT_START,        OFF,              OFF,    -1, // Wait after last blink
    /* OFF       */   ENT_OFF,        -1,        -1,           -1,            -1,          -1,         -1,  WT_ON,     OFF,  WT_START,         -1,               -1,  IDLE, // All off -> IDLE
    /* WT_ON     */        -1,        -1,        -1,           -1,            -1,          ON,         -1,  WT_ON,     OFF,  WT_START,         -1,               -1,    -1, // LEAD for ON
    /* WT_START  */        -1,        -1,        -1,           -1,            -1,       START,         -1,  WT_ON,     OFF,  WT_START,         -1,               -1,    -1, // LEAD for BLINK
  }; 
  // clang-format on
  Machine::begin( state_table, ELSE );
  on_timer.set( 500 );
  off_timer.set( 500 );
  pwm( 512, 1 );
  lead_timer.set( 0 );
  repeat_count = ATM_COUNTER_OFF;
  counter.set( repeat_count );
  while ( state() != 0 ) cycle();
  return *this;
}

Atm_virtual_led& Atm_virtual_led::pwm( uint16_t width, float freq ) {

    if ( freq > -1 ) {	
		this->freq = freq;
	} else {
		freq = this->freq;
	}
	this->width = width;
	float cycle_width = 1000 / freq;
	on_timer.set( cycle_width / 1024 * this->width );
	off_timer.set( cycle_width / 1024 * ( 1024 - this->width ) );
	return *this;
}

Atm_virtual_led& Atm_virtual_led::frequency( float freq ) {

	this->freq = freq;
	float cycle_width = 1000 / freq;
	on_timer.set( cycle_width / 1024 * this->width );
	off_timer.set( cycle_width / 1024 * ( 1024 - this->width ) );
	return *this;
}

int Atm_virtual_led::event( int id ) {
  switch ( id ) {
    case EVT_ON_TIMER:
      return on_timer.expired( this );
    case EVT_OFF_TIMER:
      return off_timer.expired( this );
    case EVT_WT_TIMER:
      return lead_timer.expired( this );
    case EVT_COUNTER:
      return counter.expired();
  }
  return 0;
}

void Atm_virtual_led::action( int id ) {
  switch ( id ) {
    case ENT_INIT:
      counter.set( repeat_count );
      return;
    case ENT_ON:
      if ( on_timer.value > 0 ) { // Never turn if on_timer is zero (duty cycle 0 must be dark)
          oneventon.push( 1 );
      }
      return;
    case ENT_OFF:
      counter.decrement();
      oneventoff.push( 0 );
      return;
    case EXT_CHAIN:
      onfinish.push( 0 );
      return;
  }
}

Atm_virtual_led& Atm_virtual_led::on( void ) {
  trigger( EVT_ON );
  return *this;
}

Atm_virtual_led& Atm_virtual_led::off( void ) {
  trigger( EVT_OFF );
  return *this;
}

Atm_virtual_led& Atm_virtual_led::toggle( void ) {
  trigger( EVT_TOGGLE );
  return *this;
}

Atm_virtual_led& Atm_virtual_led::toggleBlink( void ) {
  trigger( EVT_TOGGLE_BLINK );
  return *this;
}

Atm_virtual_led& Atm_virtual_led::start( void ) {
  trigger( EVT_BLINK );
  return *this;
}

Atm_virtual_led& Atm_virtual_led::onFinish( Machine& machine, int event /* = 0 */ ) {
  onfinish.set( &machine, event );
  return *this;
}

Atm_virtual_led& Atm_virtual_led::onFinish( atm_cb_push_t callback, int idx /* = 0 */ ) {
  onfinish.set( callback, idx );
  return *this;
}

Atm_virtual_led& Atm_virtual_led::blink( uint32_t duration, uint32_t pause_duration, uint16_t repeat_count /* = ATM_COUNTER_OFF */ ) {
  blink( duration );  // Time in which led is fully on
  pause( pause_duration );
  repeat( repeat_count );
  return *this;
}

Atm_virtual_led& Atm_virtual_led::blink( uint32_t duration ) {
  on_timer.set( duration );  // Time in which led is fully on
  return *this;
}

Atm_virtual_led& Atm_virtual_led::blink( void ) {
  trigger( EVT_BLINK );
  return *this;
}

Atm_virtual_led& Atm_virtual_led::pause( uint32_t duration ) {  // Time in which led is fully off
  off_timer.set( duration ? duration : 1 );     // Make sure off_timer is never 0 (work around)
  return *this;
}

Atm_virtual_led& Atm_virtual_led::fade( int fade ) {
  return *this;
}  // Dummy for method compatibility with Atm_fade

Atm_virtual_led& Atm_virtual_led::lead( uint32_t ms ) {
  lead_timer.set( ms );
  return *this;
} 

Atm_virtual_led& Atm_virtual_led::repeat( uint16_t repeat ) {
  counter.set( repeat_count = repeat );
  return *this;
}

Atm_virtual_led& Atm_virtual_led::onEventOn(atm_cb_push_t callback, int idx /* = 0 */ ) {
    oneventon.set( callback, idx );
    return *this;
}

Atm_virtual_led& Atm_virtual_led::onEventOff(atm_cb_push_t callback, int idx /* = 0 */ ) {
    oneventoff.set( callback, idx );
    return *this;
}

Atm_virtual_led& Atm_virtual_led::trace( Stream& stream ) {
  setTrace( &stream, atm_serial_debug::trace,
            "LED\0EVT_ON_TIMER\0EVT_OFF_TIMER\0EVT_WT_TIMER\0EVT_COUNTER\0EVT_ON\0EVT_OFF\0EVT_"
            "BLINK\0EVT_TOGGLE\0EVT_TOGGLE_BLINK\0ELSE\0"
            "IDLE\0ON\0START\0BLINK_OFF\0LOOP\0DONE\0OFF\0WT_ON\0WT_START" );
  return *this;
}
