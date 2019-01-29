/**
 *  Header file for parsing inputs arguments and holding their values
 *
 *  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
 *  This file is part of the ISA project 'isamon' -> monitoring of network
 */

#ifndef ISA_ISAMONARGS_H
#define ISA_ISAMONARGS_H

#include <ios>

using namespace std;

class IsamonArgs {

private:
    int numArg = 0;

    bool h = false; //if argument   --help        or -h
    bool i = false; //if argument   --interface   or -i
    bool t = false; //if argument   --tcp         or -t
    bool u = false; //if argument   --udp         or -u
    bool p = false; //if argument   --port        or -p
    bool w = false; //if argument   --wait        or -w
    bool n = false; //if argument   --network     or -n

    bool err = false; //if is error in parsing arguments, err is set on true

    string interfaceName; //if interface option is set, ther will be a name of interface
    uint16_t port;        //port number
    unsigned int ms;      //time of waiting
    uint32_t netAddres;   //like 192.168.1.0
    unsigned int netMask = 0;   //net mask
    uint32_t binaryNetMask = 0xFFFFFFFF;    //binary net mask

    string errMasseg;   //variable for holding a error massage

    /*
     * Function for parsing arguments
     */
    int parseArgs(int& actualArg, char **argv);


public:
    /*
     * Constructor of IsamonArgs
     * This function pars arguments and get the values from them
     *
     * params:
     *      argc: number of arguments
     *      argv: the array of c_strings of arguments
     */
    IsamonArgs(int argc, char** argv);

    //function which read H status
    bool isSetH();

    //function which read I status
    bool isSetI();

    //function which read T status
    bool isSetT();

    //function which read U status
    bool isSetU();

    //function which read P status
    bool isSetP();

    //function which read W status
    bool isSetW();

    //function which read N status
    bool isSetN();

    //function for get interface name from private variable
    string getInterfaceName();
    uint16_t getPort();
    unsigned int howLongWait();
    uint32_t getNetAddr();
    uint32_t getNetMask();

    //funciton which read private error variable
    bool isErr();

    //If isErr is true, than getErrMsg return error massage (already with \n)
    string getErrMsg();

    // Function for printing help to cout.
    void printHelp();
};


#endif //ISA_ISAMONARGS_H
