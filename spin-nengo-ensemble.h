/*
 * SpiNNaker Nengo Ensemble Implementation
 * ---------------------------------------
 */

#include "spin1_api.h"
#include "stdfix-full-iso.h"

typedef accum value_t;
typedef accum current_t;
typedef accum voltage_t;

// Main and callbacks
int c_main( void );
void timer_callback( uint arg0, uint arg1 );
void incoming_spike_callback( uint key, uint payload );

// Initialisation functions
void initialise_buffers( void );

// Buffers, variables
extern uint n_input_dimensions; //! Number of input dimensions D_{in}
extern uint n_neurons;          //! Number of neurons N

extern value_t * ibuf_accumulator; //! Input buffers : 1 x D_{in}
extern value_t * ibuf_filtered;    //! Filtered input buffers : 1 x D_{in}

extern value_t * value_gains; //! Population gains  : 1 x N
extern current_t * i_bias;    //! Population biases : 1 x N

extern accum * encoders; //! Encoder values : N x D_{in}
extern accum * decoders; //! Decoder values : N x SUM( d in D_{outs} )

// Universal parameters
extern value_t * t_rc;  //! Membrane time constant
extern value_t * t_ref; //! Refactory period 
