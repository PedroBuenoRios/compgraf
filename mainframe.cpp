#include "mainframe.h"
#include <QDebug>
mainFrame::mainFrame()
{
}

mainFrame::mainFrame(QWidget *parent): QFrame{parent}{
    window = new Window(Vec4(0,0,60),108, 45);
    _clicks = 0;
    displayFile = QList<Objeto*>();
    Objeto *q1 = new Objeto(TipoObjeto::OBJ_POLIGONO, "Casa 1", Ponto(Vec4(0, 0,0)));
    QList<Ponto> listaAux;
    // retangulo
    listaAux.append(Ponto(Vec4(0,0,0)));
    listaAux.append(Ponto(Vec4(50,0,0)));
    listaAux.append(Ponto(Vec4(50,70,0)));
    listaAux.append(Ponto(Vec4(0, 70,0)));
    // retangulo
    listaAux.append(Ponto(Vec4(10,0,0)));
    listaAux.append(Ponto(Vec4(20,0,0)));
    listaAux.append(Ponto(Vec4(20,20,0)));
    listaAux.append(Ponto(Vec4(10,20,0)));
    // retangulo
    listaAux.append(Ponto(Vec4(30,20,0)));
    listaAux.append(Ponto(Vec4(40,20,0)));
    listaAux.append(Ponto(Vec4(40,30,0)));
    listaAux.append(Ponto(Vec4(30,30,0)));
    // triangulo
    listaAux.append(Ponto(Vec4(50,70,0)));
    listaAux.append(Ponto(Vec4(0, 70,0)));
    listaAux.append(Ponto(Vec4(25,90,0)));
    _descRetas = {{0,1}, {1,2}, {2,3}, {3,0},
                          {4,5}, {5,6}, {6,7}, {7,4},
                          {8, 9}, {9,10}, {10,11}, {11,8},
                          {12, 13}, {13, 14}, {14, 12}};
    q1->setPontos(listaAux);
    q1->setListaDeRetas(_descRetas, _descRetas.size());
    displayFile.append(q1);
    _descRetas.clear();
}

void mainFrame::paintEvent(QPaintEvent *event){
    QFrame::paintEvent(event);
    // Definindo a forma de desenho
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    // Desenhando Poligonos
    //atualizarObjetos(*window);
    for(Objeto *obj : qAsConst(displayFile)){
        desenharObjeto(obj, painter);
    }
}

void mainFrame::desenharObjeto(Objeto *obj, QPainter &painter){
   QPen caneta = QPen(obj->cor, this->caneta.width());
   painter.setPen(caneta);
   for(auto *r : obj->getListaDeRetas()){
       if(r->desenhar){
       int x1, x2, y1, y2;
       x1 = (int)r->getPonto(0)->getVwPosicao().x;
       x2 = (int)r->getPonto(1)->getVwPosicao().x;

       y1 = (int)r->getPonto(0)->getVwPosicao().y;
       y2 = (int)r->getPonto(1)->getVwPosicao().y;
       painter.drawLine(x1,y1,x2,y2);
       }
   }
   this->update();
}

void mainFrame::atualizarObjetos(){
    for(Objeto *obj : qAsConst(displayFile)){
        obj->atualizar(width(), height(),*window);
    }
}

void mainFrame::setEstado(Desenhando s){
    _estado = s;
}

void mainFrame::mousePressEvent(QMouseEvent *event){
    QPoint pMouse = event->pos();
    switch(_estado){
    case Desenhando::Linha:
        _clicks++;
         if(_clicks == 1){
            adicionarPontoTemp(pMouse.x(), pMouse.y());
        }
        else if(_clicks == 2){
            adicionarPontoTemp(pMouse.x(), pMouse.y());
            QList<Ponto> laux = QList<Ponto>();
            for(auto v : listaTemp){
                v = vwToSCN(v);
                v = SCNToWorld(v, *window);
                laux.append(Ponto(v));
            }
            Objeto *obj = new Objeto();
            QString naux = QString("Reta %1").arg(displayFile.size());
            obj->setNome(naux);
            obj->setPontos(laux);
            _descRetas = {{0,1}};
            obj->setListaDeRetas(_descRetas, _descRetas.size());
            obj->atualizar(width(), height(), *window);
            displayFile.append(obj);
            emit objetoAdicionado(obj);
            update();
            listaTemp.clear();
            _descRetas.clear();
            _clicks = 0;
        }
        break;
    case Desenhando::Retangulo:
        _clicks++;
        if(_clicks == 1){
            adicionarPontoTemp(pMouse.x(), pMouse.y());
        }
        else if(_clicks == 2){
            adicionarPontoTemp(pMouse.x(), pMouse.y());
            adicionarPontoTemp(pMouse.x(), listaTemp.at(0).y);
            adicionarPontoTemp(listaTemp.at(0).x, pMouse.y());

            QList<Ponto> laux = QList<Ponto>();
            for(auto v : listaTemp){
                v = vwToSCN(v);
                v = SCNToWorld(v, *window);
                laux.append(Ponto(v));
            }

            Objeto *obj = new Objeto();
            QString naux = QString("Retangulo %1").arg(displayFile.size());
            obj->setNome(naux);
            obj->setPontos(laux);

            _descRetas = {{0,2}, {2,1}, {1,3}, {3,0}};
            obj->setListaDeRetas(_descRetas, _descRetas.size());
            obj->atualizar(width(), height(), *window);
            displayFile.append(obj);

            emit objetoAdicionado(obj);
            update();

            listaTemp.clear();
            _descRetas.clear();
            _clicks = 0;
        }
        break;
    case Desenhando::Triangulo:
        _clicks++;
        if(_clicks == 1){
            adicionarPontoTemp(pMouse.x(), pMouse.y());
        }
        else if(_clicks == 2){
            adicionarPontoTemp(pMouse.x(), pMouse.y());
        }
        else if(_clicks == 3){
            adicionarPontoTemp(pMouse.x(), pMouse.y());

            QList<Ponto> laux = QList<Ponto>();
            for(auto v : listaTemp){
                v = vwToSCN(v);
                v = SCNToWorld(v, *window);
                laux.append(Ponto(v));
            }

            Objeto *obj = new Objeto();
            QString naux = QString("Triangulo %1").arg(displayFile.size());
            obj->setNome(naux);
            obj->setPontos(laux);

            _descRetas = {{0,1}, {1,2}, {2,0}};
            obj->setListaDeRetas(_descRetas, _descRetas.size());
            obj->atualizar(width(), height(), *window);
            displayFile.append(obj);

            emit objetoAdicionado(obj);
            update();

            listaTemp.clear();
            _descRetas.clear();
            _clicks = 0;
        }
        break;
    default:

        break;
    }
    event->accept();
}

Vec4 mainFrame::vwToSCN(Vec4 vw){
    Vec4 aux = Vec4();
    aux.x = (window->max.x - window->min.x)*(vw.x - 10)/(width() - 20) + window->min.x;
    aux.y = (window->max.y - window->min.y)*(vw.y - 10)/(height() - 20) - window->min.y - 1;
    aux.y = 1 - aux.y;
    aux.z = vw.z;
    return aux;
}

Vec4 mainFrame::SCNToWorld(Vec4 scn, Window &window){
    Transformes tr;
    tr.escalonar(Vec4(window.getLargura()/2, window.getAltura()/2, 0));
    return scn*tr.matriz;
}

void mainFrame::adicionarPontoTemp(int x, int y){
    listaTemp.append(Vec4((double)x, (double)y, 0));
}

void mainFrame::pararDesenho(){
    _clicks = 0;
    listaTemp.clear();
    _descRetas.clear();
}
