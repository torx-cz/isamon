/**
 *  Header file error numbers
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */

#ifndef ISA_ISAMONERRORNO_H
#define ISA_ISAMONERRORNO_H

//return code
#define OK 0            //everything was ok
#define PARSERR 1       //error in parsing arguments
#define NETADDRERR 2    //error in set range (net address is not net addres)
#define INTERFACERR 5   //error in interfaces
#define ICMPERR 6   // error in icmp
#define TCPERR 7    // error when tcp packet is sending
#define UDPERR 8    // error when udp packet is sending

#define PCAPERR 10  //error in pcap class

#endif //ISA_ISAMONERRORNO_H
