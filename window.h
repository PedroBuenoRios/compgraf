#ifndef WINDOW_H
#define WINDOW_H
#include "objeto.h"

class Window
{
private:
    Vec3 _origem;
    int _largura, _altura;
    double _grausRotacionados;
public:
    Vec3 min;
    Vec3 max;
    Window();
    Window(const Vec3 &p, int l, int a);
    // Getters
    Vec3 getOrigem();
    int getLargura();
    int getAltura();
    double getGrausRotacionados();
    // Setters
    void setOrigem(const Vec3 &p);
    void setLargura(int l);
    void setAltura(int a);
    void setGrausRotacionados(double dg);
};

#endif // WINDOW_H
