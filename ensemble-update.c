#include "spin-nengo-ensemble.h"

void timer_callback( uint arg0, uint arg1 )
{
  /*
   * - Decay input values (L.P.F) - switch off packet receipt interrupt here?
   * - Perform neural updates, interleave with decoding and transmitting
   *   dimension data.
   */
  // Values used below
  current_t i_membrane;
  voltage_t v_delta, v_voltage;
  
  // For every input dimension, decay the input value and zero the accumulator.
  for( uint d = 0; d < n_input_dimensions; d++ ) {
    /* START CRITICAL SECTION */
    ibuf_filtered[d] = ibuf_filtered[d] * filter
                     + ibuf_accumulator[d] * (1 - filter);
    ibuf_accumulator[d] = 0;
    /* END CRITICAL SECTION */
  }

  // Perform neuron updates, interspersed with decoding and transmitting
  for( uint n = 0; n < n_neurons; n++ ) {
    // If this neuron is refractory then skip any further processing
    if( neuron_refractory( n ) != 0 ) {
      decrement_neuron_refractory( n );
      continue;
    }

    // Include neuron bias
    i_membrane = i_bias[n];

    // Encode the input and add to the membrane current
    for( uchar d = 0; d < n_input_dimensions; d++ ) {
      i_membrane += neuron_encoder(n, d) * ibuf_filtered[d];
    }

    v_voltage = neuron_voltage(n);
    v_delta = dt * ( i_membrane - v_voltage ) / t_rc / 1000000;
    /* io_printf( IO_STD, "dt = %k, J = %k, V = %k, dV = %k\n",
                  dt, i_membrane, v_voltage, v_delta );
    */
    v_voltage += v_delta;

    // Voltages can't go below 0.0
    if( v_voltage < 0.0k ) {
      v_voltage = 0.0k;
    }

    // Save state
    set_neuron_voltage( n, v_voltage );

    // If this neuron has fired then process
    if( v_voltage > 1.0k ) {
      // Zero the voltage, set the refractory time
      set_neuron_refractory( n );
      io_printf( IO_STD, "%d SPIKED.  %d ticks till active. v_ = 0x%08x\n",
                 n, neuron_refractory( n ), v_ref_voltage[n] );

      // Update the output values
      for( uint d = 0; d < n_output_dimensions; d++ ) {
        output_values[d] += neuron_decoder( n, d );
      }
    }
  }
}
