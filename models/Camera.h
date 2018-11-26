#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <vector>
#include <cmath>

struct C_Point {
    double alpha;
    double beta;
};


#define POINTS std::vector<C_Point>

class NaviMath {
public:

//    struct CamData {
//        double Fi_c1_c2,
//        double Fi_c1_l1,
//        double Fi_c1_l2,
//        double Fi_c2_c1,
//        double Fi_c2_l1,
//        double Fi_c2_l2,
//        double base
//    };

    struct CamLocation {
        double x1;
        double y1;
        double x2;
        double y2;
        double cam12; //угол под которым данная камера видит другую
        double cam21;
        double diff_cam_led;

    };

    struct LedLocation {
        double x1;
        double y1;
    };
    NaviMath();
    //CamLocation GetCameraLocation(double Alpha1, double Beta1, double Alpha2, double Beta2, double Base);
    CamLocation GetCameraLocation(double Fi_c1_c2, double Fi_c1_l1, double Fi_c1_l2, double Fi_c2_c1, double Fi_c2_l1, double Fi_c2_l2, double base);
    
//    NaviMath::CamLocation NaviMath::GetCameraLocation(const &CamData _cam_ata);
    LedLocation GetLedLocation(double Phi1, double Phi2);
    void SetCamLocation(CamLocation L);

    CamLocation Camera_Location = {0};

private:
    //CamLocation Camera_Location={0};


};

#endif /* CAMERA_H */

