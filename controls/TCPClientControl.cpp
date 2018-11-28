#include "TCPClientControl.h"

TCPClientControl::TCPClientControl() {
}

TCPClientControl::~TCPClientControl() {
}


POINTS TCPClientControl::ConvertDataToPoints(TCPClient &c) {

    POINTS _points;

    _points.clear();
    size_t read_len = c.getReadBodyLen();


    if (read_len > 0) {

        printf(" > read_len: %d\n", read_len);

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
            _points.push_back(*(C_Point *) & byff[i * sizeof (C_Point)]);
        }

        free(byff);

        //memcpy(msg, read_msg_, read_len);

        //return read_len;
    }
    
    return _points;
}



bool TCPClientControl::SendMesageToTCPServer(string ip, string port, Message &mesage) {
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
        _points_from_camera = ConvertDataToPoints(c);
    }

    return true;
}

POINTS TCPClientControl::GetPoints(){
    return _points_from_camera;
}