#include "objeto.h"

Window::Window()
{
    _origem = Vec3();
    min = Vec3(-1,-1);
    max = Vec3(1,1);
    _grausRotacionados = 0;
    _largura = 300;
    _altura = 300;
}

Window::Window(const Vec3 &p, int l, int a):_origem(p), _largura(l), _altura(a){
    min = Vec3(-1,-1);
    max = Vec3(1,1);
    _grausRotacionados = 0;
}

Vec3 Window::getOrigem(){
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

void Window::setOrigem(const Vec3 &p){
    _origem = p;
}

double Window::getGrausRotacionados(){
    return _grausRotacionados;
}

void Window::setGrausRotacionados(double dg){
    _grausRotacionados = dg;
}

