/**
 *  Source file for parsing inputs arguments and holding their values
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */

#include "IsamonArgs.h"
#include <cstring>
#include <limits>
#include <iostream>

/*
 * Constructor of IsamonArgs
 * This function pars arguments and get the values from them
 *
 * params:
 *      argc: number of arguments
 *      argv: the array of c_strings of arguments
 */
IsamonArgs::IsamonArgs(int argc, char **argv) {
    numArg = argc;

    int actualArg = 1;
    int parsErr = 0;

    //very few number of arguments
    if(numArg < 2) {
        err = true;
        errMasseg = "ERROR: Very few arguments!\n\nFor more information './ismanon -h' or './isamon --help'\n";
        return;
    }

    //main cycle of parsing of arguments
    while((actualArg < numArg) && (parsErr == 0)) {
        parsErr = parseArgs(actualArg, argv);
        actualArg++;
    }

    //if help argument is found doesn't continue
    if(h) {
        return;
    }

    //process a parsing err
    if(parsErr != 0) {
        err = true;
        string arg(argv[actualArg-1]);

        switch (parsErr) {
            case 1:
                errMasseg = "ERROR: Something missing behind argument '" + arg +"'!\n\nFor more information './ismanon -h' or './isamon --help'\n";
                break;

            //It is not supperot argument
            case 2:
                errMasseg = "ERROR: It is not support argument: '" + arg +"'!\n\nFor more information './ismanon -h' or './isamon --help'\n";
                break;

            //duplicate argument
            case 5:
                errMasseg = "ERROR: This argument '" + arg + "' is used more than once!\n\nFor more information './ismanon -h' or './isamon --help'\n";
                break;

            //wrong port number;
            case 10:
                errMasseg = "ERROR: Wrong port number! \n\nFor more information './ismanon -h' or './isamon --help'\n";
                break;

            //too long waiting
            case 20:
                errMasseg = "ERROR: Wrong waiting time! \n\nFor more information './ismanon -h' or './isamon --help'\n";
                break;

            //wrong structure of net address
            case 30:
                errMasseg = "ERROR: Wrong structure of net address!\n\nFor more information './ismanon -h' or './isamon --help'\n";
                break;

            //wrong structure of net mask
            case 40:
                errMasseg = "ERROR: Wrong structure of net mask!\n\nFor more information './ismanon -h' or './isamon --help'\n";
                break;

            //net addr is not the net addr
            case 50:
                errMasseg = "ERROR: Wrong net address! It is not net address!\n\nFor more information './ismanon -h' or './isamon --help'\n";
                break;

            default:
                errMasseg = "ERROR: General error!\n\nFor more information './ismanon -h' or './isamon --help'\n";
                break;
        }

        return;
    }


    //if net address is not entered either help arguments is not entered is error
    if (!n) {
        err = true;
        errMasseg = "ERROR: Net address and net mask is not entered!\n\nFor more information './ismanon -h' or './isamon --help'\n";
        return;
    }

    //port option is set, but there is no way to scan ports no tcp and no udp options is set
    if (p && !t && !u) {
        err = true;
        errMasseg = "ERROR: Not valid combination of options (port is set, but none scan technique tcp or udp)!\n\nFor more information './ismanon -h' or './isamon --help'\n";
        return;
    }
}


bool IsamonArgs::isErr() {
    return err;
}

string IsamonArgs::getErrMsg() {
    return errMasseg;
}

bool IsamonArgs::isSetH() {
    return h;
}

bool IsamonArgs::isSetI() {
    return i;
}

bool IsamonArgs::isSetT() {
    return t;
}

bool IsamonArgs::isSetU() {
    return u;
}

bool IsamonArgs::isSetP() {
    return p;
}

bool IsamonArgs::isSetW() {
    return w;
}

bool IsamonArgs::isSetN() {
    return n;
}

string IsamonArgs::getInterfaceName() {
    return interfaceName;
}

uint16_t IsamonArgs::getPort() {
    return port;
}

unsigned int IsamonArgs::howLongWait() {
    return ms;
}

uint32_t IsamonArgs::getNetAddr() {
    return netAddres;
}

uint32_t IsamonArgs::getNetMask() {
    return binaryNetMask;
}

/*
 * Function for parsing arguments
 */
