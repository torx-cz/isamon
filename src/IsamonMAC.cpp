/**
 *  Source file of class, which holds information about hardware address (aka MAC)
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */

#include <cstring>
#include <iostream>
#include "IsamonMAC.h"

/*
 * Different constructor of IsamonMAC
 */
IsamonMAC::IsamonMAC() = default;

/*
 * Different constructor of IsamonMAC
 */
IsamonMAC::IsamonMAC(uint8_t *macAddr) {
    setMAC(macAddr);
}

/*
 * Function for set MAC address
 */
void IsamonMAC::setMAC(uint8_t *macAddr) {
    memcpy(mac, macAddr, sizeof mac);
}

/*
 * Function for get MAC address like a string
 */
string IsamonMAC::getMAC() {
    char buf[20] = {0};
    sprintf(buf ,"%02x:%02x:%02x:%02x:%02x:%02x",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]) ;



    string ret = buf;
    //cout << "MAC: " << ret << endl;
    return ret;
}

/*
 * Function for get first pointer to MAC address
 */
uint8_t *IsamonMAC::getHwMAC() {
    return mac;
}

/*
 * Function for checking if the MAC is zero
 */
bool IsamonMAC::isZero() {
    for (int i = 0; i < 6; ++i) {
        if(mac[i] != 0)
            return false;
    }

    return true;
}
