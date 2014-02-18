#include "spin-nengo-ensemble.h"

uint n_input_dimensions = 0;
value_t * ibuf_accumulator = 0,
        * ibuf_filtered = 0;

value_t * value_gains = 0;
current_t * i_bias;

accum * encoders;
accum * decoders;

value_t * t_rc;
value_t * t_ref;

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

  io_printf( IO_STD, "Starting.\n" );

  // Go
  spin1_start( );
}

//! Initialise buffers and values
void initialise_buffers( void )
{
  ibuf_accumulator = spin1_malloc( sizeof( value_t ) * n_input_dimensions );
  ibuf_filtered = spin1_malloc( sizeof( value_t ) * n_input_dimensions );
}
