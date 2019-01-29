/**
 *  Header file for function, which can send UDP packet to specific ip address
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */

#ifndef ISA_ISAMONUDP_H
#define ISA_ISAMONUDP_H

#include "IsamonIP.h"
#include "IsamonInterfc.h"

/*
 * Function for send UDP packet
 *
 * params: host - specific host, which will be used for sending
 *         interfc - specific interface, which will be used for sending packet
 *         port - which port will be scanned -1 -> all
 */
void UDP_send(IsamonIP host, IsamonInterfc interfc, int port);


#endif //ISA_ISAMONUDP_H
