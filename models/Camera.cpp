#include "Camera.h"

using namespace std;

double angle(double x1, double y1, double x2, double y2)
//The angle of rorartion form vector (x1;y1) to (x2; y2)
//Positive rotate is contatry to clock
{
    double fi = atan2(y2, x2) - atan2(y1, x1);
    if (fi < -M_PI) fi += 2 * M_PI;
    if (fi > M_PI) fi -= 2 * M_PI;
    return fi;
}

void Rotate(double & x, double & y, double fi)
//Rotate vector (x;y) conrary to clock by the angle fi
{
    double x1 = x * cos(fi) - y * sin(fi);
    double y1 = x * sin(fi) + y * cos(fi);
    x = x1;
    y = y1;
}

NaviMath::NaviMath() {

}

//NaviMath::CamLocation NaviMath::GetCameraLocation(double Alpha1, double Beta1, double Alpha2, double Beta2, double Base)
//{
//	NaviMath::CamLocation result;
//	double K,Phi_prime,Phi2;
//
//	K = (sin(Alpha1)*sin(Beta2)*sin(Alpha2+Beta1+Beta2))/
//			(sin(Alpha2)*sin(Beta1)*sin(Alpha1+Beta1+Beta2));
//
//	Phi2 = atan(sin(Beta1 + Beta2)/(K + cos(Beta1 + Beta2)));
//
//	Phi_prime = Beta1 + Beta2 - Phi2;
//
//	result.x1 =  Base * sin(Phi_prime) * sin(Phi2-Alpha1-Beta1-Beta2) / sin(Alpha1);
//	result.y1 =  Base * sin(Phi_prime) * cos(Phi2-Alpha1-Beta1-Beta2) / sin(Alpha1);
//
//	result.x2 = -Base * sin(Alpha2 + Phi2) * sin(Phi2) / sin(Alpha2);
//	result.y2 = -Base * sin(Alpha2 + Phi2) * cos(Phi2) / sin(Alpha2);
//
//	return result;
//
//}

//NaviMath::CamLocation NaviMath::GetCameraLocation(double Alpha1, double Beta1, double Alpha2, double Beta2, double Base) {
//    NaviMath::CamLocation result;
//    double K, Phi_prime, Phi2;
//
//    K = (sin(Alpha1) * sin(Beta2) * sin(Alpha2 + Beta1 + Beta2)) /
//            (sin(Alpha2) * sin(Beta1) * sin(Alpha1 + Beta1 + Beta2));
//
//    Phi2 = atan(sin(Beta1 + Beta2) / (K + cos(Beta1 + Beta2)));
//
//    Phi_prime = Beta1 + Beta2 - Phi2;
//
//    result.x1 = Base * sin(Phi_prime) * sin(Phi2 - Alpha1 - Beta1 - Beta2) / sin(Alpha1);
//    result.y1 = Base * sin(Phi_prime) * cos(Phi2 - Alpha1 - Beta1 - Beta2) / sin(Alpha1);
//
//    result.x2 = -Base * sin(Alpha2 + Phi2) * sin(Phi2) / sin(Alpha2);
//    result.y2 = -Base * sin(Alpha2 + Phi2) * cos(Phi2) / sin(Alpha2);
//
//    return result;
//
//}
/*
 * Fi_c1_c2 - угол с кам1 на кам2
 * Fi_c1_l1 - угол с кам1 на лед1
 * Fi_c1_l2 - угол с кам1 на лед1
 * Fi_c2_c1 - угол с кам2 на кам1
 * Fi_c2_l1
 * Fi_c2_l2
 *  double & X_c1, double & Y_c1, double & X_c2, double & Y_c2 - координаны камеры
 */

NaviMath::CamLocation NaviMath::GetCameraLocation(double Fi_c1_c2, double Fi_c1_l1, double Fi_c1_l2,
        double Fi_c2_c1, double Fi_c2_l1, double Fi_c2_l2, double base)
//CALCULATION OF CAMERA COORDINATES BY THE GIVEN ANGLES
{
    NaviMath::CamLocation result;
    result.cam12 = Fi_c1_c2;
    result.cam21 = Fi_c2_c1;
    
    //calculate relative cameras angles
    double Fi11 = Fi_c1_l1 - Fi_c1_c2;
    double Fi12 = Fi_c1_l2 - Fi_c1_c2;
    double Fi21 = Fi_c2_l1 - Fi_c2_c1 + M_PI;
    double Fi22 = Fi_c2_l2 - Fi_c2_c1 + M_PI;
    cout << "Cam1 angle to LED1 = " << Fi11 * 180 / M_PI << endl;
    cout << "Cam1 angle to LED2 = " << Fi12 * 180 / M_PI << endl;
    cout << "Cam2 angle to LED1 = " << Fi21 * 180 / M_PI << endl;
    cout << "Cam2 angle to LED2 = " << Fi22 * 180 / M_PI << endl;

    //========= Calculate cam coordinates Xcc1 Ycc1 Xcc2 Ycc2 from the angles
    //calc M
    double ct11 = 1 / tan(Fi11);
    double ct12 = 1 / tan(Fi12);
    double ct21 = 1 / tan(Fi21);
    double ct22 = 1 / tan(Fi22);
    double Mcoef = pow(1 / (ct22 - ct12) - 1 / (ct21 - ct11), 2) +
            pow(ct12 / (ct12 - ct22) - ct11 / (ct11 - ct21), 2);
    double M = base / sqrt(Mcoef);

    //Calculate visible LEDs coordinates in cameras SO
    double xll1 = M / (ct21 - ct11);
    double yll1 = M * ct11 / (ct11 - ct21);
    double xll2 = M / (ct22 - ct12);
    double yll2 = M * ct12 / (ct12 - ct22);

    //SO rotation angle
    double fi = atan2(xll2 - xll1, yll2 - yll1);
    
    result.diff_cam_led = fi;

    //Cams coordinates
    double X_c1 = -xll1;
    double Y_c1 = -yll1;
    double X_c2 = -xll1;
    double Y_c2 = M - yll1;
    
    
    Rotate(X_c1, Y_c1, fi);
    Rotate(X_c2, Y_c2, fi);
    
    result.x1 = X_c1;
    result.y1 = Y_c1;
    result.x2 = X_c2;
    result.y2 = Y_c2;
    
    return result;
    
}


//NaviMath::LedLocation NaviMath::GetLedLocation(double Phi1, double Phi2)
//{
//	NaviMath::LedLocation L={0};
//
//	L.y1 = (Camera_Location.x2 - Camera_Location.x1 + Camera_Location.y1*tan(Phi1)-Camera_Location.y2*tan(Phi2))/
//			(tan(Phi1)-tan(Phi2));
//	L.x1 = Camera_Location.x1 + tan(Phi1)*(L.y1 - Camera_Location.y1 );
//	return L;
//
//}

NaviMath::LedLocation NaviMath::GetLedLocation(double Phi1, double Phi2) {
    NaviMath::LedLocation L = {0};

    L.y1 = (Camera_Location.x2 - Camera_Location.x1 + Camera_Location.y1 * tan(Phi1) - Camera_Location.y2 * tan(Phi2)) /
            (tan(Phi1) - tan(Phi2));
    L.x1 = Camera_Location.x1 + tan(Phi1)*(L.y1 - Camera_Location.y1);
    return L;

}

void NaviMath::SetCamLocation(NaviMath::CamLocation L) {
    NaviMath::Camera_Location = L;
}
