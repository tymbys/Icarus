#include "Camera.h"

NaviMath::NaviMath()
{

}

NaviMath::CamLocation NaviMath::GetCameraLocation(double Alpha1, double Beta1, double Alpha2, double Beta2, double Base)
{
	NaviMath::CamLocation result;
	double K,Phi_prime,Phi2;

	K = (sin(Alpha1)*sin(Beta2)*sin(Alpha2+Beta1+Beta2))/
			(sin(Alpha2)*sin(Beta1)*sin(Alpha1+Beta1+Beta2));

	Phi2 = atan(sin(Beta1 + Beta2)/(K + cos(Beta1 + Beta2)));

	Phi_prime = Beta1 + Beta2 - Phi2;

	result.x1 =  Base * sin(Phi_prime) * sin(Phi2-Alpha1-Beta1-Beta2) / sin(Alpha1);
	result.y1 =  Base * sin(Phi_prime) * cos(Phi2-Alpha1-Beta1-Beta2) / sin(Alpha1);

	result.x2 = -Base * sin(Alpha2 + Phi2) * sin(Phi2) / sin(Alpha2);
	result.y2 = -Base * sin(Alpha2 + Phi2) * cos(Phi2) / sin(Alpha2);

	return result;

}
NaviMath::LedLocation NaviMath::GetLedLocation(double Phi1, double Phi2)
{
	NaviMath::LedLocation L={0};

	L.y1 = (Camera_Location.x2 - Camera_Location.x1 + Camera_Location.y1*tan(Phi1)-Camera_Location.y2*tan(Phi2))/
			(tan(Phi1)-tan(Phi2));
	L.x1 = Camera_Location.x1 + tan(Phi1)*(L.y1 - Camera_Location.y1 );
	return L;

}
void NaviMath::SetCamLocation(NaviMath::CamLocation L)
{
	NaviMath::Camera_Location = L;
}
