#include "Message.h"

using namespace std;

const int ID = 0xDEEDBEEF; // 0x4F435653;

Message::Message() : _body_length(0) {
}

const char* Message::data() const {
    return data_;
}

char* Message::data() {
    return data_;
}

std::size_t Message::length() const {
    return _header_length + _body_length;
}

const char* Message::body() const {
    return data_ + _header_length;
}

char* Message::body() {
    return data_ + _header_length;
}

std::size_t Message::body_length() const {
    return _body_length;
}

void Message::body_length(std::size_t new_length) {
    _body_length = new_length;
    if (_body_length > _max_body_length)
        _body_length = _max_body_length;
}

int Message::num_cmd() {
    return _num_cmd;
}

bool Message::decode_header() {
    M_Header header;
    //char *p_header = (char *)&header;
    memcpy((char *) &header, data_, sizeof (M_Header));

    _body_length = header.DATA_LEN;
    if (_body_length > _max_body_length) {
        _body_length = 0;
        _num_cmd = -1;
        return false;
    }

    //_num_cmd = header.TYPE;

    cout << "decode_header()" << endl;
    cout << "header.ID:\t" << header.ID << endl;
    //cout << "header.NumCmd:\t" << header.TYPE << endl;
    cout << "header.DataSize:\t" << header.DATA_LEN << endl << endl;

    return true;

    /*char header[header_length + 1] = "";
    std::strncat(header, data_, header_length);
    body_length_ = std::atoi(header);
    if (body_length_ > max_body_length)
    {
      body_length_ = 0;
      return false;
    }
    return true;
     */
}

void Message::encode_header() {

    //if (body_length_ < 0) return;

    char *p;

    M_Header header;
    header.ID = ID;
    //header.TYPE = _num_cmd;
    header.DATA_LEN = static_cast<int> (_body_length);

    p = (char *) &header;

    memcpy(data_, p, sizeof (header));




    /*
   char header[header_length + 1] = "";
   std::sprintf(header, "%4d", static_cast<int>(body_length_));
   std::memcpy(data_, header, header_length);
     */
}

bool Message::decode_body() {
    //if(num_cmd_ > sizeof(bodies_length)/sizeof(bodies_length[0])) return false;

    switch (_num_cmd) {
        case CMD_NULL:
        {
            break;
        }
            //        case CMD_RECT_DETECT:
            //        {
            //            if (body_length_ > 0) {
            //                M_RectDetect * p_rDetect;
            //                p_rDetect = (M_RectDetect *) body();
            //
            //                memcpy((char *) rectDetect.ID, (char *) p_rDetect->ID, sizeof (rectDetect.ID));
            //                rectDetect.height = p_rDetect->height;
            //                rectDetect.width = p_rDetect->width;
            //                rectDetect.x = p_rDetect->x;
            //                rectDetect.y = p_rDetect->y;
            //
            //
            //                cout << "CMD_RECT_DETECT" << endl;
            //                cout << "ID:\t" << rectDetect.ID << endl;
            //                cout << "x:\t" << rectDetect.x << endl;
            //                cout << "y:\t" << rectDetect.y << endl;
            //                cout << "width:\t" << rectDetect.width << endl;
            //                cout << "height:\t" << rectDetect.height << endl << endl;
            //            }
            //
            //            break;
            //        }
            //        case CMD_CURENT_STATE:
            //        {
            //            if (body_length_ > 0) {
            //                M_CurentState *p_cState;
            //                p_cState = (M_CurentState *) body();
            //
            //                int sizeObj = sizeof (M_Object) * p_cState->amountObjects;
            //                //if (sizeObj == header->DataSize) {                    
            //                //memcpy(objects, p_cState->objects, sizeof (M_Object) * p_cState->amountObjects);
            //                memcpy((char *) curentState.objects, (char *) p_cState->objects, sizeof (M_Object) * p_cState->amountObjects);
            //                curentState.amountObjects = p_cState->amountObjects;
            //
            //                //curentState.objects = p_cState->objects;
            //                //} //else return -1;
            //
            //                cout << "CMD_CURENT_STATE" << endl;
            //                cout << "amountObjects:\t" << curentState.amountObjects << endl;
            //                for (int i = 0; i < curentState.amountObjects; i++) {
            //                    cout << "x:\t" << curentState.objects[i].x << endl;
            //                    cout << "y:\t" << curentState.objects[i].y << endl;
            //                    cout << "width:\t" << curentState.objects[i].width << endl;
            //                    cout << "height:\t" << curentState.objects[i].height << endl;
            //                    cout << "status:\t" << curentState.objects[i].status << endl << endl;
            //                }
            //
            //            }
            //            break;
            //        }
            //
            //        case CMD_PORT_STREAMING:
            //        {
            //            if (body_length_ > 0) {
            //                M_PortsStreaming *p_pStreaming;
            //                p_pStreaming = (M_PortsStreaming *) body();
            //
            //                portsStreaming.inPort = p_pStreaming->inPort;
            //                portsStreaming.outPort = p_pStreaming->outPort;
            //
            //                cout << "CMD_PORT_STREAMING" << endl;
            //                cout << "in: " << portsStreaming.inPort << endl;
            //                cout << "out: " << portsStreaming.outPort << endl << endl;
            //                ;
            //            }
            //            break;
            //        }
            //        case CMD_START_STREAMING:
            //        {
            //            if (body_length_ > 0) {
            //                M_PortsStreaming *p_pStreaming;
            //                p_pStreaming = (M_PortsStreaming *) body();
            //
            //                portsStreaming.inPort = p_pStreaming->inPort;
            //                portsStreaming.outPort = p_pStreaming->outPort;
            //
            //                cout << "CMD_START_STREAMING" << endl;
            //                cout << "in: " << portsStreaming.inPort << endl;
            //                cout << "out: " << portsStreaming.outPort << endl << endl;
            //                ;
            //            }
            //
            //            break;
            //        }

    }

    return true;
}

