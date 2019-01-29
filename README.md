# ISAMON 


### Author
Vladimir Jerabek [xjerab21@stud.fit.vutbr.cz]

### What is it?
    Isamon is a network monitoring application, which can scan internet network and find active devices.
    It can also scan the devices open TCP ports as well as UDP ports.


### Usage:

```
$ isamon [-h] [-i <interface>] [-t] [-u] [-p <port>] [-w <ms>] -n <net_address/mask>
        -h --help                        - show help
        -i --interface <interface>       - interface, which will be use for scanning
        -n --network <net_address/mask>  - ip address of network whit a mask, which define the range for scanning

        -t --tcp                         - TCP will be use for scanning
        -u --udp                         - UDP will be use for scanning
        -p --port <port>                 - set specific scanning port, otherwise the whole range of ports is scanned (0-65536)
        -w --wait <ms>                   - set timeout to wait for receiving packets
```

### Returns code:.
```
    O   -   OK (everything is ok);
    1   -   ERROR in parsing arguments
    2   -   ERROR in set range (net address is not net address)

    5   -   ERROR in interface
    6   -   ERROR in creating or sending ICMP packet
    7   -   ERROR when TCP connection is creating
    8   -   ERROR when UDP packet is sending or creating

    10  -   ERROR in capturing packets (pcap faild)
```

### Examples:

    $ isamon -h                             ->  The 'help' is printed

    $ isamon -i eth0 -n 192.168.1.0/24      ->  scans the network and displays
                                                active clients using interface eth0

    $ isamon -n 192.168.1.0/30              ->  scans the network an print the
                                                active clients using all interfaces

    $ isamon -n 192.168.1.0/28 -t -p 22     ->  performs network scanning and displays
                                                active clients with open TCP port 22
                                                using all interfaces


    $ isamon -n 192.168.1.0/30 -t -u -w 5   ->  performs network scanning and displays
                                                active clients and all open TCP and UDP
                                                ports using all interfaces
                                                if the client does not respond
                                                within 5ms, isamon will consider
                                                the port as closed


### Examples of running:
```
    $ isamon -n 192.168.1.0/30 -t -u -w 5
    192.168.1.1
    192.168.1.1 TCP 80
    192.168.1.1 TCP 22
    192.168.1.1 UDP 53
```
### Note:
    isamon needs the privilages for creating RAW sockets and also use bpf programs.

### List of inclued files:
```
    Makefile
    manual.pdf
    README
    src/
        IsamonArgs.cpp
        IsamonArgs.h
        IsamonARP.cpp
        IsamonARP.h
        IsamonErrorNo.h
        IsamonHost.cpp
        IsamonHost.h
        IsamonICMP.cpp
        IsamonICMP.h
        IsamonInterfaces.cpp
        IsamonInterfaces.h
        IsamonInterfc.cpp
        IsamonInterfc.h
        IsamonIP.cpp
        IsamonIP.h
        IsamonLiveScan.cpp
        IsamonLiveScan.h
        IsamonLiveVect.cpp
        IsamonLiveVect.h
        IsamonMAC.cpp
        IsamonMAC.h
        IsamonNetAddr.cpp
        IsamonNetAddr.h
        IsamonPcap.cpp
        IsamonPcap.h
        IsamonTCP.cpp
        IsamonTCP.h
        IsamonTCPscan.cpp
        IsamonTCPscan.h
        IsamonUDP.cpp
        IsamonUDP.h
        IsamonUDPscan.cpp
        IsamonUDPscan.h
        main.cpp
        Makefile
```

Created by Vladimir Jerabek
