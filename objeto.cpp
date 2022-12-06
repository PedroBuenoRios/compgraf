#include "objeto.h"
#include <QDebug>
/***************************************************************************/
Objeto::Objeto()
{
    _tipo = OBJ_POLIGONO;
    _nome = "Poligono";
    _retas = QList<Reta*>();
    _origem = Vec4(0,0,0);
    cor = QColor(0,0,0);
}

Objeto::Objeto(const TipoObjeto t, const QString &n, const Ponto &orig): _tipo(t), _nome(n), _origem(orig){
    _retas = QList<Reta*>();
    _pontos = QList<Ponto*>();
}

Objeto::Objeto(const TipoObjeto t,const QString &n, QList<Ponto> pts, const Ponto &p):_tipo(t), _nome(n), _origem(p){
    this->setPontos(pts);
}

// Getters

QString Objeto::getNome(){
    return this->_nome;
}

TipoObjeto Objeto::getTipoObjeto(){
    return this->_tipo;
}

QList<Reta*> Objeto::getListaDeRetas(){
    return this->_retas;
}

Ponto Objeto::getOrigem(){
    return _origem;
}

QList<Face*> Objeto::getListaDeFaces(){
    return _faces;
}

// Setters

void Objeto::setListaDeRetas(std::vector<std::vector<int>> descRetas, int nlinhas){
    _retas.clear();
    _nLinhas = nlinhas;
    _descRetas.clear();
    for(int l = 0; l < nlinhas; l++){
        _descRetas.push_back(descRetas.at(l));
        Ponto p1 = *(_pontos.at(descRetas.at(l).at(0)));
        Ponto p2 = *(_pontos.at(descRetas.at(l).at(1)));
        _retas.append(new Reta(new Ponto(p1), new Ponto(p2)));
    }
}

void Objeto::setNome(const QString &n){
    _nome = n;
}

void Objeto::setTipoObjeto(const TipoObjeto &t){
    _tipo = t;
}

void Objeto::setOrigem(const Ponto &p){
    _origem = p;
}

void Objeto::setPontos(QList<Ponto> pts){
    _pontos.clear();
    for(Ponto p : qAsConst(pts)){
        _pontos.append(new Ponto(p));
    }
    ajustarOrigem();
}

void Objeto::setFaces(QVector<QVector<int>> df, int nf){
    _faces.clear();
    _nFaces = nf;
    QList<Ponto> pts;
    for(int i = 0; i < nf; i++){
       // descFaces.append(df.at(i));
        for(int j = 0; j < df.at(i).length(); j++){
            if(df[i][j] > _pontos.length())
                qDebug() << "Teste" <<'\n';

            pts.append(*(_pontos.at(df[i][j])));

            if(j == (df.at(i).length() - 1))
                _descRetas.push_back(std::vector<int>({df[i][j], df[i][0]}));
            else
                _descRetas.push_back(std::vector<int>({df[i][j], df[i][j+1]}));
        }
        _faces.append(new Face(pts));
    }
    setListaDeRetas(_descRetas, _descRetas.size());
}

void Objeto::transformar(Transformes tr){
    for(Ponto *p : qAsConst(_pontos)){
        p->setPosicaoNoMundo(p->getPosicaoNoMundo()*tr.matriz);
    }
    ajustarOrigem();
}

// Atualiza o SCN de cada ponto do objeto
void Objeto::atualizarSCN(Window &window){
    _origem.setSCNPosicao(window);
    for(Ponto *p : qAsConst(_pontos)){
        p->setSCNPosicao(window);
    }
    for(int i = 0; i < _nLinhas; i++){
        Reta *r = _retas.at(i);
        int j1 = _descRetas.at(i).at(0);
        int j2 = _descRetas.at(i).at(1);
        QVector<Ponto> v;
        v.push_back(Ponto(*(_pontos.at(j1))));
        v.push_back(Ponto(*(_pontos.at(j2))));
        r->setPontos(v);
        r->atualizarSCN(window);
    }
}

void Objeto::atualizarSCN(const Transformes &tr){
    _origem.setSCNPosicao(tr);
    for(Ponto *p : qAsConst(_pontos)){
        p->setSCNPosicao(tr);
    }
    for(Reta *r : qAsConst(_retas)){
        r->atualizarSCN(tr);
    }
}
// Atualiza o RC de cada ponto do Objeto
void Objeto::atualizarRC(Window &window){
    for(Ponto *p : qAsConst(_pontos)){
        p->setRC(window);
    }
    for(Reta *r : qAsConst(_retas)){
        r->atualizarRC(window);
    }
}

void Objeto::atualizarVw(int w, int h, Window &window){
    for(Ponto *p : qAsConst(_pontos)){
        p->setVwPosicao(w, h, window);
    }
    for(Reta *r : qAsConst(_retas)){
        r->atualizarVw(w, h, window);
    }
}

