#include "MainSingle.h"


namespace pt = boost::property_tree;

ostringstream ss_calibration;

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
    StartCalibrationCamera();

    //exit(0);


    _processing_beacon_clients = thread([this]() {

        delay_processing = 100000;
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

        enum {
            isCam1 = 1, isCam2 = 2, isCamAll = 3
        };

        while (is_run_processing_beacon_clients) {
            //beacon.data[1] = beacon.data[1] ? 0 : 1;
            beacon.data[1] = 1;
                    //                    m.setBody(beacon);
                    int id = 0;
                    double Phi1 = 0.0f;
                    double Phi2 = 0.0f;
                    uint8_t isPoint = 0;

                    ostringstream ss;
                    ss << "\"coordinates\": [";


            for (auto b : _remore_beacons) {

                if (b.type == 1) {

                    m.setBody(beacon);
                            SendMesageToTCPServer(b.ip, b.port, m);

                } else if (b.type == 2) {
                    m.setHeader(Message::CMD_GET_DATA_CAMERA);
                            bool isOk = SendMesageToTCPServer(b.ip, b.port, m);



                    if (_points_from_camera.size() > 0) {


                        //                        ss << "[";
                        bool isBeginMakeJson_coordinates = true;
                        for (auto point : _points_from_camera) {



                            if (b.status == isCam1 && isOk) {
                                Phi1 = point.alpha;
                                        isPoint |= b.status;

                            } else if (b.status == isCam2 && isOk) {
                                Phi2 = point.alpha;
                                        isPoint |= b.status;
                            }


                            if (isPoint == isCamAll) {

                                id++;

                                        NaviMath::LedLocation led_colation = _navi_math.GetLedLocation(Phi1, Phi2);
                                if (isnormal(led_colation.x1) && isnormal(led_colation.y1)) {


                                    if (isBeginMakeJson_coordinates) {
                                        isBeginMakeJson_coordinates = false;
                                                ss << "{\"id\":" << id << ", \"dx\": " << led_colation.x1 << ", \"dy\": " << led_colation.y1 << "}";
                                    } else {
                                        ss << ",{\"id\":" << id << ", \"dx\": " << led_colation.x1 << ", \"dy\": " << led_colation.y1 << "}";
                                    }
                                }
                            }

                            //if(isnormal()())



                            //cout << "led_colation.x1: " << led_colation.x1 << " led_colation.y1: " << led_colation.y1 << endl;



                            //ss << "{\"id\":"<< id << ", \"dx\": " << led_colation.x1 << ", \"dy\": "<< led_colation.y1 <<"},";
                            //ss << "[{\"id\":1, \"dx\": " << i << ", \"dy\": 1}, {\"id\": 2,\"dx\": 2, \"dy\": 2}]";

                        }

                        //                        ss << "]";
                        //                                ss.flush();
                        //
                        //                                cout << ss.str() << endl;
                        //                                setSensors(ss.str());

                    }

                }


                //SendMesageToTCPServer(b.ip, b.port, m, NULL, 0, NULL);
                //SendMesageToTCPServer(b.ip, b.port, m);
                //MainSingle::get().SendMesageToTCPServer("192.168.1.105", "10000", beacon);

                usleep(delay_processing);
            }

            ss << "]";
                    ss.flush();
                    ostringstream out_json;

                    out_json << "{" << ss_calibration.str() << ", " << ss.str() << "}";
                    out_json.flush();
                    cout << out_json.str() << endl;
                    setSensors(out_json.str());
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

    double c1_alpha1;
    double c1_beta1; //-
    double c1_alpha2;
    double c1_beta2; //-
    double c1_alpha3;
    double c1_beta3; //-

    double alpha1; // abs(c1_alpha1 - c1_alpha2)
    double beta1; // abs(c1_alpha2 - c1_alpha3)

    double c2_alpha1;
    double c2_beta1; //-
    double c2_alpha2;
    double c2_beta2; //-
    double c2_alpha3;
    double c2_beta3; //-

    double alpha2; // abs(c2_alpha1 - c2_alpha2)
    double beta2; // abs(c2_alpha2 - c2_alpha3)

    double base = 20.0f;

    uint8_t isCalibratio = 0;

    enum {
        isCalibratioCam1 = 1, isCalibratioCam2 = 2, isCalibratioCamAll = 3
    };

    ss_calibration.str("");
    ss_calibration.clear();
    ss_calibration << "\"calibration\": {";

    //    ss_calibration
    //            << "\"c1_alpha1\": " << c1_alpha1 << ", "
    //            << "\"c1_beta1\": " << c1_beta1 << ", "
    //            << "\"c1_alpha2\": " << c1_alpha2 << ", "
    //            << "\"c1_beta2\": " << c1_beta2 << ", "
    //            << "\"c1_alpha3\": " << c1_alpha3 << ", "
    //            << "\"c1_beta3\": " << c1_beta3 << ", "
    //            << "\"c2_alpha1\": " << c2_alpha1 << ", "
    //            << "\"c2_beta1\": " << c2_beta1 << ", "
    //            << "\"c2_alpha2\": " << c2_alpha2 << ", "
    //            << "\"c2_beta2\": " << c2_beta2 << ", "
    //            << "\"c2_alpha3\": " << c2_alpha3 << ", "
    //            << "\"c2_beta3\": " << c2_beta3 << ", "
    //            << "\"CamLocation.x1\": " << 1 << ", "
    //            << "\"CamLocation.y1\": " << 2 << ", "
    //            << "\"CamLocation.x2\": " << 3 << ", "
    //            << "\"CamLocation.y2\": " << 4 << "}";
    //    ss_calibration.flush();
    //cout << ss_calibration.str() << endl;
    //setSensors(ss_calibration.str());

    for (auto b : _remore_beacons) {

        if (b.type == 2) {
            //size_t len = SendMesageToTCPServer(b.ip, b.port, m);
            m.setHeader(Message::CMD_INIT_CAMERA);
            bool isOk = SendMesageToTCPServer(b.ip, b.port, m);

            if (b.status == isCalibratioCam1 && isOk) {

                c1_alpha1 = _points_from_camera[0].alpha;
                c1_beta1 = _points_from_camera[0].beta;
                c1_alpha2 = _points_from_camera[1].alpha;
                c1_beta2 = _points_from_camera[1].beta;
                c1_alpha3 = _points_from_camera[2].alpha;
                c1_beta3 = _points_from_camera[2].beta;

                alpha1 = abs(c1_alpha1 - c1_alpha2);
                beta1 = abs(c1_alpha2 - c1_alpha3);

                isCalibratio |= b.status;


                cout << "c1_alpha1: " << c1_alpha1
                        << ", c1_beta1: " << c1_beta1
                        << ", c1_alpha2: " << c1_alpha2
                        << ", c1_beta2: " << c1_beta2
                        << ", c1_alpha3: " << c1_alpha3
                        << ", c1_beta3: " << c1_beta3
                        << endl;

                //cout << endl;
                //                    for (auto i : _points_from_camera) {
                //                        cout << " | alpha: " << i.alpha << "| beta" << i.beta << endl;
                //                    }

            } else if (b.status == isCalibratioCam2 && isOk) {
                c2_alpha1 = _points_from_camera[0].alpha;
                c2_beta1 = _points_from_camera[0].beta;
                c2_alpha2 = _points_from_camera[1].alpha;
                c2_beta2 = _points_from_camera[1].beta;
                c2_alpha3 = _points_from_camera[2].alpha;
                c2_beta3 = _points_from_camera[2].beta;

                cout << "c2_alpha1: " << c2_alpha1
                        << ", c2_beta1: " << c2_beta1
                        << ", c2_alpha2: " << c2_alpha2
                        << ", c2_beta2: " << c2_beta2
                        << ", c2_alpha3: " << c2_alpha3
                        << ", c2_beta3: " << c2_beta3
                        << endl;

                alpha2 = abs(c2_alpha1 - c2_alpha2);
                beta2 = abs(c2_alpha2 - c2_alpha3);

                isCalibratio |= b.status;
            }


            if (isCalibratio == isCalibratioCamAll) {
                NaviMath::CamLocation _CamLocation = _navi_math.GetCameraLocation(alpha1, beta1, alpha2, beta2, base);

                cout << "CamLocation.x1 : " << _CamLocation.x1 << ", y1: " << _CamLocation.y1 << endl;
                cout << "CamLocation.x2 : " << _CamLocation.x2 << ", y2: " << _CamLocation.y2 << endl;

                _navi_math.SetCamLocation(_CamLocation);
                isCalibratio = 0;


                ss_calibration
                        << "\"c1_alpha1\": " << c1_alpha1 << ", "
                        << "\"c1_beta1\": " << c1_beta1 << ", "
                        << "\"c1_alpha2\": " << c1_alpha2 << ", "
                        << "\"c1_beta2\": " << c1_beta2 << ", "
                        << "\"c1_alpha3\": " << c1_alpha3 << ", "
                        << "\"c1_beta3\": " << c1_beta3 << ", "
                        << "\"c2_alpha1\": " << c2_alpha1 << ", "
                        << "\"c2_beta1\": " << c2_beta1 << ", "
                        << "\"c2_alpha2\": " << c2_alpha2 << ", "
                        << "\"c2_beta2\": " << c2_beta2 << ", "
                        << "\"c2_alpha3\": " << c2_alpha3 << ", "
                        << "\"c2_beta3\": " << c2_beta3 << ", "
                        << "\"CamLocation.x1\": " << _CamLocation.x1 << ", "
                        << "\"CamLocation.y1\": " << _CamLocation.y1 << ", "
                        << "\"CamLocation.x2\": " << _CamLocation.x2 << ", "
                        << "\"CamLocation.y2\": " << _CamLocation.y2 << "}";

                ss_calibration.flush();


                //cout << ss_calibration.str() << endl;
                //setSensors(ss_calibration.str());
            }



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

bool MainSingle::SendMesageToTCPServer(string ip, string port, Message &mesage) {
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
        return false;
    }

    if (mesage.num_cmd() == Message::CMD_GET_DATA_CAMERA || mesage.num_cmd() == Message::CMD_INIT_CAMERA) {
        ConvertDataToPoints(c);
    }

    return true;

    /*   
       _points_from_camera.clear();

       size_t read_len = c.getReadBodyLen();
       if (read_len > 0) {
           char *byff = (char*) malloc(read_len);
           c.getReadBodyData(byff);


           cout << endl;
           for (int i = 0; i < read_len / sizeof (C_Point); i++) {
               C_Point *p = (C_Point *) & byff[i * sizeof (C_Point)];
               C_Point cam;

               cam.alpha = p->alpha;
               cam.beta = p->beta;

               cout << i << " | alpha: " << cam.alpha << "| beta" << cam.beta << endl;

               //_points_from_camera.push_back(cam);
               //_points_from_camera.push_back(*p);
               _points_from_camera.push_back(*(C_Point *) & byff[i * sizeof (C_Point)]);
           }

           free(byff);
           //        cout << endl;
           //        for(auto i: _points_from_camera ){
           //            cout << " | alpha: " << i.alpha << "| beta" << i.beta  << endl;
           //        }
       }
    
     */

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

void MainSingle::ConvertDataToPoints(TCPClient &c) {

    //POINTS _points_from_camera;

    _points_from_camera.clear();
    size_t read_len = c.getReadBodyLen();
    if (read_len > 0) {
        char *byff = (char*) malloc(read_len);
        c.getReadBodyData(byff);


        //cout << endl;
        for (int i = 0; i < read_len / sizeof (C_Point); i++) {
            C_Point *p = (C_Point *) & byff[i * sizeof (C_Point)];
            C_Point cam;

            cam.alpha = p->alpha;
            cam.beta = p->beta;

            //cout << i << " | alpha: " << cam.alpha << "| beta" << cam.beta << endl;

            //_points_from_camera.push_back(cam);
            //_points_from_camera.push_back(*p);
            _points_from_camera.push_back(*(C_Point *) & byff[i * sizeof (C_Point)]);
        }

        free(byff);

        //memcpy(msg, read_msg_, read_len);

        //return read_len;
    }
}