#include "spin-nengo-ensemble.h"

int c_main( void )
{
  /* Initialise system as in the standard harness, see JK re correctly
   * integrating this code at some point. (We don't use delay buffers,
   * have dimension buffers, etc.)
   *
   *   - Setup routing table entries
   *   - Setup timer and callbacks
   *   - Any work to move neuron parameters into the correct locations.
   */

  // Setup timer tick
  // **TODO** base on defined machine time step
  spin1_set_timer_tick( 1000 );

  // Setup callbacks, etc.
  spin1_callback_on( MC_PACKET_RECEIVED, incoming_spike_callback, -1 );
  spin1_callback_on( TIMER_TICK, timer_callback, 2 );

  // Go
  spin1_start( );
}

//! Initialise the accumulators for incoming dimension packets
void initialise_incoming_value_accums( void )
{
  // TODO
}

//! Initialise the buffers for encoded values
void initialise_incoming_value_buffers( void )
{
  // TODO
}

//! Initialise the buffers for outgoing decoded values
void initialise_outgoing_value_accums( void )
{
  // TODO
}
