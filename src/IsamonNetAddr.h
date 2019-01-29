/**
 *  Header file for holding inforamtion about scanning network range.
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */

#ifndef ISA_ISAMONNETADDR_H
#define ISA_ISAMONNETADDR_H


#include <cstdint>
#include "IsamonIP.h"

class IsamonNetAddr {
protected:
    IsamonIP localNetAddr;  //Address of network
    IsamonIP localNetMask;  //Network mask


public:
    /*
     * Constructor of NetAddr class.
     * This function hold information about scanning range
     *
     * params:
     *      netIp = IPv4 address of network
     *      netMask = IPv4 mask of the network
     */
    IsamonNetAddr(uint32_t netIp, uint32_t netMask);

    //Function for check if the network address and mask is correct
    bool isCorrect();

    //function for returning net IP
    string getStrNetIp();

    //function for returning net Mask
    string getStrNetMask();

    //return first IP form this range
    IsamonIP getFirstIp();

    //function for determine, that insert ip is from this range
    bool isIncludedIp(IsamonIP ip);
};


#endif //ISA_ISAMONNETADDR_H
