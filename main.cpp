#include <iostream>
#include <unistd.h>
#include <string>
#include <sstream>
#include "MainSingle.h"
#include "models/Beacon.h"
//#include "controls/TCPClient.h"

using namespace std;

Beacon _beacon;

int main(int argc, char** argv) {

    MainSingle::get().Init();

    thread &Server = MainSingle::get().GetServerThread();
    cout << "Server.join()" << endl;


    int i = 0;
    Beacon beacon;
    beacon.data[0] = 0xDE;
    beacon.data[1] = 0xED;
    beacon.data[2] = 0xBE;
    beacon.data[3] = 0xEF;
    beacon.data[4] = 0x00;
    beacon.data[5] = 0x02;
    beacon.data[6] = 0x00;
    beacon.data[7] = 0;

    while (1) {
        ostringstream ss;
        //ss << "[{\"id\":1, \"dx\": " << i << ", \"dy\": 1}, {\"id\": 2,\"dx\": 2, \"dy\": 2}]";
        
//        ss << "[{\"id\":1, \"dx\": 0, \"dy\": 0},{\"id\":2, \"dx\": -7, \"dy\": -4.25},{\"id\":3, \"dx\": -7, \"dy\": -13.7447}]";
//        
//        ss.flush();
//        string _sensors = ss.str();        
//        MainSingle::get().setSensors(_sensors);
//        //cout << "_sensors: "<< _sensors << endl;
//        i++;

        //beacon.data[7] = beacon.data[7] ? 0 : 1;
//
//        cout << "Start SendMesageToTCPServer" << endl;
        //MainSingle::get().SendMesageToTCPServer("192.168.1.111", "10000", beacon);
//        cout << "End SendMesageToTCPServer" << endl;
        usleep(3000000);
    }

    Server.join();
    cout << "Hello World!" << endl;
    return 0;
}

