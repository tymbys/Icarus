#ifndef TCPCLIENTCONTROL_H
#define TCPCLIENTCONTROL_H

#include <iostream>
#include <sstream>

#include "../models/Message.h"
#include "TCPClient.h"

using namespace std;

class TCPClientControl {
public:
    TCPClientControl();
    virtual ~TCPClientControl();
    
    
    POINTS ConvertDataToPoints(TCPClient &c);
    bool SendMesageToTCPServer(string ip, string port, Message &mesage);
    
    POINTS GetPoints();
private:
    POINTS _points_from_camera;
};

#endif /* TCPCLIENTCONTROL_H */

