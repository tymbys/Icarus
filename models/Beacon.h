#ifndef BEACON_H
#define BEACON_H

#include <stdint.h>

//typedef union {
//#define LEN 2
//    struct {
//        //uint8_t b0;
//        //uint8_t b1;
//        //uint8_t b2;
//        //uint8_t b3;
//        //uint8_t b4;
//        //uint8_t b5;
//        //uint8_t b6;
//        //uint8_t b7;
//        
//        uint16_t mNumber;
//        uint16_t len;
//        uint8_t data[LEN];
//        
//    } byte;
//
//    uint8_t data[8];
//
//} BeaconHeader;

struct Beacon{
    uint8_t data[2];
} ;


#endif /* BEACON_H */

