#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "objeto.h"
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void transformarWindow();
    void transformarDisplayFile(Transformes tr);
    void transformarObjeto();
    void normalizarObjetos(Transformes tr);

private:
    Ui::MainWindow *ui;
public slots:
    void adicionarObjetoUI(Objeto *obj);
    void selecionarObj();
    void mudarCaneta();
    void desenharLinha();
    void desenharQuadrado();
    void desenharTriangulo();
    void aplicarTransformacoes();
    void pararDesenho();
};
#endif // MAINWINDOW_H
