#include "objeto.h"

Window::Window()
{
    _origem = Vec4();
    min = Vec4(-1,-1, 0);
    max = Vec4(1,1,0);
    _grausRotacionados = 0;
    _largura = 300;
    _altura = 300;
    cop = Vec4();
}

Window::Window(const Vec4 &p, int l, int a):_origem(p), _largura(l), _altura(a){
    min = Vec4(-1,-1,0);
    max = Vec4(1,1,0);
    cop = Vec4(0,0,0);
    _grausRotacionados = 0;
}

Vec4 Window::getOrigem(){
    return _origem;
}

int Window::getAltura(){
    return _altura;
}

int Window::getLargura(){
    return _largura;
}

void Window::setAltura(int a){
    _altura = a;
}

void Window::setLargura(int l){
    _largura = l;
}

void Window::setOrigem(const Vec4 &p){
    _origem = p;
}

double Window::getGrausRotacionados(){
    return _grausRotacionados;
}

void Window::setGrausRotacionados(double dg){
    _grausRotacionados = dg;
}

void Window::calcularVetoresNormais(){
    vpn = produtoVetorial(Vec4(_largura/2,0,0), Vec4(0,_altura/2,0));
    viewUp = Vec4(0,_altura/2,0);

    norm_n = vpn/comprimentoVetor(vpn);

    Vec4 prod = produtoVetorial(viewUp,vpn);
    norm_u  = prod/comprimentoVetor(prod);

    norm_v = produtoVetorial(norm_n, norm_u);
}
