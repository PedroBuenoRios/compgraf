#include "vec.h"

Vec3 operator*(Vec3 n,Mat3x3 m){
    Vec3 aux;
    aux.x = n * m.getLinha(0);
    aux.y = n * m.getLinha(1);
    aux.z = n * m.getLinha(2);
    return aux;
}

Vec3 operator*(Mat3x3 m,Vec3 n){
    Vec3 aux;
    aux.x = n * m.getLinha(0);
    aux.y = n * m.getLinha(1);
    aux.z = n * m.getLinha(2);
    return aux;
}
