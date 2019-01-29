/**
 *  Source file of class, which capture and process the packets
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */


#include <iostream>
#include <csignal>
#include <unistd.h>
#include <net/ethernet.h>


#include <cstdlib>
#include <pcap.h>
#include <cerrno>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <err.h>
#include <unistd.h>
#include <csignal>
#include <netinet/ether.h>
#include <ctime>
#include <utility>
#include <pcap/sll.h>
#include <netinet/ip_icmp.h>
#include <mutex>


#include "IsamonPcap.h"

#define SLL_HDR_LEN 16
#define ICMP_HDR_LEN 8

//structure of arp header
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

//global variable
pcap_t * glob_handle;
IsamonNetAddr * glob_range;
IsamonLiveVect * glob_live;
struct bpf_program * glob_fp;
unsigned int us = 0;
unsigned int set_wait = 0;

std::mutex mutex1;


/*
 * Function for processing packets ->  it sort packets into the coressponding function
 */
void packet_process(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);

/*
 * Function for processiong arp packets.
 */
void arp_process(void *ptr);

/*
 * Function for processiong ip packets
 */
void ip_process(void *ptr);

/*
 * Function for processiong icmp packets
 */
void icmp_process(void *ptr);

/*
 * Function for processiong tcp packets
 */
void tcp_process(void *ptr);

/*
 * Function for set pcap_loop by  new scanning thread
 */
void set_loop();

/*
 * Function for hendle a SIGALRM, to break a pcap_loop()
 */
void time_is_over(int sig);


/*
 * Init PCAP filter
 *
 * params:  filter  = what will be filtered (filter expression)
 */
IsamonPcap::IsamonPcap(string filter) {

    this->dev = "any";
    char errbuf[PCAP_ERRBUF_SIZE] = {0};  // constant defined in pcap.h

    // open the interface for live sniffing
    if ((handle = pcap_open_live(this->dev.c_str(),BUFSIZ,0,1000,errbuf)) == NULL){
        cerr << "ERROR: Pcap (create handle): " << errbuf << endl;
        exit(PCAPERR);
    }

    // compile the filter
    if (pcap_compile(handle, &fp,filter.c_str(),0,PCAP_NETMASK_UNKNOWN) == -1) {
        cerr << "ERROR: Pcap (compile filter)" << errbuf << endl;
        exit(PCAPERR);
    }

    // set the filter to the packet capture handle
    if (pcap_setfilter(handle, &fp) == -1) {
        cerr << "ERROR: Pcap (set filter): " << errbuf << endl;
        exit(PCAPERR);
    }
}

/*
 * Destruct Pcap class
 */
IsamonPcap::~IsamonPcap() {
    //wait for scanning thread
    scanThread.join();

    //when the class is destroy, the handle and also filter can be closed;
    pcap_freecode(&fp);     //free filter code
    pcap_close(handle);     //close pcap handle
}

/*
 * Function for capture packets and processed that.
 *
 * params:  results = vector of hosts, which answered (returned value)
 *          range   = which Ip can be add to results
 */
void IsamonPcap::getPackets(IsamonLiveVect &results, IsamonNetAddr &range) {

    glob_range = &range;    //make access for range of scanning dev
    glob_handle = handle;   //and also for handle of pcap
    glob_live = &results;   //and also for vector of Live hosts
    glob_fp = &fp;


    thread local(set_loop);
    scanThread = std::move(local);

}

/*
 * Function for set timer
 *
 * params:  wait    = time of waiting for packets in milliseconds
 */
void IsamonPcap::setTimer(unsigned int wait) {

    mutex1.lock();
    set_wait = wait;

    if(set_wait != 0) {
        //set the timeout for pcap_loop()
        unsigned int sec = set_wait/1000;
        if(sec) {
            us = (set_wait%1000) * 1000;
            alarm(sec);
            signal(SIGALRM, time_is_over);
        } else {
            ualarm(set_wait*1000, 0);
            signal(SIGALRM, time_is_over);
        }
        set_wait = 0;
    }

}

/*
 * Function for waiting until pcap_breakloop
 */
void IsamonPcap::waitTime() {
    mutex1.lock();
    mutex1.unlock();
}


//Function for DEBUG use
void print_eth_frame(char *eth_frame, int size)
{
    char * iterator;
    iterator = eth_frame;

    printf("eth frame: \n");
    int i;
    for (i = 1; i < size+1; ++i)
    {
        printf("%02x ", iterator[i-1] & 0xff);

        if ((i%10) == 0)
            printf("\n");
    }
    printf("\nEND of ETH FRAME\n");
}

/*
 * Function for processing packets ->  it sort packets into the coressponding function
 */
