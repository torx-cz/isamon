/**
 *  Header file of class, which holds information about hardware address (aka MAC)
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */

#ifndef ISA_ISAMONMAC_H
#define ISA_ISAMONMAC_H


#include <cstdint>
#include <ios>

using namespace std;


class IsamonMAC {
private:
    uint8_t mac[6] = {0}; //array, which contains the hardware address


public:
    /*
     * Different constructor of IsamonMAC
     */
    explicit IsamonMAC(uint8_t * macAddr);
    IsamonMAC();

    /*
     * Function for set MAC address
     */
    void setMAC(uint8_t * macAddr);

    /*
     * Function for get MAC address like a string
     */
    string getMAC();

    /*
     * Function for get first pointer to MAC address
     */
    uint8_t * getHwMAC();

    /*
     * Function for checking if the MAC is zero
     */
    bool isZero();
};


#endif //ISA_ISAMONMAC_H
