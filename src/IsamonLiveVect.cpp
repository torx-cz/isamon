/**
 *  Source file of class, which holds information about the active hosts
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */

#include "IsamonLiveVect.h"

#include <iostream>

IsamonLiveVect::IsamonLiveVect(IsamonInterfaces &interfaces) : interfaces(interfaces) {

}

/*
 * Function for adding live host (it is mean that host is up) into the vector of up hosts
 *
 * params:
 *          interface   = it is interface where was host found. (for example: eth0)
 *          ip          = it is ipv4 address of found host
 */
void IsamonLiveVect::addMember(IsamonIP destIp, IsamonIP ip) {

    int position = 0;

    //If host has already added, so don't do that again (not duplicate)
    if (!findHost(ip, position)) {

        //find interface, where was packet captured
        IsamonInterfc interface = interfaces.findInterface(destIp);
        if(!interface.isValidInterface())
            return;

        //make new host
        IsamonHost newHost(interface, ip);

        //insert new host into the position
        //if the position is more than the last member in vector, push back
        if((unsigned)position < vectOfUp.size()) {
            vectOfUp.insert(vectOfUp.begin() + position, newHost);
        } else {
            vectOfUp.push_back(newHost);
        }
    }
}


/*
 * Function for find th host in vector of host.
 * It is implemented by binary search.
 *
 * params:
 *          ip       = it is tag by we are looking for a existing host
 *          position = is i found position (if return is true)
 *                     or is it position to insert the new one (if return value is false)
 */
bool IsamonLiveVect::findHost(IsamonIP ip, int &position) {

    //if vector is empty, so retur false and pos == 0
    if (vectOfUp.empty()) {
        position = 0;
        return false;
    }

    uint32_t testIp = ip.getIp();     //ipNet is ip in net order

    //here is a code for non empty vector
    int min = 0;
    auto max = (int) vectOfUp.size() - 1;
    int act = 0;

    //binary search
    while(min <= max) {
        act = (min + max)/2;
        uint32_t vectIp = vectOfUp.at((unsigned)act).getIP().getIp();

        if (vectIp == testIp) {

            position = act;
            return true;

        } else if (vectIp > testIp) {

            max = act - 1;

        } else if (vectIp < testIp) {

            min = act + 1;

        }
    }

    if (max < act) {
        position = act;
    }

    if (min > act) {
        position = min;
    }

    return false;
}


/*
 * Function for print each host, which is up
 */
void IsamonLiveVect::printVect() {
    for (auto &i : vectOfUp) {
        i.printHost();
    }
    //debug
    //cout << "Number of hosts, which is UP, is: " << vectOfUp.size() << endl;
}

/*
 * Function for adding tcp open port into the existing host
 *
 * params: ip - which host
 *         port - which port is open
 */
void IsamonLiveVect::addTCPort(IsamonIP ip, uint16_t port) {
    int pos = 0;

    //find host and after that add tcp port
    if (findHost(ip, pos)) {
        vectOfUp.at((unsigned)pos).addTcpPort(port);
    }
}

/*
 * Function for the getting next host, which is in the vector
 */
IsamonHost IsamonLiveVect::getNextHost() {
    //if index is more than size, there is no more valid interface
    if(index < vectOfUp.size()) {
        index++;
        return vectOfUp.at(index-1);
    } else {
        //return non valid interface
        IsamonInterfc nonValidInterfc("null",IsamonIP(),IsamonIP());
        IsamonHost nonValid(nonValidInterfc, IsamonIP());
        return nonValid;
    }
}

/*
 * Function for the getting the first host, which is up
 */
IsamonHost IsamonLiveVect::getFirstHost() {

    //just set the index
    index = 0;

    return getNextHost();
}


/*
 * Function for adding udp close port into the existing host
 *
 * params: ip - which host
 *         port - which port is close
 */
void IsamonLiveVect::addUDPort(IsamonIP ip, uint16_t port) {
    int pos = 0;

    //find host and after that add udp port
    if (findHost(ip, pos)) {
        vectOfUp.at((unsigned)pos).addUdpPort(port);
    }
}

/*
 * Function for set range of udp scanning ports
 *
 * params: ip - which host
 *         port - if it is -1 -> all ports will be scanned
 *                          0 -> none port
 *                        > 0 -> selected port will be scanned
 */
void IsamonLiveVect::setUDPRange(IsamonIP ip, int port) {
    int pos = 0;

    if (findHost(ip, pos)) {
        vectOfUp.at((unsigned)pos).setUdpRange(port);
    }
}
