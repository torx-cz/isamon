/**
 *  Header file, which holds information about all interfaces, which are used for scanning
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */

#ifndef ISA_ISAMONINTERFACES_H
#define ISA_ISAMONINTERFACES_H

#include <vector>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <linux/if_link.h>


#include "IsamonInterfc.h"


using namespace std;

class IsamonInterfaces {
private:
    vector<IsamonInterfc> vectInterfaces;   //vector of interfaces
    int index = -1;                         //number of interfaces

    /*
     * Function for get and add hardware address to specific interface
     */
    void setMACs();

public:
    /*
     * Constructor of IsamonInterfaces
     * This function do nothing
     */
    IsamonInterfaces();

    /*
     * Function for set only one interface for scanning
     *
     * params: name - specific name from this interface
     *
     * return value: TRUE - if the interface with a specific name is found
     *               FALSE - if there is none interface with this name
     */
    bool setInterface(string name);

    /*
     * Function for get any interfaces with IPv4 for scanning
     *
     * return: TRUE only
     */
    bool setAllInterfaces();

    /*
     * Function for get next interface
     *
     * return: IsamonInterfc, which is valid or non valid
     */
    IsamonInterfc getNextInterface();

    /*
     * Function for get first interface from the array of interfaces
     *
     * return: IsamonInterfc, which is valid or non valid
     */
    IsamonInterfc getFirstInterface();

    /*
     * Function for find interface with a specific IPv4 address set on this interface
     *
     * return: IsamonInterfc, which is valid or non valid
     */
    IsamonInterfc findInterface(IsamonIP ip);

    /*
     * Destructor of IsamonInterfaces
     *
     * This functionis only for clean vector  of interfaces
     */
    virtual ~IsamonInterfaces();
};


#endif //ISA_ISAMONINTERFACES_H
