/**
 *  Source file for function, which can send UDP packet to specific ip address
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include "IsamonUDP.h"
#include "IsamonErrorNo.h"

using namespace std;

/*
 * Function for send UDP packet, to specific port
 *
 * params: host - specific host, which will be used for sending
 *         interfc - specific interface, which will be used for sending packet
 *         port - which port will be scanned
 */
void sendUDP_to_port(IsamonIP host, IsamonInterfc interfc, uint16_t port);

/*
 * Function for send UDP packet
 *
 * params: host - specific host, which will be used for sending
 *         interfc - specific interface, which will be used for sending packet
 *         port - which port will be scanned -1 -> all
 */
void UDP_send(IsamonIP host, IsamonInterfc interfc, int port) {
    if(port < 0) {      //Send to all ports
        for(int i = 1; i <= 0XFFFF ; i++) {
            sendUDP_to_port(host, interfc, (uint16_t)i);

            if(i == 65530)
                this_thread::sleep_for(chrono::milliseconds(10));
        }
    } else {
        sendUDP_to_port(host, interfc, (uint16_t) port);
    }
}


void sendUDP_to_port(IsamonIP host, IsamonInterfc interfc, uint16_t port) {
    int sockfd;
    struct sockaddr_in server_addr{};

    //open socket udp
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0) {
        perror("Create socket (UDP)");
        exit(UDPERR);
    }

    //bind socket to specific interface
    if(setsockopt( sockfd, SOL_SOCKET, SO_BINDTODEVICE, interfc.getName().c_str(), (int)interfc.getName().size()) != 0)
    {
        cerr << "ERROR: setsockopt! Can't bind socket to specific interface!";
        exit(UDPERR);
    }

    //fill information about sending
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    inet_pton(AF_INET, host.getStringIp().c_str(), &server_addr.sin_addr);

    //send packets
    if((sendto(sockfd, NULL, 0, 0, (sockaddr*)&server_addr, sizeof(server_addr))) < 0) {
        perror("Sendto (UDP)");
        exit(UDPERR);
    }

    //close socket
    close(sockfd);
}
