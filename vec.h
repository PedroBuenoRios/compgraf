#pragma once
#include <QtMath>

class Vec2{
public:
    Vec2(double x, double y): x(x), y(y){

    }
    Vec2(){
        this->x = 0.0;
        this->y = 0.0;
    }

    Vec2 operator+(const Vec2 &soma) const{
        return Vec2(soma.x + x, soma.y+y);
    }

    Vec2 operator-(const Vec2 &sub) const{
        return Vec2(x - sub.x, y - sub.y);
    }

    Vec2 operator*(const Vec2 &mult) const{
        return Vec2(x*mult.x, y*mult.y);
    }
    double x, y;
};

class Vec3{

public:
    double x, y, z;

    Vec3(double x, double y, double z = 1): x(x), y(y), z(z){

    }
    Vec3(): x(0.0), y(0.0), z(1.0){
    }
    // Soma entre cada elemento do vetor, [x + x0, y + y0, z + z0]
    Vec3 operator+(const Vec3 &soma) const{
        return Vec3(x+soma.x, y+soma.y, z + soma.z);
    }
    // Subtração entre cada elemento do vetor, [x - x0, y - y0, z - z0]
    Vec3 operator-(const Vec3 &sub) const{
        return Vec3(x - sub.x, y - sub.y, z - sub.z );
    }
    // Produto interno entre vetores w = x.x0 + y.y0 + z.z0
    double operator*(const Vec3 &mult) const{
        return (x*mult.x + y*mult.y + z*mult.z);
    }

    void setValueAt(int col, double v){
        switch(col){
        case 0:
            x = v;
            break;
        case 1:
            y = v;
            break;
        case 2:
            z = v;
            break;
        default:
            break;
        }
    }
    double getValueAt(int col){
        switch(col){
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
         default:
            return 0;
        }
    }
};

class Mat3x3{
   private:
    Vec3 coluna[3];
    Vec3 linha[3];
public:

    Mat3x3(){
        for(int i = 0; i <3; i++)
        {
            linha[i] = Vec3(0,0,0);
            coluna[i] = Vec3(0,0,0);
        }
        linha[0].x = 1;
        linha[1].y = 1;
        linha[2].z = 1;

        coluna[0].x = 1;
        coluna[1].y = 1;
        coluna[2].z = 1;
    }

    Mat3x3(double m[][3]){
        for(int i = 0; i < 3; i++){
                linha[i] = Vec3(m[i][0], m[i][1], m[i][2]);
                coluna[i] = Vec3(m[0][i], m[1][i], m[2][i]);
        }
    }

    void setValueAt(int lin, int col, double v){
        linha[lin].setValueAt(col, v);
        coluna[col].setValueAt(lin, v);
    }

    void setColuna(const Vec3 &v, int col){
        coluna[col].x = v.x;
        coluna[col].y = v.y;
        coluna[col].z = v.z;

        linha[0].setValueAt(col, v.x);
        linha[1].setValueAt(col, v.y);
        linha[2].setValueAt(col, v.z);
    }

    Vec3 getLinha(int lin){
        return linha[lin];
    }

    Vec3 getColuna(int col){
        return coluna[col];
    }

    Mat3x3 operator*(const Mat3x3 &m)const{
        double aux[3][3];
        // Linha
        for(int i = 0; i < 3; i++){
            // Coluna
            for(int k = 0; k < 3;k++)
            {
                aux[i][k] = linha[i] * m.coluna[k];
            }
        }
        return Mat3x3(aux);
    }
};

//class Transformes{
//public:
//    Mat3x3 matriz;
//    Transformes(){
//        matriz = Mat3x3();
//    }

//    void rotacionar(double dg){
//        double rd = qDegreesToRadians(dg);
//        double m[3][3] = {{qCos(rd),-qSin(rd),0},
//                          {qSin(rd), qCos(rd), 0},
//                          {0,0,1}};
//        Mat3x3 aux(m);
//        matriz = matriz*aux;
//    }

//    void transladar(const Vec3 &v){
//        double m[3][3] = {{1, 0, v.x}, {0, 1, v.y}, {0 ,0 , v.z}};
//        Mat3x3 aux(m);
//        matriz = matriz*aux;
//    }

//    void escalonar(const Vec3 &v){
//        double m[3][3] = {{v.x, 0, 0}, {0, v.y, 0}, {0, 0, v.z}};
//        Mat3x3 aux(m);
//        matriz = matriz*aux;
//    }
//};

class Vec4{

public:
    double x, y, z, w;

    Vec4(double x, double y, double z, double w = 1): x(x), y(y), z(z), w(w){

    }
    Vec4(): x(0.0), y(0.0), z(0.0), w(1.0){
    }
    // Soma entre cada elemento do vetor, [x + x0, y + y0, z + z0]
    Vec4 operator+(const Vec4 &soma) const{
        return Vec4(x + soma.x, y + soma.y, z + soma.z, w + soma.w);
    }
    // Subtração entre cada elemento do vetor, [x - x0, y - y0, z - z0]
    Vec4 operator-(const Vec4 &sub) const{
        return Vec4(x - sub.x, y - sub.y, z - sub.z, w - sub.w );
    }
    // Produto interno entre vetores w = x.x0 + y.y0 + z.z0
    double operator*(const Vec4 &mult) const{
        return (x*mult.x + y*mult.y + z*mult.z + w*mult.w);
    }

    Vec4 operator*(double escalar){
        return Vec4(x*escalar, y*escalar, z*escalar);
    }

