/**
 *  Source file for function, which scann tcp all live host
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */

#include <iostream>
#include "IsamonTCPscan.h"
#include "IsamonTCP.h"
#include "IsamonPcap.h"

/*
 * Function, which provides the techniques for searching open tcp ports on active devices
 *
 * params: range - the range of scnanning net
 *         wait - how many milliseconds pcap can wait after the last packet
 *         liveVect - is vector of host, which is live (or up)
 *         port - port number which will be scanned -> -1 is all
 */
void tcpScan(IsamonNetAddr range, unsigned int wait, IsamonLiveVect &liveVect, int port) {

    //get first active host for sanning
    IsamonHost actHost = liveVect.getFirstHost();

    //turn on pcap for capturing and processing tcp packets
    IsamonPcap pcap1("tcp");
    pcap1.getPackets(liveVect, range);

    //go throught the active hosts
    while(actHost.isValid()) {

        //send scanning packets for open ports
        TCP_send(actHost.getIP(), actHost.getInterfc(), port);

        //get next active host
        actHost = liveVect.getNextHost();

    }

    //set timer for pcap
    pcap1.setTimer(wait);
    pcap1.waitTime();
    TCP_stuck(liveVect.getFirstHost().getInterfc());
}
