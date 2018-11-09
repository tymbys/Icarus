#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "Beacon.h"
#include "Camera.h"

class Message {
public:
#pragma pack(push, 1)

    struct M_Header {
        uint32_t ID; // идентификатор: 4 байта = ‘OCVS’ = 0x4F435653
        //int TYPE;
        uint16_t DATA_LEN;

    };
#pragma pack(pop)

    enum {
        _header_length = sizeof (M_Header)
    };

    enum {
        _max_body_length = 512
    };

    enum NUM_CMD {
        CMD_NULL = 0,
        CMD_GET_DATA_BEACON = 1,
        CMD_GET_DATA_CAMERA = 2,
        CMD_SET_DATA_BEACON = 16,
        CMD_SET_DATA_CAMERA = 17,
        CMD_INIT_CAMERA = 32,
    };

    Message();
    const char* data() const;
    char* data();
    std::size_t length() const;
    const char* body() const;
    char* body();
    std::size_t body_length() const;
    void body_length(std::size_t new_length);
    int num_cmd();
    bool decode_header();
    void encode_header();
    bool decode_body();
    bool encode_body();

    void setHeader(NUM_CMD num_cmd);
    bool setBody(Beacon body);
    bool setBody(Camera body);

    //void setBody(Camera body);
    Camera getBody();


    //    bool makeData(NUM_CMD num_cmd, M_RectDetect _rectDetect);
    //    bool makeData(NUM_CMD num_cmd, M_CurentState _curentState);
    //    bool makeData(NUM_CMD num_cmd, M_PortsStreaming _portsStreaming);
    //    bool makeData(NUM_CMD num_cmd);
    int getNumCMD();

private:
    char data_[_header_length + _max_body_length];
    std::size_t _body_length;
    int _num_cmd;

    M_Header header;
    Beacon _beacon;
    Camera _camera;
};

#endif