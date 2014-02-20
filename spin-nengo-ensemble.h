/*
 * SpiNNaker Nengo Ensemble Implementation
 * ---------------------------------------
 */

#include "spin1_api.h"
#include "stdfix-full-iso.h"

typedef accum value_t;
typedef accum current_t;
typedef accum voltage_t;

/* Main and callbacks ********************************************************/
int c_main( void );
void timer_callback( uint arg0, uint arg1 );
void incoming_spike_callback( uint key, uint payload );

/* Initialisation functions **************************************************/
void initialise_buffers( void );

/* Concatenated matrix utilities *********************************************/
typedef struct cm_index {
  //! For indexing elements in a concatenated matrix.
  uint start, length;
} cm_index_t;

/* Parameters ****************************************************************/
extern uint n_input_dimensions;      //! Number of input dimensions D_{in}
extern uint n_output_dimensions;     //! Number of output dimensions D_{out}
extern cm_index_t * output_decoders; //! Indexing for decoders
extern uint n_neurons;               //! Number of neurons N

extern uint dt;                      //! Machine time step      [useconds]
extern uint t_ref;                   //! Refractory period -1    [steps]
extern value_t t_rc;                 //! Membrane time constant [seconds]
extern value_t filter;               //! Input decay factor

extern current_t * i_bias;           //! Population biases : 1 x N

extern accum * encoders; //! Encoder values : N x D_{in} (including gains)
extern accum * decoders; //! Decoder values : N x SUM( d in D_{outs} )

/* Buffers *******************************************************************/
extern value_t * ibuf_accumulator; //! Input buffers : 1 x D_{in}
extern value_t * ibuf_filtered;    //! Filtered input buffers : 1 x D_{in}
extern uint * v_ref_voltage;       //! 4b refractory state, remainder voltages

/* Static inline access functions ********************************************/
// -- Encoder(s) and decoder(s)
//! Get the encoder value for the given neuron and dimension
static inline accum neuron_encoder( uint n, uint d )
  { return encoders[ n * n_input_dimensions + d ]; };

static inline accum neuron_decoder( uint n, uint block_start, uint d )
  { return decoders[ n * n_output_dimensions + block_start + d ]; };

// -- Voltages and refractory periods
//! Get the membrane voltage for the given neuron
static inline voltage_t neuron_voltage( uint n )
  { return kbits( v_ref_voltage[n] & 0x0fffffff ); };

//! Set the membrane voltage for the given neuron
static inline void set_neuron_voltage( uint n, voltage_t v )
  { v_ref_voltage[n] = (
      ( v_ref_voltage[n] & 0xf0000000 )
    | ( bitsk( v ) & 0x0fffffff ) );
  };

//! Get the refractory status of a given neuron
static inline uint neuron_refractory( uint n )
  { return ( v_ref_voltage[n] & 0xf0000000 ) >> 28; };

//! Put the given neuron in a refractory state
static inline void set_neuron_refractory( uint n )
  { v_ref_voltage[n] = t_ref << 28; };

//! Decrement the refractory time for the given neuron
static inline void decrement_neuron_refractory( uint n )
  { v_ref_voltage[n] -= 0x10000000; };
