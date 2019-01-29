/**
 *  Source file for holding inforamtion about scanning network range.
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */


#include "IsamonNetAddr.h"

/*
     * Constructor of NetAddr class.
     * This function hold information about scanning range
     *
     * params:
     *      netIp = IPv4 address of network
     *      netMask = IPv4 mask of the network
     */
IsamonNetAddr::IsamonNetAddr(uint32_t netIp, uint32_t netMask) {
    localNetMask.setIp(netMask);
    localNetAddr.setIp(netIp);
}

//Function for check if the network address and mask is correct
bool IsamonNetAddr::isCorrect() {
    uint32_t tmpNetAddr = localNetAddr.getIp() & localNetMask.getIp();

    return (tmpNetAddr == localNetAddr.getIp());
}

//function for returning net IP
string IsamonNetAddr::getStrNetIp() {
    return localNetAddr.getStringIp();
}

//function for returning net Mask
string IsamonNetAddr::getStrNetMask() {
    return localNetMask.getStringIp();
}

//return first IP form this range
IsamonIP IsamonNetAddr::getFirstIp() {
    return localNetAddr;
}

//function for determine, that insert ip is from this range
bool IsamonNetAddr::isIncludedIp(IsamonIP ip) {
    return (localNetAddr.getIp() == (ip.getIp()&localNetMask.getIp()));
}