bool Message::encode_body() {

    //if(num_cmd_ > sizeof(bodies_length)/sizeof(bodies_length[0])) return false;  
    if (_body_length <= 0) return false;

    //body_length_ = 0;
    char *p = NULL;

    switch (_num_cmd) {
        case CMD_NULL:
        {
            break;
        }
        
        case CMD_GET_DATA_BEACON:
        {
            p = (char *) &_beacon;
            //body_length_ = sizeof(rectDetect);

            break;
        }
        
            //        case CMD_RECT_DETECT:
            //        {
            //            p = (char *) &rectDetect;
            //            //body_length_ = sizeof(rectDetect);
            //
            //            break;
            //        }
            //        case CMD_CURENT_STATE:
            //        {
            //            p = (char *) &curentState;
            //            //body_length_ = sizeof(M_Object) * curentState.amountObjects + sizeof(int);
            //
            //            break;
            //        }
            //
            //        case CMD_PORT_STREAMING:
            //        {
            //            p = (char *) &portsStreaming;
            //            //body_length_ = sizeof(portsStreaming);
            //
            //            break;
            //        }
            //        case CMD_START_STREAMING:
            //        {
            //            p = (char *) &portsStreaming;
            //
            //            break;
            //        }

    }


    if (p != NULL && _body_length > 0)
        memcpy(&data_[sizeof (M_Header)], p, _body_length);
    else return false;

    return true;

}

void Message::setHeader(NUM_CMD num_cmd) {
    //header = _header;
    _num_cmd = num_cmd; //header.NumCmd;
    _body_length = 0;



    switch (_num_cmd) {
        case CMD_NULL:
        {
            break;
        }

        case CMD_GET_DATA_BEACON:
        {
            _body_length = sizeof (_beacon);

            break;
        }
        
        case CMD_INIT_CAMERA:
        {
            _body_length = 0;

            break;
        }
    }

    /*
    switch(num_cmd_){
          case CMD_NULL:
          {
              break;
          }          
          case CMD_RECT_DETECT:
          {
              body_length_ = sizeof(rectDetect);
              
              break;
          }
          case CMD_CURENT_STATE:
          {
              body_length_ = sizeof(M_Object) * curentState.amountObjects + sizeof(int);
              
              break;
          }

          case CMD_PORT_STREAMING:
          {
              body_length_ = sizeof(portsStreaming);
              
              break;
          }
          case CMD_START_STREAMING:
          {
                           
              break;
          }

      }
     */
}

//bool makeData(NUM_CMD num_cmd, M_RectDetect _rectDetect) {
//    setHeader(num_cmd);
//    setRectDetect(_rectDetect);
//
//    encode_header();
//    return encode_body();
//}
//
//bool makeData(NUM_CMD num_cmd, M_CurentState _curentState) {
//    setHeader(num_cmd);
//    setCurentState(_curentState);
//
//    encode_header();
//    return encode_body();
//}
//
//bool makeData(NUM_CMD num_cmd, M_PortsStreaming _portsStreaming) {
//    setHeader(num_cmd);
//    setPortsStreaming(_portsStreaming);
//
//    encode_header();
//    return encode_body();
//}
//
//bool makeData(NUM_CMD num_cmd) {
//    setHeader(num_cmd);
//
//    encode_header();
//    return true;
//}

bool Message::setBody(Beacon body) {
    _beacon = body;
    setHeader(CMD_GET_DATA_BEACON);
    encode_header();
    return encode_body();
}

bool Message::setBody(C_Point body){
    _camera = body;
    setHeader(CMD_GET_DATA_BEACON);
    encode_header();
    return encode_body();
}


C_Point Message::getBody() {
    C_Point cam;
    memcpy(&cam, data_, sizeof(C_Point));
    return cam;
}

int Message::getNumCMD() {
    return _num_cmd;
}




