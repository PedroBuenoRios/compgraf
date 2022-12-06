#ifndef WINDOW_H
#define WINDOW_H
#include "objeto.h"

class Window
{
private:
    Vec4 _origem;
    int _largura, _altura;
    double _grausRotacionados;
public:
    Vec4 min, max;
    // Vetor Normal ao Plano
    Vec4 vpn;
    // View up
    Vec4 viewUp;
    // Vetores normais
    Vec4 norm_n, norm_u, norm_v;
    // Ponto de perspectiva
    Vec4 cop;
    Window();
    Window(const Vec4 &p, int l, int a);
    // Getters
    Vec4 getOrigem();
    int getLargura();
    int getAltura();
    double getGrausRotacionados();
    // Setters
    void calcularVetoresNormais();
    void calcularVPN();
    void setOrigem(const Vec4 &p);
    void setLargura(int l);
    void setAltura(int a);
    void setGrausRotacionados(double dg);
};

#endif // WINDOW_H
