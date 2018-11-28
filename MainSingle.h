#ifndef MAINSINGLE_H
#define MAINSINGLE_H

#include "controls/WebServer.h"
#include "controls/TCPClientControl.h"

#include "models/Beacon.h"
#include "models/RemoteBeacon.h"
#include "models/Message.h"
#include "models/Camera.h"
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
    void TestCalibrationCamera();
    void TestCalibrationCamera_2points() ;
    
    void Init();
    
    //bool SendMesageToTCPServer(string ip, string port, Message &mesage);
    //size_t SendMesageToTCPServer(string ip, string port, Message &mesage, char *tx_data_body, size_t tx_len , char *rx_data);
    
    thread &GetServerThread();
    void setSensors(string json);
    //void ConvertDataToPoints(TCPClient &c);
    
    void Calc();
    
    void prepare_points_1point();
    
private:
    thread _web_server;
    
    thread _processing_beacon_clients;
    bool is_run_processing_beacon_clients;
    useconds_t delay_processing;
    
    
    shared_ptr<PrintfLogger> logger;
    shared_ptr<MyServer> ws_server;
    shared_ptr<MyHandler> handler;
    MyHandler *p_handler;
    
    
    vector<RemoteBeacon> _remore_beacons;
    POINTS _points_from_camera;
    
    NaviMath _navi_math;
    NaviMath::CamLocation _CamLocation;
};

#endif /* MAINSINGLE_H */

