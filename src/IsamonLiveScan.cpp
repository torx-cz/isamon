/**
 *  Source file of function, which provide the techniques for looking for the active hosts in the net range
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */

#include <iostream>
#include "IsamonNetAddr.h"
#include "IsamonInterfaces.h"
#include "IsamonICMP.h"
#include "IsamonARP.h"
#include "IsamonPcap.h"

using namespace std;

/*
 * Function, which provides the techniques for searching active
 * This function choose ARP or ICMP protocol for sending request
 *
 * params: interfc - which interface has to be use for scanning
 *         ip      - IPv4 of scanning hosts
 *         arp     - the arp frame ready
 */
void scanningTech(IsamonInterfc interfc, IsamonIP ip, IsamonARP &arp);

/*
 * Function, which provides the techniques for searching active devices in the scanNet
 *
 * params: scanNet - the range of scnanning net
 *         interfaces - vector of interfaces, which have to be used for scanningTech
 *         timeout - how many milliseconds pcap can wait after the last packet
 *         liveVect - is vector of host, which is live (or up)
 */
void liveScan(IsamonNetAddr scanNet, IsamonInterfaces interfaces, unsigned int timeout, IsamonLiveVect &liveVect) {

    //init pcap class for cepturing arp or icmp
    IsamonPcap pcap1("arp or icmp");
    pcap1.getPackets(liveVect,scanNet);

    //go throught the list of interfaces
    for (IsamonInterfc actInterface = interfaces.getFirstInterface();
         actInterface.isValidInterface();
         actInterface = interfaces.getNextInterface()) {
        //body of for cycle

        IsamonARP arp(actInterface);

        //send scanning request on all IPs from scanNet
        for(IsamonIP testIP = scanNet.getFirstIp();
            scanNet.isIncludedIp(testIP);
            testIP.incIp()) {

            scanningTech(actInterface, testIP, arp);
        }
    }

    pcap1.setTimer(timeout);
    //ICMP_Stuck(interfaces.getFirstInterface());

    pcap1.waitTime();
    ICMP_Stuck(interfaces.getFirstInterface());
}

/*
 * Function, which provides the techniques for searching active
 * This function choose ARP or ICMP protocol for sending request
 *
 * params: interfc - which interface has to be use for scanning
 *         ip      - IPv4 of scanning hosts
 *         arp     - the arp frame ready
 */
void scanningTech(IsamonInterfc interfc, IsamonIP ip, IsamonARP &arp) {

    /*
     * I cant send massage on reserved IP (address of network and broadcast addresss),
     * so I have to check if ip is not reserved IP
     * */
    if(interfc.isReservedIP(ip))
        return;

    /*
     * If the ip is local, I use the arp request otherwise I use the ICMP echo request
     *
     * Loopback only ICMP
     * */
    if(interfc.isLocalIP(ip) and !interfc.isLoopback() and !interfc.isMe(ip)) {


        // I can sedn ARP request
        arp.send(ip);
    }
    else {

        // I can send ICMP echo request
        ICMP_Send(ip, interfc);
    }
}