int IsamonArgs::parseArgs(int& actualArg, char **argv) {
    // for help argument
    if((!strcmp(argv[actualArg],"-h")) || (!strcmp(argv[actualArg],"--help"))) {
        if (h)
            return 5;

        h = true;
        return 0;
    }

    // for tcp argument
    if((!strcmp(argv[actualArg],"-t")) || (!strcmp(argv[actualArg],"--tcp"))) {
        if (t)
            return 5;

        t = true;
        return 0;
    }

    // for udp argument
    if((!strcmp(argv[actualArg],"-u")) || (!strcmp(argv[actualArg],"--udp"))) {
        if (u)
            return 5;

        u = true;
        return 0;
    }

    // for port argument
    if((!strcmp(argv[actualArg],"-p")) || (!strcmp(argv[actualArg],"--port"))) {
        if (p)
            return 5;

        p = true;

        //save port number
        actualArg = actualArg + 1;
        if(actualArg < numArg) {
            char * pEnd = nullptr;
            unsigned long int tmpNum = (unsigned) strtol(argv[actualArg],&pEnd,10);

            //Wrong sctructure of port number
            if(tmpNum > 65535 || strcmp(pEnd,"")!=0)
                return 10;

            port = (uint16_t) tmpNum;

        } else {
            actualArg = actualArg - 1;
            return 1;
        }

        return 0;
    }

    // for interface argument
    if((!strcmp(argv[actualArg],"-i")) || (!strcmp(argv[actualArg],"--interface"))) {
        if (i)
            return 5;

        i = true;

        //save interface name number
        actualArg = actualArg + 1;
        if(actualArg < numArg) {
            string tmpInterfaceName(argv[actualArg]);
            interfaceName = tmpInterfaceName;
        } else {
            actualArg = actualArg - 1;
            return 1;
        }

        return 0;
    }

    // for wait argument
    if((!strcmp(argv[actualArg],"-w")) || (!strcmp(argv[actualArg],"--wait"))) {
        if (w)
            return 5;

        w = true;

        //save ms of waiting
        actualArg = actualArg + 1;
        if(actualArg < numArg) {
            char * pEnd = nullptr;
            unsigned long int tmpWait = (unsigned) strtol(argv[actualArg],&pEnd,10);

            //Too long waiting
            if(tmpWait > numeric_limits<int>::max() || strcmp(pEnd,"")!=0)
                return 20;

            ms = (unsigned int) tmpWait;

        } else {
            actualArg = actualArg - 1;
            return 1;
        }

        return 0;
    }


    //net address with mask
    if(!strcmp(argv[actualArg],"-n") || (!strcmp(argv[actualArg],"--network"))) {
        if (n)
            return 5;

        n = true;

        //save net address and mask
        actualArg = actualArg + 1;
        if(actualArg < numArg) {
            char * pointChar = argv[actualArg];
            bool isMask = false;
            unsigned int array[4] = {0};

            int indx = 0;

            //get net addr
            while (*pointChar != '\0') {
                if (!isMask) {
                    switch (*pointChar) {
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                        case '8':
                        case '9':
                        case '0':
                            array[indx] = 10 * array[indx] + (*pointChar - '0');
                            break;

                        case '.': //delimiter between each byte writenn in decadic
                            indx++;
                            break;

                        case '/': //delimiter for net mask
                            isMask = true;
                            break;

                        default:  //unexpected character
                            return 30;
                    }
                } else {
                    if (isdigit(*pointChar)) {
                        netMask =  netMask * 10 + (*pointChar - '0');
                    }
                    else {
                        return 40;
                    }
                }
                //get next character
                pointChar++;
            }

            if(indx != 3 || !isMask)
                return 30;

            for (int j = 0; j < 4; ++j) {
                if (array[i] > 255)
                    return 30;
            }

            netAddres = (array[0] << 24) |
                        (array[1] << 16) |
                        (array[2] <<  8) |
                        (array[3]);


            if (netMask > 32)
                return 40;

            binaryNetMask = binaryNetMask << (32 - netMask);


        } else {
            actualArg = actualArg - 1;
            return 1;
        }

        return 0;
    }

    return 2;

}

// Function for printing help to cout.
void IsamonArgs::printHelp() {
    cout << "Usage:" << endl << endl;
    cout << "isamon [-h] [-i <interface>] [-t] [-u] [-p <port>] [-w <ms>] -n <net_address/mask> " << endl;
    cout << "\t-h --help                        - show help" << endl;
    cout << "\t-i --interface <interface>       - interface, which will be use for scanning" << endl;
    cout << "\t-n --network <net_address/mask>  - ip address of network whit a mask, which define the range for scanning " << endl;
    cout << "\t-t --tcp                         - TCP will be use for scanning" << endl;
    cout << "\t-u --udp                         - UDP will be use for scanning" << endl;
    cout << "\t-p --port <port>                 - set specific scanning port, otherwise the whole range of ports is scanned (0-65536)" << endl;
    cout << "\t-w --wait <ms>                   - set timeout to wait for receiving packets" << endl;
}
