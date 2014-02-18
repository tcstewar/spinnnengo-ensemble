#include "spin-nengo-ensemble.h"

void incoming_spike_callback( uint key, uint payload )
{
  /*
   * - Determine the dimension this packet relates to from the key
   * - Add the value of the payload (cast to accum) to the received value for
   *   that dimension.
   */
  io_printf( IO_STD, "Received packet: Payload: 0x%08x\n", payload );
}

/*
 * Possible TODO:
 * - On timer2 (if possible) transmit the decoded value for a dimension.
 */
