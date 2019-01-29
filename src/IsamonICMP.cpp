/**
 *  Source file for sending ICMP ECHO request to host via IPv4.
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */


#include "IsamonICMP.h"
#include "IsamonErrorNo.h"

#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>


//Size of ICMP packet
#define PACKETSIZE 64

//Protocol of ICMP
struct protoent *proto = NULL;

//Struct of ICMP packet
//It consists of IMCP header and massage
struct packet
{
    struct icmphdr hdr;
    char msg[PACKETSIZE-sizeof(struct icmphdr)];
};


/*
 * Function for counting controlling checksum of ICMP packet.
 *
 * params:
 *      b = it is a pointer to ICMP massage
 *      len = length of ICMP massage
 */
uint16_t checksum(void *b, int len);

/*
 * Function for sending ICMP Echo request to the specific IPv4 address.
 *
 * params:
 *      host:       IP address of host
 *      interfc:    which interface has to be used for sending ICMP Echo request
 */
void ICMP_Send(IsamonIP host, IsamonInterfc interfc) {

    //protocol of the sending massage <= ICMP
    proto = getprotobyname("ICMP");

    //PID of the thread for id of ICMP massage
    auto pid = (uint16_t) getpid();

    struct sockaddr_in addr = {0};      //variable for sending info
    const int val=255;                  //value of TTL
    int sd;                             //socket descriptor
    static uint16_t cnt = 1;            //No of sending ICMP echo request

    struct packet pckt{};       //ICMP packet

    //fill the sending info
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;  //IPv4
    addr.sin_port = 0;          //port number
    inet_aton(host.getStringIp().c_str(), (in_addr *) &addr.sin_addr.s_addr);   //host IP

    //create socket
    sd = socket(PF_INET, SOCK_RAW, proto->p_proto);
    if ( sd < 0 ) {

        perror("Create socket (ICMP)");
        exit(ICMPERR);
    }

    //use specific interface for sending ICMP massage
    if(setsockopt( sd,
                   SOL_SOCKET,
                   SO_BINDTODEVICE,
                   interfc.getName().c_str(),
                   (socklen_t)interfc.getName().size()) != 0)
    {
        perror("Set INTERFACE (ICMP)");
        exit(ICMPERR);
    }

    //if it's possible, increase the TTL of ICMP packet
    if ( setsockopt(sd, SOL_IP, IP_TTL, &val, sizeof(val)) != 0)
        perror("Set TTL option (ICMP)");

    //fill ICMP packet up
    bzero(&pckt, sizeof(pckt));
    pckt.hdr.type = ICMP_ECHO;
    pckt.hdr.code = 0;
    pckt.hdr.un.echo.id = pid;

    //sequence of ICMP massage
    pckt.hdr.un.echo.sequence = cnt++;
    pckt.hdr.checksum = checksum(&pckt, sizeof(pckt)); //count checksum

    //send ICMP massage to the internet
    if ( sendto(sd, &pckt, sizeof(pckt), 0,(struct sockaddr *) &addr, sizeof(addr)) <= 0 ) {
        perror("Sendto (ICMP)");
        exit(ICMPERR);
    }

    //close socket
    close(sd);
}

void ICMP_Stuck(IsamonInterfc interfc) {
    IsamonIP ip(interfc.getIp());

    ICMP_Send(ip, interfc);
}

/*
 * Function for counting controlling checksum of ICMP packet.
 *
 * params:
 *      b = it is a pointer to ICMP massage
 *      len = length of ICMP massage
 */
uint16_t checksum(void *b, int len) {

    auto *buf = (uint16_t *) b;
    unsigned long sum = 0;
    uint16_t result;


    //Compute Internet Checksum
    for ( sum = 0; len > 1; len -= 2 )
        sum += *buf++;

    //  Add left-over byte, if any
    if ( len > 0 )
        sum += *(unsigned char*)buf;


    //  Fold 32-bit sum to 16 bits
    while(sum>>16) {
        sum = (sum >> 16) + (sum & 0xFFFF);
    }

    result = (uint16_t) ~sum;
    return result;
}