void packet_process(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {

    //because the microseconds alarm is to short, we have to pot the set this alarm here.


    struct sll_header * lccptr;     // pointer to linux cooked capture, because of 'any'

    // read the Ethernet header
    lccptr = (struct sll_header *) packet;

    //Process packet in the corresponding function
    switch (ntohs((uint16_t)lccptr->sll_protocol)) {
        case ETHERTYPE_IP: // IPv4 packet
            ip_process((void*) (packet+SLL_HDR_LEN));
            break;

        case ETHERTYPE_ARP:  // ARP packet
            arp_process((void*)(packet+SLL_HDR_LEN));
            break;

        default:
            break;
    }

}

/*
 * Function for processiong arp packets.
 */
void arp_process(void *ptr) {

    arp_frame * arp_packet;
    char buf[INET_ADDRSTRLEN] = {0};

    arp_packet = (arp_frame *) ptr;
    unsigned short code = ntohs(arp_packet->op);

    //Only arp reply are used
    if (code != 0x0002)     //arp reply;
        return;

    //Get the sender IP address
    if (inet_ntop(AF_INET,arp_packet->s_ip , buf, INET_ADDRSTRLEN) == NULL) {
        perror("inet_ntop (pcap)");
        exit(PCAPERR);
    }

    IsamonIP sourceIP(buf);

    //Is the sender IP address in my range?
    if(glob_range->isIncludedIp(sourceIP)) {
        //Get the my IP address for finding which interface capture this packet
        if (inet_ntop(AF_INET,arp_packet->d_ip , buf, INET_ADDRSTRLEN) == NULL) {
            perror("inet_ntop (pcap)");
            exit(PCAPERR);
        }

        IsamonIP destIP(buf);

        //add this host to the LiveVector
        glob_live->addMember(destIP, sourceIP);
    }
}

/*
 * Function for processiong ip packets
 */
void ip_process(void *ptr) {
    struct ip *my_ip;

    my_ip = (struct ip *) ptr;

    IsamonIP sourceIP(inet_ntoa(my_ip->ip_src));
    if(glob_range->isIncludedIp(sourceIP)) {
        switch (my_ip->ip_p) {
            case 1: //ICMP
                icmp_process(ptr);
                break;

            case 6: //TCP
                tcp_process(ptr);
                break;

            default:
                break;
        }
    }
}

/*
 * Function for processiong imcp packets
 */
void icmp_process(void *ptr) {
    struct ip * my_ip;
    struct icmphdr * my_icmp;
    u_int size_ip;

    my_ip = (struct ip *) ptr;

    //Get the sender IP address
    IsamonIP sourceIP(inet_ntoa(my_ip->ip_src));

    //Get the my IP address for finding which interface capture this packet
    IsamonIP destIP(inet_ntoa(my_ip->ip_dst));

    size_ip = my_ip->ip_hl*4;

    my_icmp = (struct icmphdr *) ((char*)ptr + size_ip);

    switch (my_icmp->type) {
        case 3: //destination unreachable
            if (my_icmp->code == 3) {

                struct ip * old_ip;
                struct udphdr * old_udp;

                old_ip = (struct ip * ) ((char *)my_icmp + ICMP_HDR_LEN);
                size_ip = old_ip->ip_hl*4;

                old_udp = (struct udphdr *) ((char *)old_ip + size_ip);
                glob_live->addUDPort(sourceIP, ntohs(old_udp->dest));
            }
            break;
        case 0: //echo reply
            //add this host to the LiveVector
            glob_live->addMember(destIP, sourceIP);
            break;

        default:
            break;
    }
}

/*
 * Function for processiong tcp packets
 */
void tcp_process(void *ptr) {
    struct ip * my_ip;
    struct tcphdr *my_tcp;    // pointer to the beginning of TCP header
    u_int size_ip;

    my_ip = (struct ip *) ptr;

    //Get the sender IP address
    IsamonIP sourceIP(inet_ntoa(my_ip->ip_src));
    IsamonIP destIP(inet_ntoa(my_ip->ip_dst));
    size_ip = my_ip->ip_hl*4;

    my_tcp = (struct tcphdr *) ((char*)ptr+size_ip); // pointer to the TCP header

    //same port
    if(sourceIP.getIp() == destIP.getIp())
        if(ntohs(my_tcp->source) == ntohs(my_tcp->dest))
            return;

    if((my_tcp->syn) && (my_tcp->ack)) {
        glob_live->addTCPort(sourceIP, ntohs(my_tcp->source));
    }
}

/*
 * Function for hendle a SIGALRM, to break a pcap_loop()
 */
void time_is_over(int sig) {
    if(us > 0) {
        //if there is some time for waiting, so set it up;
        ualarm(us, 0);
        us = 0;
        signal(SIGALRM, time_is_over);
    } else {
        //break a pcap loop, because timeout is done;
        pcap_breakloop(glob_handle);
        mutex1.unlock();
    }
}

/*
 * Function for set pcap_loop by  new scanning thread
 */
void set_loop() {
    // read packets from the interface in the infinite loop (count == -1)
    // incoming packets are processed by function packet_process
    if (pcap_loop(glob_handle, -1, packet_process, NULL) == -1) {
        cerr << "ERROR: Pcap (loop) failed. " << endl;
        exit(PCAPERR);
    }
}
