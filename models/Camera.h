#ifndef CAMERA_H
#define CAMERA_H

#include <vector>
#include <cmath>

struct C_Point{
    double alpha;
    double beta; 
};


#define POINTS std::vector<C_Point>

class NaviMath
{
public:
	struct CamLocation
	{
		double x1;
		double y1;
		double x2;
		double y2;
	};
	struct LedLocation
	{
		double x1;
		double y1;
	};
	NaviMath();
	CamLocation GetCameraLocation(double Alpha1, double Beta1, double Alpha2, double Beta2, double Base);
	LedLocation GetLedLocation(double Phi1, double Phi2);
	void		SetCamLocation(CamLocation L);
private:
	CamLocation Camera_Location={0};


};

#endif /* CAMERA_H */

