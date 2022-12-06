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

Vec4 operator*(Mat4x4 m ,Vec4 v){
    Vec4 aux;
    aux.x = v * m.getLinha(0);
    aux.y = v * m.getLinha(1);
    aux.z = v * m.getLinha(2);
    aux.w = v * m.getLinha(3);
    return aux;
}

Vec4 operator*(Vec4 v ,Mat4x4 m){
    Vec4 aux;
    aux.x = v * m.getLinha(0);
    aux.y = v * m.getLinha(1);
    aux.z = v * m.getLinha(2);
    aux.w = v * m.getLinha(3);
    return aux;
}

double comprimentoVetor(Vec4 v){
    return qSqrt(v.x*v.x + v.y*v.y +v.z*v.z);
}

Vec4 produtoVetorial(Vec4 u, Vec4 v){
    Vec4 prod;
    prod.x = u.y*v.z - u.z*v.y;
    prod.y = u.z*v.x - u.x*v.z;
    prod.z = u.x*v.y - u.y*v.x;
//    double m[3][3] = {{1, 1, 1},
//                      {u.x, u.y, u.x},
//                      {v.x, v.y, v.z}};
//    determ(m, 3);
    return prod;
}

double determ(double a[3][3],double n) {
  double det=0, temp[3][3];
  int p, h, k, i, j;
  if(n==1) {
    return a[0][0];
  } else if(n==2) {
    det=(a[0][0]*a[1][1]-a[0][1]*a[1][0]);
    return det;
  } else {
    for(p=0;p<n;p++) {
      h = 0;
      k = 0;
      for(i=1;i<n;i++) {
        for( j=0;j<n;j++) {
          if(j==p) {
            continue;
          }
          temp[h][k] = a[i][j];
          k++;
          if(k==n-1) {
            h++;
            k = 0;
          }
        }
      }
      det=det+a[0][p]*pow(-1,p)*determ(temp,n-1);
    }
    return det;
  }
}
