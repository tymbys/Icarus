#include "WebServer.h"
#include "../MainSingle.h"

MyHandler::MyHandler(MyServer* server) : _server(server), _currentValue(0) {
}

void MyHandler::onConnect(WebSocket* connection) {

    cout << "onConnect getRequestUri: " << connection->getRequestUri() << endl;
    //cout << "onConnect getRequestUri: " << connection->getHeader() << endl;

    //_connections.insert(connection);
    _connections.insert(connection);
    //connection->send(_currentSetValue);
    //sleep(1);
    cout << "Connected: " << connection->getRequestUri()
            << " : " << formatAddress(connection->getRemoteAddress())
            << endl;
    cout << "Credentials: " << *(connection->credentials()) << endl;
}

void MyHandler::onData(WebSocket* connection, const char* data) {
    //        if (0 == strcmp("die", data)) {
    //            _server->terminate();
    //            return;
    //        }
    //        if (0 == strcmp("close", data)) {
    //            cout << "Closing.." << endl;
    //            connection->close();
    //            cout << "Closed." << endl;
    //            return;
    //        }
    //
    //        int value = atoi(data) + 1;
    //        if (value > _currentValue) {
    //            setValue(value);
    //            for (auto c : _connections) {
    //                c->send(_currentSetValue.c_str());
    //            }
    //        }

    std::stringstream ss;
    ss << data;
    ss.flush();
    
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(ss, pt);
    string ip =  pt.get<std::string>("ip") ;
    string port =  pt.get<std::string>("port") ;
    bool checked =  pt.get<bool>("checked") ;
    
    Message m;
    Beacon beacon;
    beacon.data[0] = 0;
    beacon.data[1] = checked;
    
    m.setBody(beacon);
    //MainSingle::get().SendMesageToTCPServer(ip, port, m);
    
    TCPClientControl tcp_client;
    tcp_client.SendMesageToTCPServer(ip, port, m);
    
    cout << "onData: " << data << endl;
}

void MyHandler::onDisconnect(WebSocket* connection) {
    _connections.erase(connection);
    cout << "Disconnected: " << connection->getRequestUri()
            << " : " << formatAddress(connection->getRemoteAddress())
            << endl;


}

void MyHandler::setValue(int value) {
    _currentValue = value;
    _currentSetValue = makeExecString("set", _currentValue);
}

void MyHandler::setValue(string value) {
    _currentSetValue = value;
    //cout  <<__func__ << endl;
    for (auto c : _connections) {
        c->send(_currentSetValue.c_str());
        //cout << "_currentSetValue: " <<_currentSetValue << endl;
    }
}

//void MyHandler::setSensors(string json) {
//    _currentSetValue = json;
//
//    for (auto c : _connections) {
//        //c->send(_currentSetValue.c_str());
//    }
//}