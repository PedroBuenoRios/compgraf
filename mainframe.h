#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <QFrame>
#include <QPainter>
#include <QMouseEvent>
#include "objeto.h"
#include "window.h"

class mainFrame : public QFrame
{
    Q_OBJECT
public:
    enum Desenhando{
        Linha,
        Retangulo,
        Triangulo,
        Ocioso
    };
    mainFrame();
    explicit mainFrame(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
    //
    Window *window;
    QList<Objeto*> displayFile;
    QPen caneta;
    QList<Vec4> listaTemp;
    // Desenha um objeto
    void desenharObjeto(Objeto *obj, QPainter &painter);
    // Atualiza as propriedades de desenho de cada objeto
    void atualizarObjetos();
    // muda o estado de desenho
    void setEstado(Desenhando s);
    // Normaliza as coordenadas do objeto
    void normalizarObjetos();
    // Quando houver clicks na area de desenho
    void mousePressEvent(QMouseEvent *event) override;
    // Parar de desenhar objetos com o click
    void pararDesenho();
signals:
    void objetoAdicionado(Objeto *obj);
private:
    void adicionarPontoTemp(int x, int y);
    Desenhando _estado;
    int _clicks;
    std::vector<std::vector<int>> _descRetas;

    Vec4 vwToSCN(Vec4 vw);
    Vec4 SCNToWorld(Vec4 scn, Window &window);
};

#endif // MAINFRAME_H
