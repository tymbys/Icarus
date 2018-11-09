#ifndef REMOTEBEACON_H
#define REMOTEBEACON_H
#include <string>

using namespace std;

struct RemoteBeacon {
    
    RemoteBeacon(string _ip, string _port, uint8_t _status, uint8_t _type);
    
    string ip;
    string port;
    uint8_t status;
    uint8_t type;
};

#endif /* REMOTEBEACON_H */

