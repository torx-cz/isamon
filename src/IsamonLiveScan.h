/**
 *  Header file of function, which provide the techniques for looking for the active hosts in the net range
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */

#ifndef ISA_ISAMONLIVESCAN_H
#define ISA_ISAMONLIVESCAN_H

#include "IsamonNetAddr.h"
#include "IsamonInterfaces.h"
#include "IsamonLiveVect.h"

/*
 * Function, which provides the techniques for searching active devices in the scanNet
 *
 * params: scanNet - the range of scnanning net
 *         interfaces - vector of interfaces, which have to be used for scanningTech
 *         timeout - how many milliseconds pcap can wait after the last packet
 *         liveVect - is vector of host, which is live (or up)
 */
void liveScan(IsamonNetAddr scanNet, IsamonInterfaces interfaces, unsigned int timeout, IsamonLiveVect &liveVect);

#endif //ISA_ISAMONLIVESCAN_H
