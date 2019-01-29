/**
 *  Header file of class, which holds information about the active hosts
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */


#ifndef ISA_ISAMONLIVEVECT_H
#define ISA_ISAMONLIVEVECT_H

#include <ios>
#include <vector>
#include "IsamonIP.h"
#include "IsamonHost.h"
#include "IsamonInterfaces.h"

using namespace std;

class IsamonLiveVect {
private:
    bool finish = false; //TODO
    IsamonInterfaces &interfaces;
    vector<IsamonHost> vectOfUp;
    unsigned int index = 0;

    /*
     * Function for find th host in vector of host.
     * It is implemented by binary search.
     *
     * params:
     *          ip       = it is tag by we are looking for a existing host
     *          position = is i found position (if return is true)
     *                     or is it position to insert the new one (if return value is false)
     */
    bool findHost(IsamonIP ip,int &position);

public:
    explicit IsamonLiveVect(IsamonInterfaces &interfaces);

    /*
     * Function for adding live host (it is mean that host is up) into the vector of up hosts
     *
     * params:
     *          interface   = it is interface where was host found. (for example: eth0)
     *          ip          = it is ipv4 address of found host
     */
    void addMember(IsamonIP destIp, IsamonIP ip);

    /*
     * Function for adding tcp open port into the existing host
     *
     * params: ip - which host
     *         port - which port is open
     */
    void addTCPort(IsamonIP ip, uint16_t port);

    /*
     * Function for adding udp close port into the existing host
     *
     * params: ip - which host
     *         port - which port is close
     */
    void addUDPort(IsamonIP ip, uint16_t port);

    /*
     * Function for set range of udp scanning ports
     *
     * params: ip - which host
     *         port - if it is -1 -> all ports will be scanned
     *                          0 -> none port
     *                        > 0 -> selected port will be scanned
     */
    void setUDPRange(IsamonIP ip, int port);

    /*
     * Function for the getting the first host, which is up
     */
    IsamonHost getFirstHost();

    /*
     * Function for the getting next host, which is in the vector
     */
    IsamonHost getNextHost();

    /*
     * Function for print each host, which is up
     */
    void printVect();

};


#endif //ISA_ISAMONLIVEVECT_H
