#include <OriginLLA.hpp>
#include <Angle.hpp>


OriginLLA::OriginLLA():undefined(true),latitude(0.0),longitude(0.0),altitude(0.0),sinOriginLat(0.0),cosOriginLat(1.0){}

OriginLLA::~OriginLLA(){}

void OriginLLA::Update(double lat, double lon, double alt){
    if(undefined){
        undefined = false;
        this->latitude = (lat < -1.570796326794896619) ? -1.570796326794896619 : ((lat > 1.570796326794896619) ? 1.570796326794896619 : lat);
        this->longitude = lon;
        this->altitude = alt;
        this->sinOriginLat = sin(this->latitude());
        this->cosOriginLat = cos(this->latitude());
    }
}

void OriginLLA::Get(double& lat, double& lon, double& alt){
    lat = this->latitude();
    lon = this->longitude();
    alt = this->altitude;
}

void OriginLLA::LLA2NED(glm::dvec3& xyz, glm::dvec3 lla){
    // Ensure correct inputs
    Angle aLat(lla.x);
    Angle aLon(lla.y);
    double lat = aLat();
    double lon = aLon();
    lat = (lat < -1.570796326794896619) ? -1.570796326794896619 : ((lat > 1.570796326794896619) ? 1.570796326794896619 : lat);
    double alt = lla.z;

    // Constants according to WGS84
    const double a = 6378137.0;
    const double e2 = 0.00669438000426092; // (e2 = 1 - (b/a)^2) with (b = 6356752.3142)

    // Some useful definitions
    double dphi = lat - this->latitude();
    double dlam = lon - this->longitude();
    double dh = alt - this->altitude;
    double cp = this->cosOriginLat;
    double sp = this->sinOriginLat;
    double tmp1 = sqrt(1.0 - e2*sp*sp);
    double tmp3 = (a * (1.0 - e2)) / (tmp1 * tmp1 * tmp1) + this->altitude;
    double atmp1 = a / tmp1;
    double dphi2 = dphi * dphi;
    double dlam2 = dlam * dlam;
    double cpcp = cp * cp;
    double spcp = sp * cp;
    double cpdlam = cp * dlam;
    double spdphi = sp * dphi;

    // Transformations
    xyz.x = tmp3*dphi + 1.5*spcp*a*e2*dphi2 + sp*dh*spdphi + 0.5*spcp*(atmp1 + this->altitude)*dlam2;
    xyz.y = (atmp1 + this->altitude)*cpdlam - tmp3*spdphi*dlam + cpdlam*dh;
    xyz.z = -(dh - 0.5*(a - 1.5*a*e2*cpcp + 0.5*a*e2 + this->altitude)*dphi2 - 0.5*cpcp*(atmp1 - this->altitude)*dlam2);
}

