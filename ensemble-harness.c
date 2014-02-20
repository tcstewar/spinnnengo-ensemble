#include "spin-nengo-ensemble.h"

uint n_input_dimensions, n_output_dimensions, n_outputs, n_neurons, dt, t_ref,
     *v_ref_voltage, *output_aliases;
current_t *i_bias;
accum *encoders, *decoders;
value_t *ibuf_accumulator, *ibuf_filtered, *output_values, t_rc, filter,
        *decoded_values;

void test_initialise( void )
{
  // For some testing, we'll stick some sensible values in...
  dt = 1000;
  filter = 0.990049833749k;
  n_input_dimensions = 1;
  n_neurons = 1;
}

void test_initialise_2( void )
{
  i_bias[0] = 0.5k;
  encoders[0] = 0.5k;
  v_ref_voltage[0] = 0x00000000;
  t_rc = 0.02k;
  t_ref = 5;
}

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

  // Setup callbacks, etc.
  spin1_callback_on( MC_PACKET_RECEIVED, incoming_spike_callback, -1 );
  spin1_callback_on( TIMER_TICK, timer_callback, 2 );

  // Setup buffers, etc.
  test_initialise( );
  initialise_buffers( );
  test_initialise_2( );

  // Setup timer tick, start
  spin1_set_timer_tick( dt );
  spin1_start( );
}

//! Initialise buffers and values
void initialise_buffers( void )
{
  // Encoders / decoders / bias
  encoders = spin1_malloc( sizeof(accum) * n_input_dimensions * n_neurons );
  decoders = spin1_malloc( sizeof(accum) * n_output_dimensions * n_neurons );
  output_aliases = spin1_malloc( sizeof( uint ) * n_output_dimensions );
  i_bias = spin1_malloc( sizeof(current_t) * n_neurons );

  // Input buffers / voltages
  ibuf_accumulator = spin1_malloc( sizeof(value_t) * n_input_dimensions );
  ibuf_filtered = spin1_malloc( sizeof(value_t) * n_input_dimensions );
  v_ref_voltage = spin1_malloc( sizeof(uint) * n_neurons );

  // Output buffers
  decoded_values = spin1_malloc( sizeof(value_t) * n_output_dimensions );
}
