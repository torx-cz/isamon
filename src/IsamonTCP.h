/**
 *  Header file for function, which can send TCP packet to specific ip address
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */

#ifndef ISA_ISAMONTCP_H
#define ISA_ISAMONTCP_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include "IsamonIP.h"
#include "IsamonInterfc.h"

/*
 * Function for send TCP packet
 *
 * params: host - specific host, which will be used for sending
 *         interfc - specific interface, which will be used for sending packet
 *         port - which port will be scanned -1 -> all
 */
void TCP_send(IsamonIP host, IsamonInterfc interfc,  int port);

/*
 * Function for send TCP packet, which prevent to pcap stuck
 *
 * params:
 *         interfc - specific interface, which will be used for sending packet
 */
void TCP_stuck(IsamonInterfc interfc);

#endif //ISA_ISAMONTCP_H
