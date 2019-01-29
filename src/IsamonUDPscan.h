/**
 *  Header file for function, which scann udp all live host
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */

#ifndef ISA_ISAMONUDPSCAN_H
#define ISA_ISAMONUDPSCAN_H


#include "IsamonNetAddr.h"
#include "IsamonLiveVect.h"

/*
 * Function, which provides the techniques for searching open udp ports on active devices
 *
 * params: range - the range of scnanning net
 *         wait - how many milliseconds pcap can wait after the last packet
 *         liveVect - is vector of host, which is live (or up)
 *         port - port number which will be scanned -> -1 is all
 */
void udpScan(IsamonNetAddr range, unsigned int wait, IsamonLiveVect &liveVect, int port);


#endif //ISA_ISAMONUDPSCAN_H
