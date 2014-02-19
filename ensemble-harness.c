#include "spin-nengo-ensemble.h"

// Parameters
uint n_input_dimensions;  //! Number of input dimensions D_{in}
uint n_output_dimensions; //! Number of output dimensions D_{out}
uint n_neurons;           //! Number of neurons N
value_t t_rc;             //! Membrane time constant
value_t t_ref;            //! Refactory period 

current_t * i_bias;       //! Population biases : 1 x N

accum * encoders; //! Encoder values : N x D_{in} (including gains)
accum * decoders; //! Decoder values : N x SUM( d in D_{outs} )

// Buffers
value_t * ibuf_accumulator; //! Input buffers : 1 x D_{in}
value_t * ibuf_filtered;    //! Filtered input buffers : 1 x D_{in}
voltage_t * v_voltages;     //! Neuron voltages

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

  // Setup buffers, etc.
  initialise_buffers( );

  // Go
  spin1_start( );
}

//! Initialise buffers and values
void initialise_buffers( void )
{
  // Encoders / decoders / bias
  encoders = spin1_malloc( sizeof(accum) * n_input_dimensions * n_neurons );
  decoders = spin1_malloc( sizeof(accum) * n_output_dimensions * n_neurons );
  i_bias = spin1_malloc( sizeof(current_t) * n_neurons );

  // Input buffers / voltages
  ibuf_accumulator = spin1_malloc( sizeof(value_t) * n_input_dimensions );
  ibuf_filtered = spin1_malloc( sizeof(value_t) * n_input_dimensions );
  v_voltages = spin1_malloc( sizeof(voltage_t) * n_neurons );
}
