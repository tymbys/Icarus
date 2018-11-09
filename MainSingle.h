#ifndef MAINSINGLE_H
#define MAINSINGLE_H

#include "controls/WebServer.h"
#include "controls/TCPClient.h"
#include "models/Beacon.h"
#include "models/RemoteBeacon.h"
#include "models/Message.h"
#include <unistd.h>
#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


class MainSingle {
public:

    static MainSingle& get() {
        static MainSingle instance;
        return instance;
    }

    void InitWebServer(int port);
    void InitRemoteBeacons(string json);
    void StartCalibrationCamera();
    
    void Init();
    
    void SendMesageToTCPServer(string ip, string port, Message &mesage);
    
    thread &GetServerThread();
    void setSensors(string json);
    
private:
    thread _web_server;
    
    thread _processing_beacon_clients;
    bool is_run_processing_beacon_clients;
    uint8_t delay_processing;
    
    
    shared_ptr<PrintfLogger> logger;
    shared_ptr<MyServer> ws_server;
    shared_ptr<MyHandler> handler;
    MyHandler *p_handler;
    
    
    vector<RemoteBeacon> _remore_beacons;
};

#endif /* MAINSINGLE_H */

