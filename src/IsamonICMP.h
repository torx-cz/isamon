/**
 *  Header file for sending ICMP ECHO request to host via IPv4.
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */

#ifndef ISA_ISAMONICMP_H
#define ISA_ISAMONICMP_H

#include "IsamonIP.h"
#include "IsamonInterfc.h"

/*
 * Function for sending ICMP Echo request to the specific IPv4 address.
 *
 * params:
 *      host:       IP address of host
 *      interfc:    which interface has to be used for sending ICMP Echo request
 */
void ICMP_Send(IsamonIP host, IsamonInterfc interfc);

void ICMP_Stuck(IsamonInterfc interfc);

#endif //ISA_ISAMONICMP_H
