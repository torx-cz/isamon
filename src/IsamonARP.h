/**
 *  Header file for sending ARP requests to specific host via specific interface.
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */

#ifndef ISA_ISAMONARP_H
#define ISA_ISAMONARP_H

#include <netpacket/packet.h> //struct sockaddr_ll

#include "IsamonInterfc.h"


class IsamonARP {
private:
    int sockfd = 0;         //private variable for holdinh the socket handler
    struct sockaddr_ll device{};    //handler for holding the info about devices
    char ethFrame[128] = {0};      //pre-prepered ethernet frame for sending arp request

    /**
     * Function for generate arp packets
     *
     * params: host: destination host of asking
     *         interfc: which interface has to be use for sending request
     */
    void gen_arp_frame(IsamonIP host, IsamonInterfc interfc);

    /**
     * Function for change the destination host in pre-prepered ethernet frame
     *
     * params: host: destination host of asking
     */
    void set_host_ip(IsamonIP host);

public:
    /**
     * Constructor of ARP class.
     *
     * This function prepere everithing for future sending packets
     *
     * params: interfc: which interface has to be use for sending request
     */
    explicit IsamonARP(IsamonInterfc interfc);

    /**
     * This function send packet to specific IPv4 address
     *
     * params: host: destination host of asking
     */
    void send(IsamonIP ip);

    /**
     * Destructor is used for the closing socket hendler 
     */
    ~IsamonARP();
};


#endif //ISA_ISAMONARP_H
