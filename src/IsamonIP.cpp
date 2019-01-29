/**
 *  Source file, which holds information about IPv4 address
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */

#include <iostream>
#include "IsamonIP.h"
/*
 * Different constructor of IsamonIP
 */
IsamonIP::IsamonIP(uint32_t ip) {
    setIp(ip);
}

/*
 * Different constructor of IsamonIP
 */
IsamonIP::IsamonIP(char * ip) {
    setIp(ip);
}

/*
 * Different constructor of IsamonIP
 */
IsamonIP::IsamonIP(string ip) {
    setIp((char *)ip.c_str());
}

/*
 * Different constructor of IsamonIP
 */
IsamonIP::IsamonIP() = default;

/*
 * Function for get IP in a source form
 */
uint32_t IsamonIP::getIp() {
    return localIp;
}

/*
 * Function for get IP in a string form
 */
string IsamonIP::getStringIp() {
    unsigned int byteArray[4] = {0};
    byteArray[0] = (localIp >> 24) & (0xFF);
    byteArray[1] = (localIp >> 16) & (0xFF);
    byteArray[2] = (localIp >>  8) & (0xFF);
    byteArray[3] = (localIp >>  0) & (0xFF);

    string tmpStringIP =   to_string(byteArray[0]) + "."
                           + to_string(byteArray[1]) + "."
                           + to_string(byteArray[2]) + "."
                           + to_string(byteArray[3]);
    return tmpStringIP;
}

/*
 * Function, which increment IP
 */
void IsamonIP::incIp() {
    localIp++;
}

/*
 * Function for set IP by different params
 */
bool IsamonIP::setIp(char *ip) {
    int dotsCounter = 0;

    //array of each byte
    unsigned array[4] = {0};

    //read the chars
    char * tmpChar;
    tmpChar = ip;
    while((*tmpChar != '\0') && (dotsCounter < 4)) {
        switch (*tmpChar) {
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '0':
                array[dotsCounter] = array[dotsCounter]*10 + ((*tmpChar) - '0');
                break;

            case '.':
                dotsCounter++;
                break;

            default:
                return false;
        }
        //next char
        tmpChar++;
    }

    //check if the IP contains 4 bytes
    if (dotsCounter != 3)
        return false;

        //check the size of each IP byte
    for (int i = 0; i < 4; ++i) {
        if (array[i] > 255)
            return false;
    }

    //set up the IP
    localIp = (array[0] << 24) |
              (array[1] << 16) |
              (array[2] <<  8) |
              (array[3]);

    return true;


}

/*
 * Function for set IP by different params
 */
bool IsamonIP::setIp(uint32_t ip) {
    localIp = ip;
    return true;
}
