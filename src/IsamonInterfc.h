/**
 *  Header file, which holds information about one interface
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */

#ifndef ISA_ISAMONINTERFC_H
#define ISA_ISAMONINTERFC_H

#include <ios>
#include "IsamonIP.h"
#include "IsamonMAC.h"

using namespace std;

class IsamonInterfc {

private:
    string intrfcName;      //name of interface
    IsamonIP intrfcNet;     //Local Net address of this interfaces
    IsamonIP intrfcIp;      //IPv4 address of this interface
    IsamonIP intrfcMask;    //IPv4 mask of the local network
    IsamonMAC intrfcMAC;    //hardware address of this interface
    int index = 0;          //interface index


public:
    /*
     * Constructor of IsamonInterfc
     *
     * This function create interface like a name, and interface IP address and also  MASK
     *
     * params: name - name of interface
     *         ip   - ip address of the interface
     *         mask - ip mask
     */
    IsamonInterfc(string name, IsamonIP ip, IsamonIP mask);

    /*
     * Function for return interface name
     */
    string getName();

    /*
     * Function for return interface ip address
     */
    string getIp();

    /*
     * Function for return interface ip mask
     */
    string getMask();

    /*
     * Function for return interface hardware address, by string
     */
    string getMAC();

    /*
     * Function for check is the interface is correct
     *
     * return: True if is correct
     *         False if it is not correct
     */
    bool isValidInterface();

    /*
     * Function for check is the post ip is local ip from this interface
     *
     * return: True if ip is local
     *         False if ip is not local
     */
    bool isLocalIP(IsamonIP ip);

    //IP addres is reserved if is broadcast or net.addr
    bool isReservedIP(IsamonIP ip);

    /*
     * Function for check if this interface is loopback
     *
     * return: True if interface is loopback
     *         False if interface is not loopback
     */
    bool isLoopback();

    /*
     * Function for check if the post address is my IPv4 address set of this interface
     *
     * params: ip, which has to be checked
     *
     * return: True if ip is me
     *         False if ip is not me
     */
    bool isMe(IsamonIP ip);

    /*
     * Function for set hardware address into this interface
     *
     * params: macAddr - is pointer to first BYTE of the MAC address
     */
    void setMAC(uint8_t * macAddr);

    /*
     * Function for set interface index
     *
     * params: index - interface index
     */
    void setIndex(int index);

    /*
     * Function for get interface index
     */
    int getIndex();

    /*
     * Function for get pointer to hardware address of this interface
     */
    uint8_t * getHwMAC();

};


#endif //ISA_ISAMONINTERFC_H
