/**
 *  Header file of class, which capture and process the packets
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */

#ifndef ISA_ISAMONPCAP_H
#define ISA_ISAMONPCAP_H


#include <ios>
#include <pcap.h>

#include "IsamonLiveVect.h"
#include "IsamonErrorNo.h"
#include "IsamonNetAddr.h"
#include <thread>

using namespace std;

class IsamonPcap {
private:
    pcap_t * handle;            //pcap handle
    struct bpf_program fp{};    // the compiled filter
    string dev;                 //always "any"

    thread scanThread;          //separeted thread for scanning


public:
    /*
     * Init PCAP filter
     *
     * params:  filter  = what will be filtered (filter expression)
     */
    explicit IsamonPcap(string filter);

    /*
     * Function for capture packets and processed that.
     *
     * params:  results = vector of hosts, which answered (returned value)
     *          range   = which Ip can be add to results
     */
    void getPackets(IsamonLiveVect &results, IsamonNetAddr &range);


    /*
     * Destruct Pcap class
     */
    virtual ~IsamonPcap();

    /*
     * Function for set timer
     *
     * params:  wait    = time of waiting for packets in milliseconds
     */
    void setTimer(unsigned int wait);

    /*
     * Function for waiting until pcap_breakloop
     */
    void waitTime();
};


#endif //ISA_ISAMONPCAP_H
