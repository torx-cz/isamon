/**
 *  Header file, which contains information about the active host
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */

#ifndef ISA_ISAMONHOST_H
#define ISA_ISAMONHOST_H


#include <vector>
#include "IsamonIP.h"
#include "IsamonInterfc.h"

class IsamonHost {
private:
    IsamonInterfc interface;    //It holds information, where was the host founded
    IsamonIP ip;                //information aboout host IP
    vector<uint16_t> udp;       //vector, which holds list of UDP close ports on this host
    vector<uint16_t> tcp;       //vector, which holds list of TCP open ports on this host

    int UdpPortRange = 0;       //which UDP port was scanned
                                // value = -1 -> all ports
                                // value =  0 -> none
                                // value >  0 -> only ths one;

public:
    /*
     * Constructor of IsamonHost
     * This function create host
     *
     * params:
     *      interfc: where was founded the answer from the host
     *      ip: the IP address of the active host
     */
    IsamonHost(IsamonInterfc interface, IsamonIP ip);

    /*
     * Function for add open tcp port to this host
     *
     * params:
     *      port: which port is opened
     */
    void addTcpPort(uint16_t port);

    /*
     * Function for add close udp port to this host
     *
     * params:
     *      port: which port is opened
     */
    void addUdpPort(uint16_t port);

    /*
     * Function for set scanning udp port
     *
     * params:
     *      port: which port is scanned
     */
    void setUdpRange(int port);

    //return the host ip
    IsamonIP getIP();

    //return the host interface
    IsamonInterfc getInterfc();

    /*
     * Function for check is the host is valid
     */
    bool isValid();

    /*
     * Function for printing host, which is live
     */
    void printHost();
};


#endif //ISA_ISAMONHOST_H
