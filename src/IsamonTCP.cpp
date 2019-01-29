/**
 *  Source file for function, which can send TCP packet to specific ip address
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */

#include <iostream>
#include <fcntl.h>
#include <thread>
#include "IsamonTCP.h"
#include "IsamonErrorNo.h"

using namespace std;

/*
 * Function for send TCP packet, to specific port
 *
 * params: host - specific host, which will be used for sending
 *         interfc - specific interface, which will be used for sending packet
 *         port - which port will be scanned
 */
void send_to_port(IsamonIP host, IsamonInterfc interfc, uint16_t port);


/*
 * Function for send TCP packet
 *
 * params: host - specific host, which will be used for sending
 *         interfc - specific interface, which will be used for sending packet
 *         port - which port will be scanned -1 -> all
 */
void TCP_send(IsamonIP host, IsamonInterfc interfc, int port) {

    if(port < 0) {      //Send to all ports
        for(int i = 0; i <= 0XFFFF ; i++) {
            send_to_port(host, interfc, (uint16_t)i);
        }
    } else {
        send_to_port(host, interfc, (uint16_t) port);
    }
}

/*
 * Function for send TCP packet, which prevent to pcap stuck
 *
 * params:
 *         interfc - specific interface, which will be used for sending packet
 */
void TCP_stuck(IsamonInterfc interfc) {
    IsamonIP ip(interfc.getIp());

    TCP_send(ip, interfc, 80);
}

/*
 * Function for send TCP packet, to specific port
 *
 * params: host - specific host, which will be used for sending
 *         interfc - specific interface, which will be used for sending packet
 *         port - which port will be scanned
 */
void send_to_port(IsamonIP host, IsamonInterfc interfc, uint16_t port) {
    int sockfd;
    struct sockaddr_in server_addr{};

    //open socket tcp
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        perror("Create socket (TCP)");
        exit(TCPERR);
    }

    //bind socket to specific interface
    if(setsockopt( sockfd, SOL_SOCKET, SO_BINDTODEVICE, interfc.getName().c_str(), (int)interfc.getName().size()) != 0)
    {
        cerr << "ERROR: setsockopt! Can't bind socket to specific interface!";
        exit(TCPERR);
    }

    //make non blocking connection
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

    //fill information about sending
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    inet_pton(AF_INET, host.getStringIp().c_str(), &server_addr.sin_addr);

    //connect
    connect(sockfd,(struct sockaddr *)&server_addr, sizeof(server_addr));

    //close socket
    close(sockfd);
}