void Objeto::atualizarVCS(Transformes &tr){
    Vec4 aux;
    for(Ponto *p : qAsConst(_pontos)){
        p->setVCSPosicao(tr.matriz*p->getPosicaoNoMundo());
        aux = p->getVCSPosicao();
        aux.x = p->getVCSPosicao().x/p->getVCSPosicao().w;
        aux.y = p->getVCSPosicao().y/p->getVCSPosicao().w;
        p->setVCSPosicao(aux);
    }
    for(Reta *r : qAsConst(_retas)){
        r->atualizarVCS(tr);
    }
}

void Objeto::atualizar(int w, int h, Window &window){
    Transformes tr;
    tr.WCparaVCS(window);
    atualizarVCS(tr);
    atualizarSCN(window);
    atualizarRC(window);
    clippingCohenSutherland(window);
    atualizarVw(w, h, window);
    ajustarOrigem();
}

void Objeto::ajustarOrigem(){
    int n = _pontos.length();
    Vec4 ori = Vec4(0,0,0);
    for(int i = 0; i < n; i++){
        ori.x += _pontos.at(i)->getPosicaoNoMundo().x;
        ori.y += _pontos.at(i)->getPosicaoNoMundo().y;
        ori.z += _pontos.at(i)->getPosicaoNoMundo().z;
    }
    ori.x = ori.x/n;
    ori.y = ori.y/n;
    ori.z = ori.z/n;
    _origem.setPosicaoNoMundo(ori);
}

void Objeto::clippingCohenSutherland(Window &window){
     unsigned short rc1 = 0b0000, rc2 = 0b0000;
     for(Reta *r : qAsConst(_retas)){
         rc1 = r->getPonto(0)->getRC();
         rc2 = r->getPonto(1)->getRC();
         //Segmento completamente dentro da window
         if(rc1 == 0b0000 && rc2 == 0b0000){
             r->desenhar = true;
         }
         else if((rc1 & rc2) == 0b0000){
            for(int i = 0; i < 2; i++){
                Ponto *p = r->getPonto(i);
                recortar(p, r, window);
            }
         }
         else if((rc1 & rc2) != 0b0000){
             r->desenhar = false;
         }
     }
}


void Objeto::recortar(Ponto *p, Reta *r, Window &window){
    unsigned short rc = p->getRC();
    double m = 0.0;

    m = (r->getPonto(1)->getSCNPosicao().y - r->getPonto(0)->getSCNPosicao().y)/
        (r->getPonto(1)->getSCNPosicao().x - r->getPonto(0)->getSCNPosicao().x);

    switch(rc){
    // Passa pela borda esquerda
    case 0b0001:
        // y = m*(xe - x1) + y1
        r->desenhar = intersectaWindowEixoY(p, m, window, window.min.x);
        break;
      // Passa pela borda direita
      case 0b0010:
        // y = m*(xd - x1) + y1
        r->desenhar = intersectaWindowEixoY(p, m, window, window.max.x);
        break;
    // Passa pela borda de cima
    case 0b1000:
        // x = x1 + (yt - y1)/m
        r->desenhar = intersectaWindowEixoX(p, m, window, window.max.y);
        break;
     // Passa pela borda de baixo
     case 0b0100:
        // x = x1 + (yb - y1)/m
        r->desenhar = intersectaWindowEixoX(p, m, window, window.min.y);
        break;
     // Cima ou direita
     case 0b1010:
        r->desenhar = intersectaWindowEixoY(p, m, window, window.max.x) ||
                      intersectaWindowEixoX(p, m, window, window.max.y);
     break;
     // Cima ou esquerda
     case 0b1001:
        r->desenhar = intersectaWindowEixoY(p, m, window, window.min.x) ||
                      intersectaWindowEixoX(p, m, window, window.max.y);
        break;
     // Baixo ou direita
     case 0b0110:
        r->desenhar = intersectaWindowEixoY(p, m, window, window.max.x) ||
                      intersectaWindowEixoX(p, m, window, window.min.y);
        break;
     // Baixo ou esquerda
     case 0b0101:
        r->desenhar = intersectaWindowEixoY(p, m, window, window.max.x) ||
                      intersectaWindowEixoX(p, m, window, window.max.y);
        break;
     default:
        break;
    }
}

bool Objeto::intersectaWindowEixoX(Ponto *p,double m, Window &window, double y2){
    double x = 0.0;
    x = p->getSCNPosicao().x + (1/m) *(y2 - p->getSCNPosicao().y);
    if(x >= window.min.x && x <= window.max.x){
        Vec4 scn = Vec4(x, y2, 0);
        p->setSCNPosicao(scn);
        return true;
    }
    return false;
}

bool Objeto::intersectaWindowEixoY(Ponto *p, double m, Window &window, double x2){
     double y = m*(x2 - p->getSCNPosicao().x) + p->getSCNPosicao().y;
    if(y >= window.min.y && y <= window.max.y ){
        Vec4 scn = Vec4(x2, y, 0);
        p->setSCNPosicao(scn);
        return true;
    }
   return false;
}

/***************************************************************************/

Ponto::Ponto(const Vec4 &p){
    _mPosicao = p;
    _scnPosicao = Vec4(0, 0, 0);
    _rc = 0b0000;
}

