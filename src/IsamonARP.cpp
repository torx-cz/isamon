/**
 *  Source file for sending ARP requests to specific host via specific interface.
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <net/if.h>
#include <netinet/in.h>

#include <arpa/inet.h> //inet_ntoa
#include <unistd.h>
#include <net/ethernet.h>
#include <iostream>

#include "IsamonARP.h"

using namespace std;

typedef struct {
    unsigned short 	hrd;		        //hardware address type
    unsigned short 	proto;			    //protocol adress type
    unsigned char 	hrd_add_len;		//hardware address length
    unsigned char 	proto_add_len;		//Protocol adress length
    unsigned short 	op;			        //Operation
    unsigned char 	s_mac[ETH_ALEN];	//source MAC (Ethernet Address)
    unsigned char 	s_ip[4];			//source IP
    unsigned char 	d_mac[ETH_ALEN];	//destination MAC (Ethernet Address)
    unsigned char 	d_ip[4];			//destination IP
    char 		    pad[18];		    //Padding, ARP-Requests are quite small (<64)
} arp_frame;

/**
 * Constructor of ARP class.
 *
 * This function prepere everithing for future sending packets
 *
 * params: interfc: which interface has to be use for sending request
 */
IsamonARP::IsamonARP(IsamonInterfc interfc) {
    //ganaeration of ethernet frame
    IsamonIP interfcIP(interfc.getIp());
    gen_arp_frame(interfcIP, interfc);

    //set up the device for sending
    memset (&device, 0, sizeof (device));
    device.sll_ifindex = interfc.getIndex();
    device.sll_family = AF_PACKET;
    memcpy (device.sll_addr, interfc.getHwMAC(), ETH_ALEN);
    device.sll_halen = ETH_ALEN;

    //open socket for sending arp reguests
    if ((sockfd = socket (PF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) < 0)
    {
        perror("ERROR: socket() for arp requests");
        exit(EXIT_FAILURE);
    }
}

/**
 * This function send packet to specific IPv4 address
 *
 * params: host: destination host of asking
 */
void IsamonARP::send(IsamonIP ip) {
    ssize_t n;

    //sending of arp request via specific interface
    set_host_ip(ip);
    n = sendto(sockfd, ethFrame, 42, 0, (struct sockaddr *) &device, sizeof(device));
    if(n <= 0)
    {
        perror("ERROR: failed to sendto()");
        exit(EXIT_FAILURE);
    }
}

/**
 * Function for change the destination host in pre-prepered ethernet frame
 *
 * params: host: destination host of asking
 */
void IsamonARP::set_host_ip(IsamonIP host) {

    arp_frame *arp_header;         //build up the arp packet
    arp_header = (arp_frame *) (ethFrame + ETH_HLEN);
    
    inet_pton(AF_INET, host.getStringIp().c_str(),arp_header->d_ip);
}

/**
 * Function for generate arp packets
 *
 * params: host: destination host of asking
 *         interfc: which interface has to be use for sending request
 */
void IsamonARP::gen_arp_frame(IsamonIP host, IsamonInterfc interfc) {
    arp_frame *arp_header;         //build up the arp packet
    struct ethhdr *eth_header;            //build up ethernet header, from if_ether.h
    unsigned char d_mac[ETH_ALEN] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};    //Ethernet dest. Address
    unsigned char dest_mac[ETH_ALEN] = {0};


    eth_header = (struct ethhdr *) ethFrame;                 //build up the ethernet packet
    memcpy(eth_header->h_dest, d_mac, ETH_ALEN);


    eth_header->h_proto = htons(0x0806);            //0x0806 for Address Resolution Packet
    memcpy(eth_header->h_source, interfc.getHwMAC(), ETH_ALEN);

    arp_header = (arp_frame *) (ethFrame + ETH_HLEN);  //start address in mem
    arp_header->hrd = htons(0x0001);                        //0x0001 for 802.3 Frames
    arp_header->proto = htons(0x0800);                      //0x0800 protocol -> IPv4
    arp_header->hrd_add_len = ETH_ALEN;                     // 6 for eth-mac addr
    arp_header->proto_add_len = 4;                          //4 for IPv4 addr
    arp_header->op = htons(0x0001);                     //0x0001 for ARP Request
    inet_pton (AF_INET,interfc.getIp().c_str(), arp_header->s_ip);      //source ip addrs
    inet_pton(AF_INET, host.getStringIp().c_str(),arp_header->d_ip);    //destination ip addres
    memcpy(arp_header->s_mac, interfc.getHwMAC(), ETH_ALEN);            //source MAC addr
    memcpy(arp_header->d_mac, dest_mac, ETH_ALEN);            //Set destination mac in arp-header
    bzero(arp_header->pad, 18);
}

/**
 * Destructor is used for the closing socket hendler
 */
IsamonARP::~IsamonARP() {
    close(sockfd);
}
