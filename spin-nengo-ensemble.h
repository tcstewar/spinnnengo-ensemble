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

// Parameters
extern uint n_input_dimensions;  //! Number of input dimensions D_{in}
extern uint n_output_dimensions; //! Number of output dimensions D_{out}
extern uint n_neurons;           //! Number of neurons N
extern uint t_ref;               //! Refactory period -1    [steps]
extern value_t t_rc;             //! Membrane time constant [seconds]
extern value_t filter;           //! Input decay factor     [?????]
extern value_t dt;               //! Machine time step      [seconds]

extern current_t * i_bias;       //! Population biases : 1 x N

extern accum * encoders; //! Encoder values : N x D_{in} (including gains)
extern accum * decoders; //! Decoder values : N x SUM( d in D_{outs} )

// Buffers
extern value_t * ibuf_accumulator; //! Input buffers : 1 x D_{in}
extern value_t * ibuf_filtered;    //! Filtered input buffers : 1 x D_{in}
extern uint * v_ref_voltage;       //! 4b refractory state, remainder voltages [?????]

// Static inline access functions
static inline accum neuron_encoder( uint n, uint d ) //! Encoder value for neuron, decoder
  { return encoders[ n * n_input_dimensions + d ]; };

static inline voltage_t neuron_voltage( uint n ) //! Neuron voltage
  { return kbits( v_ref_voltage[n] & 0x0fffffff ); };

static inline void set_neuron_voltage( uint n, voltage_t v )
  { v_ref_voltage[n] = (
      ( v_ref_voltage[n] & 0xf0000000 )
    | ( bitsk( v ) & 0x0fffffff ) );
  };

static inline uint neuron_refractory( uint n ) //! Refractory time
  { return ( v_ref_voltage[n] & 0xf0000000 ) >> 28; };

static inline void set_neuron_refractory( uint n ) //! Make a neuron refractory
  { v_ref_voltage[n] |= t_ref << 28; };

static inline void decrement_neuron_refractory( uint n ) //! Decrement refractory time
  { v_ref_voltage[n] -= 0x10000000; };
