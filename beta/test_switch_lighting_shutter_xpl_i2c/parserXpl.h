
/*
 * xPL message parser v0.1 by Gromain59 - Gromain59@gmail.com
 * This code is parsing a xPL message stored in 'received' buffer
 * - isolate and store in 'line' buffer each part of the message -> detection of EOL character (DEC 10)
 * - analyse 'line', function of its number and store information in xpl_header memory
 * - check for each step if the message respect xPL protocol
 * - parse each command line
 * - suppress source identification because no need and take to much memory
 */
 
 
// TODO: PREVOIR UN RESET DE xpl_header OU DE LE DETRUIRE A CHAQUE FIN DE PARSING
// remplacer COMMAND_MAX par Config.xpl_command_max


#ifndef parserXpl_h
#define parserXpl_h

#include <stdio.h>
//~ #include "xpl.h"

#include "config.h"	// specific project data

// Variables definition
// extern struct_xpl_header xpl_header;			// header xpl message variable
// extern struct_command command[];	// schema command

int parse(uint8_t *udpPayload, int len);

void analyse_schema(uint8_t *udpPayload, int16_t msg, int16_t first_cmnd);


#endif