Ponto::Ponto(){
    _mPosicao = Vec4(0, 0, 0);
    _scnPosicao = Vec4(0, 0, 0);
    _rc = 0b0000;
}

Ponto::Ponto(double x, double y, double z){
    _mPosicao = Vec4(x,y,z);
    _scnPosicao = Vec4(0,0, 0);
    _rc = 0b0000;
}

void Ponto::setRC(Window &window){
    _rc = 0b0000;
    if(_scnPosicao.y > window.max.y)
        _rc = _rc | 0b1000;
    else if(_scnPosicao.y < window.min.y)
        _rc = _rc | 0b0100;
    if(_scnPosicao.x > window.max.x)
        _rc = _rc | 0b0010;
    else if(_scnPosicao.x < window.min.x)
        _rc = _rc | 0b0001;
}

Vec4 Ponto::getPosicaoNoMundo(){
    return _mPosicao;
}

Vec4 Ponto::getSCNPosicao(){
    return _scnPosicao;
}

unsigned short Ponto::getRC(){
    return _rc;
}

Vec4 Ponto::getVwPosicao() const {
    return _vwPosicao;
}

Vec4 Ponto::getVCSPosicao(){
    return _vcsPosicao;
}

void Ponto::setVCSPosicao(const Vec4 &p){
    _vcsPosicao = p;
}

void Ponto::setSCNPosicao(Window &window){
    Transformes tr = Transformes();
    tr.escalonar(Vec4(1.0/(window.getLargura()/2.0), 1.0/(window.getAltura()/2.0), 0));
    _scnPosicao = _vcsPosicao*tr.matriz;
}

void Ponto::setSCNPosicao(const Transformes &tr){
    _scnPosicao = _vcsPosicao*tr.matriz;
}

void Ponto::setSCNPosicao(Vec4 &s){
    _scnPosicao.x = s.x;
    _scnPosicao.y = s.y;
    _scnPosicao.z = s.z;
}

void Ponto::setVwPosicao(int w, int h, Window &window){
    _vwPosicao.x = (_scnPosicao.x - window.min.x)/(window.max.x - window.min.x);
    _vwPosicao.x = (_vwPosicao.x)*(w-20) + 10;
    _vwPosicao.y = (_scnPosicao.y - window.min.y)/(window.max.y - window.min.y);
    _vwPosicao.y = (1 - _vwPosicao.y)*(h-20) + 10;

}

void Ponto::setPosicaoNoMundo(const Vec4 &mp){
    _mPosicao = mp;
}

Reta::Reta(Ponto *p1, Ponto *p2){
    _pontos = QVector<Ponto*>(2);
    _pontos[0] = p1;
    _pontos[1] = p2;
}

Reta::Reta(){}

Reta::~Reta(){
    delete _pontos[0];
    delete _pontos[1];
}

Ponto *Reta::getPonto(int p){
    return _pontos.at(p);
}

void Reta::setPontos(QVector<Ponto> pts){
    _pontos[0] = new Ponto(pts.at(0));
    _pontos[1] = new Ponto(pts.at(1));
}

void Reta::atualizarSCN(Window &window){
    _pontos[0]->setSCNPosicao(window);
    _pontos[1]->setSCNPosicao(window);
}

void Reta::atualizarSCN(const Transformes &tr){
    _pontos[0]->setSCNPosicao(tr);
    _pontos[1]->setSCNPosicao(tr);
}

void Reta::atualizarRC(Window &window){
    _pontos[0]->setRC(window);
    _pontos[1]->setRC(window);
}

void Reta::atualizarVw(int w, int h, Window &window){
    _pontos[0]->setVwPosicao(w, h, window);
    _pontos[1]->setVwPosicao(w, h, window);
}

void Reta::atualizarVCS(Transformes &tr){
    Vec4 aux;
    _pontos[0]->setVCSPosicao(_pontos[0]->getPosicaoNoMundo()*tr.matriz);
    aux = _pontos[0]->getVCSPosicao();
    aux.x = _pontos[0]->getVCSPosicao().x/_pontos[0]->getVCSPosicao().w;
    aux.y = _pontos[0]->getVCSPosicao().y/_pontos[0]->getVCSPosicao().w;
    _pontos[0]->setVCSPosicao(aux);

    _pontos[1]->setVCSPosicao(_pontos[1]->getPosicaoNoMundo()*tr.matriz);
    aux = _pontos[1]->getVCSPosicao();
    aux.x = _pontos[1]->getVCSPosicao().x/_pontos[1]->getVCSPosicao().w;
    aux.y = _pontos[1]->getVCSPosicao().y/_pontos[1]->getVCSPosicao().w;
    _pontos[1]->setVCSPosicao(aux);
}

Face::Face(){
    pontos = QList<Ponto*>();
}

Face::Face(QList<Ponto> pts){
    for(auto pt : pts){
        pontos.append(new Ponto(pt));
    }
}
Face::~Face(){
    pontos.clear();
}