    Vec4 operator/(double escalar){
        return operator*(1/escalar);
    }

    void setValueAt(int col, double v){
        switch(col){
        case 0:
            x = v;
            break;
        case 1:
            y = v;
            break;
        case 2:
            z = v;
            break;
        case 3:
            w = v;
            break;
        default:
            break;
        }
    }
    double getValueAt(int col){
        switch(col){
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        case 3:
            return w;
         default:
            return 0;
        }
    }
};

class Mat4x4{
   private:
    Vec4 coluna[4];
    Vec4 linha[4];
public:

    Mat4x4(){
        for(int i = 0; i <4; i++)
        {
            linha[i] = Vec4(0,0,0,0);
            coluna[i] = Vec4(0,0,0,0);
        }
        linha[0].x = 1;
        linha[1].y = 1;
        linha[2].z = 1;
        linha[3].w = 1;

        coluna[0].x = 1;
        coluna[1].y = 1;
        coluna[2].z = 1;
        coluna[3].w = 1;
    }

    Mat4x4(double m[4][4]){
        for(int i = 0; i < 4; i++){
                linha[i] = Vec4(m[i][0], m[i][1], m[i][2], m[i][3]);
                coluna[i] = Vec4(m[0][i], m[1][i], m[2][i], m[3][i]);
        }
    }

    void setValueAt(int lin, int col, double v){
        linha[lin].setValueAt(col, v);
        coluna[col].setValueAt(lin, v);
    }

    void setColuna(const Vec4 &v, int col){
        coluna[col].x = v.x;
        coluna[col].y = v.y;
        coluna[col].z = v.z;
        coluna[col].w = v.w;

        linha[0].setValueAt(col, v.x);
        linha[1].setValueAt(col, v.y);
        linha[2].setValueAt(col, v.z);
        linha[3].setValueAt(col, v.w);
    }

    Vec4 getLinha(int lin){
        return linha[lin];
    }

    Vec4 getColuna(int col){
        return coluna[col];
    }

    Mat4x4 operator*(const Mat4x4 &m)const{
        double aux[4][4];
        // Linha
        for(int i = 0; i < 4; i++){
            // Coluna
            for(int k = 0; k < 4;k++)
            {
                aux[i][k] = linha[i] * m.coluna[k];
            }
        }
        return Mat4x4(aux);
    }
};

enum EIXO{
    X = 0,
    Y,
    Z,
    N
};

class Transformes{
public:

    Mat4x4 matriz;
    Transformes(){
        matriz = Mat4x4();
    }

    void rotacionar(double dg, EIXO eixo){
        double rd = qDegreesToRadians(dg);
        Mat4x4 aux = Mat4x4();
        switch(eixo){
        case EIXO::X:
            aux.setValueAt(1, 1, qCos(rd));
            aux.setValueAt(1, 2, -qSin(rd));
            aux.setValueAt(2, 1, qSin(rd));
            aux.setValueAt(2, 2, qCos(rd));
            matriz = matriz * aux;
            break;
        case EIXO::Y:
           // double _coseno = -(p2.x - p1.x)/(qSqrt(qPow(p2.z - p1.z,2) + qPow(p2.x - p1.x,2)));
            aux.setValueAt(0, 0, qCos(rd));
            aux.setValueAt(0, 2, qSin(rd));
            aux.setValueAt(2, 0, -qSin(rd));
            aux.setValueAt(2, 2, qCos(rd));
            matriz = matriz * aux;
            break;
        case EIXO::Z:
            aux.setValueAt(0, 0, qCos(rd));
            aux.setValueAt(0, 1, -qSin(rd));
            aux.setValueAt(1, 0, qSin(rd));
            aux.setValueAt(1, 1, qCos(rd));
            matriz = matriz*aux;
            break;
        default:
            break;
        }
    }

    void transladar(const Vec4 &v){
        double m[4][4] = {{1, 0, 0, v.x},
                          {0, 1, 0, v.y},
                          {0, 0, 1, v.z},
                          {0, 0, 0,   1}};
        Mat4x4 aux(m);
        matriz = matriz*aux;
    }

    void escalonar(const Vec4 &v){
        double m[4][4] = {{v.x, 0, 0, 0},
                          {0, v.y, 0, 0},
                          {0, 0, v.z, 0},
                          {0, 0, 0,   1}};
        Mat4x4 aux(m);
        matriz = matriz*aux;
    }

    template<class W>
    void WCparaVCS(W &window){
        window.calcularVetoresNormais();
        Mat4x4 m_per;
        m_per.setValueAt(3,2, 1/qAbs(window.getOrigem().z));
        m_per.setValueAt(3,3,0.0);
        matriz = matriz*m_per;

        double m_rot[4][4] = {{window.norm_u.x, window.norm_u.y, window.norm_u.z, -window.getOrigem().x},
                          {window.norm_v.x, window.norm_v.y, window.norm_v.z, -window.getOrigem().y},
                          {window.norm_n.x, window.norm_n.y, window.norm_n.z, -window.getOrigem().z},
                          {              0,               0,               0,                1}};
        Mat4x4 aux(m_rot);
        matriz = matriz*aux;
        matriz.setValueAt(3,3,1);
    }
};

double comprimentoVetor(Vec4 v);
Vec4 produtoVetorial(Vec4 u, Vec4 v);

Vec4 operator*(Mat4x4 m, Vec4 v);
Vec4 operator*(Vec4 v, Mat4x4 m);

Vec3 operator*(Mat3x3 m, Vec3 v);
Vec3 operator*(Vec3 v, Mat3x3 m);
