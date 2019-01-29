/**
 *  Main file
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */

#include <iostream>

#include "IsamonArgs.h"
#include "IsamonIP.h"
#include "IsamonNetAddr.h"
#include "IsamonErrorNo.h"
#include "IsamonInterfaces.h"
#include "IsamonLiveScan.h"
#include "IsamonTCPscan.h"
#include "IsamonUDP.h"
#include "IsamonUDPscan.h"

using namespace std;

//main function of the isamon
int main(int argc, char** argv)
{
    //parsing inputs arguments
    IsamonArgs arguments(argc,argv);

    //if there was error in parsing return that
    if(arguments.isErr()) {
        cerr << arguments.getErrMsg();
        return PARSERR;
    }

    //if the help is set, print and finish
    if(arguments.isSetH()) {
        arguments.printHelp();
        return OK;
    }

    IsamonNetAddr netAddr(arguments.getNetAddr(),arguments.getNetMask());

    //check if net Addr is correct
    if(!netAddr.isCorrect()) {
        cerr << "ERROR: Invalid network address for scanning. "<< endl
             << "IP address '" << netAddr.getStrNetIp() << "' is not network address with mask '" << netAddr.getStrNetMask() << "'!";
        return NETADDRERR;
    }

    //Set interface
    IsamonInterfaces interfaces;

    //if the interface option is set, only one interface can be use for scanning
    //otherwise use every interface for scanning
    if(arguments.isSetI()) {

        if(!interfaces.setInterface(arguments.getInterfaceName())) {
            cerr << "ERROR: Invalid name of interface! None interface with this name: "<< arguments.getInterfaceName() << endl;
            exit(INTERFACERR);
        }
    } else {
        interfaces.setAllInterfaces();
    }

    //If ther is not valid interface, we can end
    if(!interfaces.getFirstInterface().isValidInterface()) {
        return OK;
    }

    //set default timeout for waiting for packets
    unsigned int timeout = 2000;
    //Set timeout
    if(arguments.isSetW()) {
        timeout = arguments.howLongWait();
    }

    //create live vector for active hosts
    IsamonLiveVect liveVect(interfaces);

    liveScan(netAddr, interfaces, timeout, liveVect);

    //there is no host up
    if(!liveVect.getFirstHost().isValid()) {
        return OK;
    }

    //default scan is all ports
    int port = -1;
    if(arguments.isSetP()) {
        port = arguments.getPort();
    }

    //if the tcp option is set, so scan it
    if(arguments.isSetT()) {
        tcpScan(netAddr, timeout, liveVect, port);
    }

    //if the udp option is set so scan it
    if(arguments.isSetU()) {
        udpScan(netAddr, timeout, liveVect, port);
    }

    //print live vectors
    liveVect.printVect();
    return OK;
}
