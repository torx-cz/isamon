/**
 *  Header file, which holds information about IPv4 address
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */

#ifndef ISA_ISAMONIP_H
#define ISA_ISAMONIP_H


#include <cstdint>
#include <ios>

using namespace std;

class IsamonIP {
protected:
    uint32_t localIp = 0;   //is holding ip address

public:
    /*
     * Different constructor of IsamonIP
     */
    explicit IsamonIP(uint32_t ip);
    explicit IsamonIP(char * ip);
    explicit IsamonIP(string ip);
    IsamonIP();

    /*
     * Function for get IP in a string form
     */
    string getStringIp();

    /*
     * Function for get IP in a source form
     */
    uint32_t getIp();

    /*
     * Function, which increment IP
     */
    void incIp();

    /*
     * Function for set IP by different params
     */
    bool setIp(char * ip);
    bool setIp(uint32_t ip);

};


#endif //ISA_ISAMONIP_H
