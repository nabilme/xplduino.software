/*
 * Arduino ENC28J60 Ethernet shield UDP broadcast client
 */

 
#ifndef ETHERSHIELD_BROADCAST_H
#define ETHERSHIELD_BROADCAST_H
 
//~ #include "xpl.h"
#include "EtherShield.h"

void setup_udp();

void loop_udp();

// Broadcast the data
//~ void broadcastData( void );

void SendBroadcastData(uint8_t *buf, uint8_t lenght, uint16_t dest_port);

#endif
