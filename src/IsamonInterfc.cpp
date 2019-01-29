/**
 *  Source file, which holds information about one interface
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */

#include "IsamonInterfc.h"

#include <utility>
#include <iostream>

/*
 * Constructor of IsamonInterfc
 *
 * This function create interface like a name, and interface IP address and also  MASK
 *
 * params: name - name of interface
 *         ip   - ip address of the interface
 *         mask - ip mask
 */
IsamonInterfc::IsamonInterfc(string name, IsamonIP ip, IsamonIP mask) {
    intrfcName = std::move(name);
    intrfcIp = ip;
    intrfcMask = mask;
    intrfcNet = IsamonIP(mask.getIp() & ip.getIp());
}

/*
 * Function for return interface name
 */
string IsamonInterfc::getName() {
    return intrfcName;
}

/*
 * Function for return interface ip address
 */
string IsamonInterfc::getIp() {
    return intrfcIp.getStringIp();
}

/*
 * Function for return interface ip mask
 */
string IsamonInterfc::getMask() {
    return intrfcMask.getStringIp();
}

/*
 * Function for check is the post ip is local ip from this interface
 *
 * return: True if ip is local
 *         False if ip is not local
 */
bool IsamonInterfc::isLocalIP(IsamonIP ip) {
    return ((ip.getIp() & intrfcMask.getIp()) == intrfcNet.getIp());
}

//IP addres is reserved if is broadcast or net.addr
bool IsamonInterfc::isReservedIP(IsamonIP ip) {
    return ((ip.getIp() == intrfcNet.getIp()) || (ip.getIp() == (intrfcNet.getIp() | ~intrfcMask.getIp())));
}


/*
 * Function for check is the interface is correct
 *
 * return: True if is correct
 *         False if it is not correct
 */
bool IsamonInterfc::isValidInterface() {
    return ((intrfcName != "null") &&
            (intrfcIp.getIp() != 0) &&
            (intrfcMask.getIp() != 0));
}

/*
 * Function for check if this interface is loopback
 *
 * return: True if interface is loopback
 *         False if interface is not loopback
 */
bool IsamonInterfc::isLoopback() {
    return (intrfcMAC.isZero());
}

/*
 * Function for return interface hardware address, by string
 */
string IsamonInterfc::getMAC() {
    return intrfcMAC.getMAC();
}

/*
 * Function for set hardware address into this interface
 *
 * params: macAddr - is pointer to first BYTE of the MAC address
 */
void IsamonInterfc::setMAC(uint8_t *macAddr) {
    intrfcMAC.setMAC(macAddr);
}

/*
 * Function for get pointer to hardware address of this interface
 */
uint8_t *IsamonInterfc::getHwMAC() {
    return intrfcMAC.getHwMAC();
}

/*
 * Function for set interface index
 *
 * params: index - interface index
 */
void IsamonInterfc::setIndex(int index) {
    this->index = index;
}

/*
 * Function for get interface index
 */
int IsamonInterfc::getIndex() {
    return index;
}

/*
 * Function for check if the post address is my IPv4 address set of this interface
 *
 * params: ip, which has to be checked
 *
 * return: True if ip is me
 *         False if ip is not me
 */
bool IsamonInterfc::isMe(IsamonIP ip) {
    return (ip.getIp() == intrfcIp.getIp());
}
