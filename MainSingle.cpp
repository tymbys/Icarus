#include "MainSingle.h"


namespace pt = boost::property_tree;

void MainSingle::InitWebServer(int port) {

    _web_server = thread([this](int port) {
        //logger = std::make_shared<PrintfLogger>(Logger::Level::DEBUG);
        //logger = std::make_shared<PrintfLogger>(verbose ? Logger::Level::DEBUG : Logger::Level::ACCESS);
        logger = std::make_shared<PrintfLogger>(Logger::Level::DEBUG);

        ws_server = make_shared<MyServer>(logger);
        //ws_server = new MyServer(logger);

        //ws_server->setClientBufferSize(100 * 1024 * 1020);
        ws_server->setLameConnectionTimeoutSeconds(60);

        //shared_ptr<MyHandler> handler = std::make_shared<MyHandler>(ws_server);

        handler = std::make_shared<MyHandler>(ws_server.get());
        //p_handler = new MyHandler(ws_server.get());

        //handler->chooseProtocol();
        ws_server->addWebSocketHandler("/ws", handler);

        ws_server->serve(".", port);
    }, std::ref(port));

    while (ws_server == nullptr) {
        cout << "Start Web Server!" << endl;
        sleep(1);
    }
}

void MainSingle::InitRemoteBeacons(string json) {
    pt::ptree root;
    pt::read_json(json, root);

    for (auto &beacon : root) {
        //int y = 0;
        //for (pt::ptree::value_type &cell : row.second) {
        //    matrix[x][y] = cell.second.get_value<int>();
        //    y++;
        //}
        //x++;
        //cout << "row.second[ip]" << beacon.second.get_value<string>("ip") << endl;

        string ip = beacon.second.get<string>("ip");
        string port = beacon.second.get<string>("port");
        uint8_t status = beacon.second.get<uint8_t>("status");
        uint8_t type = beacon.second.get<uint8_t>("type");

        if ((ip.length() > 0) && (port.length() > 0)) {
            RemoteBeacon _remore_beacon(ip, port, status, type);
            _remore_beacons.push_back(_remore_beacon);

            cout << "add to vector->" << endl
                    << "ip:\t" << ip << endl
                    << "port:\t" << port << endl
                    << "status:\t" << (int) status << endl
                    << "type:\t" << (int) type << endl;

        }
    }

    StartCalibrationCamera();

    exit(0);


    _processing_beacon_clients = thread([this]() {

        delay_processing = 1;
        is_run_processing_beacon_clients = true;

        Beacon beacon;
        //        beacon.data[0] = 0xDE;
        //        beacon.data[1] = 0xED;
        //        beacon.data[2] = 0xBE;
        //        beacon.data[3] = 0xEF;
        //        beacon.data[4] = 0x00;
        //        beacon.data[5] = 0x02;
        //        beacon.data[6] = 0x00;
        //        beacon.data[7] = 0;
        beacon.data[0] = 0;
        beacon.data[1] = 0;

        Message m;

        m.setBody(beacon);

        while (is_run_processing_beacon_clients) {
            beacon.data[1] = beacon.data[1] ? 0 : 1;
                    m.setBody(beacon);
            for (auto b : _remore_beacons) {

                //SendMesageToTCPServer(b.ip, b.port, m, NULL, 0, NULL);
                SendMesageToTCPServer(b.ip, b.port, m);
                        //MainSingle::get().SendMesageToTCPServer("192.168.1.105", "10000", beacon);

                        sleep(delay_processing);
            }
        }

    });

    while (is_run_processing_beacon_clients) {
        cout << "Start processing beacon clients!" << endl;
        sleep(1);
    }


}

void MainSingle::StartCalibrationCamera() {
    Message m;
    char rx[100];

    m.setHeader(Message::CMD_INIT_CAMERA);

    for (auto b : _remore_beacons) {

        if (b.type == 2) {
            //size_t len = SendMesageToTCPServer(b.ip, b.port, m);
            SendMesageToTCPServer(b.ip, b.port, m);
            //Camera m.getBody();
            //MainSingle::get().SendMesageToTCPServer("192.168.1.105", "10000", beacon);
            //            for (int i = 0; i < len; i++) {
            //                printf("0x%X02 ", rx[i]);
            //            }

            sleep(delay_processing);
        }
    }

}

void MainSingle::Init() {
    InitWebServer(8080);
    InitRemoteBeacons("RemoteBeacons.json");

}

thread &MainSingle::GetServerThread() {
    return _web_server;
}

// shared_ptr<MyHandler>  MainSingle::GetServerHandler(){
//     return handler;
// }

void MainSingle::setSensors(string json) {
    handler->setValue(json);
}

//size_t MainSingle::SendMesageToTCPServer(string ip, string port, Message &mesage, char *tx_data_body, size_t tx_len, char *rx_data) {

void MainSingle::SendMesageToTCPServer(string ip, string port, Message &mesage) {
    boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);
    auto endpoint_iterator = resolver.resolve({ip.c_str(), port.c_str()});
    cout << "ip: " << ip << ", port: " << port << endl;
    //auto endpoint_iterator = resolver.resolve({"192.168.1.111", "10000"});
    TCPClient c(io_service, endpoint_iterator);



    c.write(mesage);
    //boost::asio::write(c.get(), boost::asio::buffer(tx_data_body, tx_len));

    try {
        io_service.run();
    } catch (...) {
        cout << "Error conekted to " << ip << ": " << port << endl;
    }

    _points_from_camera.clear();

    size_t read_len = c.getReadBodyLen();
    if (read_len > 0) {
        char *byff = (char*)malloc(read_len);
        c.getReadBodyData(byff);
        
        
        cout << endl;
        for(int i =0; i< read_len/sizeof(C_Point); i++ ){
            C_Point *p = (C_Point *)&byff[i*sizeof(C_Point)];
            C_Point cam;
            
            cam.alpha = p->alpha;
            cam.beta = p->beta;
            
            cout << i<< " | alpha: " << cam.alpha << "| beta" << cam.beta  << endl;
            
        //_points_from_camera.push_back(cam);
            //_points_from_camera.push_back(*p);
            _points_from_camera.push_back(*(C_Point *)&byff[i*sizeof(C_Point)]);
        }
        
        free(byff);
        cout << endl;
        for(auto i: _points_from_camera ){
            cout << " | alpha: " << i.alpha << "| beta" << i.beta  << endl;
        }
    }
    //return c.getReadData(rx_data);


    //    std::thread t([&io_service]() {
    //        io_service.run(); });
}

//void MainSingle::SendMesageToCamera(string ip, string port, Message &mesage) {
//    boost::asio::io_service io_service;
//    tcp::resolver resolver(io_service);
//    auto endpoint_iterator = resolver.resolve({ip.c_str(), port.c_str()});
//    cout << "ip: " << ip << ", port: " << port << endl;
//    //auto endpoint_iterator = resolver.resolve({"192.168.1.111", "10000"});
//    TCPClient c(io_service, endpoint_iterator);
//
//
//
//    c.write(mesage);
//    try {
//        io_service.run();
//    } catch (...) {
//        cout << "Error conekted to "<< ip<< ": " << port << endl;
//    }
//
//
//
//    //    std::thread t([&io_service]() {
//    //        io_service.run(); });
//}