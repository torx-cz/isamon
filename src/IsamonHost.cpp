/**
 *  Source file, which contains information about the active host
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */

#include "IsamonHost.h"

#include <utility>
#include <iostream>
#include <algorithm>

/*
 * Constructor of IsamonHost
 * This function create host
 *
 * params:
 *      interfc: where was founded the answer from the host
 *      ip: the IP address of the active host
 */
IsamonHost::IsamonHost(IsamonInterfc interface, IsamonIP ip) : interface(interface), ip(ip) {

}

//return the host ip
IsamonIP IsamonHost::getIP() {
    return ip;
}

/*
 * Function for add open tcp port to this host
 *
 * params:
 *      port: which port is opened
 */
void IsamonHost::addTcpPort(uint16_t port) {
    tcp.push_back(port);
}

/*
 * Function for check is the host is valid
 */
bool IsamonHost::isValid() {
    return interface.isValidInterface();
}

//return the host interface
IsamonInterfc IsamonHost::getInterfc() {
    return interface;
}


/*
 * Function for printing host, which is live
 */
void IsamonHost::printHost() {
    string ipStr = ip.getStringIp();

    //print the IP
    cout << ipStr << endl;

    //print tcp ports only, if it is not empty
    if (!tcp.empty()) {
        sort(tcp.begin(), tcp.end());
        tcp.erase(unique(tcp.begin(), tcp.end()), tcp.end());

        for (unsigned short i : tcp) {
            cout << ipStr << " TCP " << i << endl;
        }
    }

    //print udp ports, but only if it's set
    if(UdpPortRange != 0) {
        if(UdpPortRange < 0) {
            std::sort(udp.begin(), udp.end(), std::less<uint16_t >());
            udp.erase(unique(udp.begin(), udp.end()), udp.end());

            uint32_t index = 0;
            for(uint32_t port = 1; port <= 0xFFFF; port++) {

                if(index >= udp.size()) {
                    cout << ipStr << " UDP " << port << endl; //TODO
                } else {

                    if(port == udp.at(index)) {
                        index++;
                    } else {
                        cout << ipStr << " UDP " << port  << endl;
                    }
                }
            }
        } else {
            if(udp.empty()) {
                cout << ipStr << " UDP " << UdpPortRange << endl;
            } else {
                if (udp.at(0) != UdpPortRange) {
                    cout << ipStr << " UDP " << UdpPortRange  << udp.at(0) << endl;
                }
            }
        }

    }
}

/*
 * Function for add close udp port to this host
 *
 * params:
 *      port: which port is opened
 */
void IsamonHost::addUdpPort(uint16_t port) {
    udp.push_back(port);

}

/*
 * Function for set scanning udp port
 *
 * params:
 *      port: which port is scanned
 */
void IsamonHost::setUdpRange(int port) {
    UdpPortRange = port;
}
