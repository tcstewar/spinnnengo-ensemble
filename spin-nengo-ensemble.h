/*
 * SpiNNaker Nengo Ensemble Implementation
 * ---------------------------------------
 */

#include "spin1_api.h"
#include "stdfix-full-iso.h"

// Main and callbacks
int c_main( void );
void timer_callback( uint arg0, uint arg1 );
void incoming_spike_callback( uint key, uint payload );

// Initialisation functions
void initialise_incoming_value_accums( void );
void initialise_incoming_value_buffers( void );
void initialise_outgoing_value_accums( void );
