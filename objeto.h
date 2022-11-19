#ifndef OBJETO_H
#define OBJETO_H
#include <QList>
#include "vec.h"
#include "window.h"
#include <QVector>
#include <QColor>

enum TipoObjeto{
    OBJ_RETA = 1,
    OBJ_RETANGULO,
    OBJ_TRIANGULO,
    OBJ_POLIGONO
};

class Ponto{
private:
    Vec3 _mPosicao, _scnPosicao, _vwPosicao;
    unsigned short int _rc = 0b0000;
public:
    Ponto(const Vec3 &mp, const Vec3 &scn, const unsigned short rc);
    Ponto();
    Ponto(const Vec3 &mp);
    Ponto(double x, double y, double z);
    // Getters
    Vec3 getPosicaoNoMundo();
    Vec3 getSCNPosicao();
    Vec3 getVwPosicao() const;
    unsigned short int getRC();
    // Setters
    void setPosicaoNoMundo(const Vec3 &mp);
    void setSCNPosicao(Window &window);
    void setSCNPosicao(const Transformes &tr);
    void setSCNPosicao(Vec3 &s);
    void setRC(Window &window);
    void setVwPosicao(int w, int h, Window &window);
};


class Reta{
private:
    QVector<Ponto*> _pontos;
public:
    bool desenhar;
    Reta();
    Reta(Ponto *p1, Ponto *p2);
    ~Reta();
    // Getters
    Ponto *getPonto(int p);
    // Setters
    void setPontos(QVector<Ponto> pts);
    // Atualiza o SCN do objeto inteiro
    void atualizarSCN(Window &window);
    void atualizarSCN(const Transformes &tr);
    // Atualiza o RegionCode do objeto inteiro
    void atualizarRC(Window &window);
    void atualizarVw(int w, int h, Window &window);
};

class Objeto
{
private:
    TipoObjeto _tipo;
    QString _nome;
    QList<Reta*> _retas;
    Ponto _origem;
    QList<Ponto*> _pontos;
    std::vector<std::vector<int>>_descRetas;
    int _nLinhas;
    int _ID;

    void recortar(Ponto *p, Reta *r, Window &window);
    bool intersectaWindowEixoX(Ponto *p,double m, Window &window, double y2);
    bool intersectaWindowEixoY(Ponto *p, double m, Window &window, double x2);
public:
    QColor cor;
    Objeto();
    // Tipo do Objeto, Nome do Objeto, Lista de Ponto e Ponto de Origem
    Objeto(const TipoObjeto t,const QString &n, QList<Ponto> pts, const Ponto &p);
    // Tipo do Objeto, Nome do Objeto e Ponto de Origem
    Objeto(const TipoObjeto t, const QString &n, const Ponto &p);
    ~Objeto();
    // Getters
    TipoObjeto getTipoObjeto();
    QString getNome();
    QList<Reta*> getListaDeRetas();
    Ponto getOrigem();
    // Setters
    void setTipoObjeto(const TipoObjeto &t);
    void setNome(const QString &n);
    void setListaDeRetas(std::vector<std::vector<int>> decRetas, int nlinhas);
    void setOrigem(const Ponto &p);
    void setPontos(const QList<Ponto> pts);
    // Atualiza o SCN do objeto inteiro
    void atualizarSCN(Window &window);
    void atualizarSCN(const Transformes &tr);
    // Atualiza o RegionCode do objeto inteiro
    void atualizarRC(Window &window);
    // Atualiza tanto o SCN quanto o RC
    void atualizar(int w, int h, Window &window);
    // Atualiza as coordenadas da viewport
    void atualizarVw(int w, int h, Window &window);
    // Realiza transformações geométricas no objeto
    void transformar(Transformes tr);
    // Calcula o ponto de origem do objeto
    void ajustarOrigem();
    // Realiza o clip (Recorte de linhas)
    void clippingCohenSutherland(Window &window);
};

#endif // OBJETO_H
