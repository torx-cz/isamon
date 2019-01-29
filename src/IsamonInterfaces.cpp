/**
 *  Source file, which holds information about all interfaces, which are used for scanning
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */

#include <iostream>
#include <cstring>
#include <ifaddrs.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "IsamonErrorNo.h"
#include "IsamonInterfaces.h"

/*
 * Constructor of IsamonInterfaces
 * This function do nothing
 */
IsamonInterfaces::IsamonInterfaces() = default;

/*
 * Destructor of IsamonInterfaces
 *
 * This functionis only for clean vector  of interfaces
 */
IsamonInterfaces::~IsamonInterfaces() {
    vectInterfaces.clear();
}

/*
 * Function for set only one interface for scanning
 *
 * params: name - specific name from this interface
 *
 * return value: TRUE - if the interface with a specific name is found
 *               FALSE - if there is none interface with this name
 */
bool IsamonInterfaces::setInterface(string name) {
    struct ifaddrs *ifaddr, *ifa;
    struct sockaddr_in *sa;



    //load interfaces info
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(INTERFACERR);
    }

    /* Walk through linked list, maintaining head pointer so we
     can free list later */
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        char strIP[INET_ADDRSTRLEN] = {0};
        char strMASK[INET_ADDRSTRLEN] = {0};

        //we are looking for the interface with the specific name:
        if (strcmp(ifa->ifa_name, name.c_str()) != 0)
            continue;

        //add interface only if vector is empty
        if (vectInterfaces.empty()) {
            index = 0;
            vectInterfaces.emplace_back(ifa->ifa_name, IsamonIP(), IsamonIP());
        }

        //If ther is no information about IPv4 or MAC address, so keep going
        if (ifa->ifa_addr == NULL)
            continue;

        //If ther is no information about IPv4, so keep going
        if (ifa->ifa_addr->sa_family != AF_INET)
            continue;


        //GET IP
        sa = (struct sockaddr_in *) ifa->ifa_addr;
        inet_ntop(AF_INET, &(sa->sin_addr), strIP, INET_ADDRSTRLEN);

        //GET MASK
        sa = (struct sockaddr_in * ) ifa->ifa_netmask;
        inet_ntop(AF_INET, &(sa->sin_addr), strMASK, INET_ADDRSTRLEN);

        //chnage interface info!
        vectInterfaces.clear();
        vectInterfaces.emplace_back(ifa->ifa_name,IsamonIP(strIP),IsamonIP(strMASK));
    }

    //set hardware address to interface, if was founded
    setMACs();

    //free alocated memory
    freeifaddrs(ifaddr);
    return (!vectInterfaces.empty()); //if ther is some interface it is TRUE, otherwise FALSE
}

/*
 * Function for get any interfaces with IPv4 for scanning
 *
 * return: TRUE only
 */
bool IsamonInterfaces::setAllInterfaces() {

    struct ifaddrs *ifaddr, *ifa;
    struct sockaddr_in *sa;



    //load interfaces info
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return false;
    }

    /* Walk through linked list, maintaining head pointer so we
     can free list later */

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        char strIP[INET_ADDRSTRLEN] = {0};
        char strMASK[INET_ADDRSTRLEN] = {0};

        //We need interfaces with IP addres, so if it is not contain IP, I don't need this interface
        if (ifa->ifa_addr == NULL)
            continue;

        if (ifa->ifa_addr->sa_family != AF_INET)
            continue;

        //GET IP
        sa = (struct sockaddr_in *) ifa->ifa_addr;
        inet_ntop(AF_INET, &(sa->sin_addr), strIP, INET_ADDRSTRLEN);

        //GET MASK
        sa = (struct sockaddr_in * ) ifa->ifa_netmask;
        inet_ntop(AF_INET, &(sa->sin_addr), strMASK, INET_ADDRSTRLEN);

        //Save this interface into vector of interfaces
        vectInterfaces.emplace_back(ifa->ifa_name,IsamonIP(strIP),IsamonIP(strMASK));
        index = 0;
    }

    //set hardware address to interface, if was founded
    setMACs();

    //free alocated memory
    freeifaddrs(ifaddr);
    return true;
}

/*
 * Function for get next interface
 *
 * return: IsamonInterfc, which is valid or non valid
 */
IsamonInterfc IsamonInterfaces::getNextInterface() {
    //if the vector of interfaces is empty return non valid interface
    if (index < 0)
        return IsamonInterfc("null",IsamonIP(),IsamonIP());

    //if there is no more interfaces in the vector, return non valid interface
    if ((unsigned) index >= vectInterfaces.size())
        return IsamonInterfc("null",IsamonIP(),IsamonIP());

    //otherwise return valid interface
    IsamonInterfc ret = vectInterfaces.at((unsigned)index);
    index++;
    return ret;
}


/*
 * Function for get and add hardware address to specific interface
 */
void IsamonInterfaces::setMACs() {

    //create socket for get information about interface
    int32_t sd = socket( PF_INET, SOCK_DGRAM, 0 );
    if (sd < 0) {
        perror("socket");
        return;
    }

    //add hardware address to each interface from my vector of interfaces
    for(unsigned i = 0; i < vectInterfaces.size(); i++) {

        //get interface name
        string name = vectInterfaces.at(i).getName();

        struct ifreq req{};     //struct for holds information about interface

        //get this interface information
        strcpy( req.ifr_name, name.c_str());
        if( ioctl( sd, SIOCGIFHWADDR, &req ) != -1 ) {

            auto mac = (uint8_t *) req.ifr_ifru.ifru_hwaddr.sa_data;

            //set MAC address on specific interface
            vectInterfaces.at(i).setMAC(mac);
        }

        //get index from specific interface and store it
        if (ioctl(sd, SIOCGIFINDEX, &req) == -1) {
            perror("Interface: SIOCGIFINDEX");
            exit(EXIT_FAILURE);
        }
        else {
            //store the interface index
            vectInterfaces.at(i).setIndex(req.ifr_ifindex);
        }
    }
}

/*
 * Function for get first interface from the array of interfaces
 *
 * return: IsamonInterfc, which is valid or non valid
 */
IsamonInterfc IsamonInterfaces::getFirstInterface() {
    //if the vector of interfaces is empty return non valid interface
    if (index < 0)
        return IsamonInterfc("null",IsamonIP(),IsamonIP());

    //set index to first position of interface
    index = 0;

    //otherwise return valid interface
    IsamonInterfc ret = vectInterfaces.at((unsigned)index);
    index++;
    return ret;
}

/*
 * Function for find interface with a specific IPv4 address set on this interface
 *
 * return: IsamonInterfc, which is valid or non valid
 */
IsamonInterfc IsamonInterfaces::findInterface(IsamonIP ip) {
    //If ther isonly one interface use them
    if(vectInterfaces.size() == 1)
        return vectInterfaces.at(0);

    //otherwise find the coressponding interface
    for (auto &i : vectInterfaces) {
        if (i.isMe(ip)) {
            return i;
        }
    }

    //if there is no interface with this IPv4 address return non valid interface
    return IsamonInterfc("null",IsamonIP(),IsamonIP());
}
