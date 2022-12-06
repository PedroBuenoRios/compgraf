#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <regex>
#include <iostream>
#include <fstream>
#include <string>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->tamanhoCaneta, SIGNAL(valueChanged(int)), this, SLOT(mudarCaneta()));
    QObject::connect(ui->frame, SIGNAL(objetoAdicionado(Objeto*)), this, SLOT(adicionarObjetoUI(Objeto*)));
    // Configurando os controles do usuário
    // Controles da caneta
    ui->tamanhoCaneta->setRange(1, 10);
    ui->tamanhoCaneta->setSingleStep(1);
    // Controles da Window
    ui->spinWindowX->setMinimum(-30000);
    ui->spinWindowX->setMaximum(30000);
    ui->spinWindowY->setMaximum(30000);
    ui->spinWindowY->setMinimum(-30000);

    double x = ui->frame->window->getOrigem().x;
    double y = ui->frame->window->getOrigem().y;
    ui->spinWindowX->setValue(x);
    ui->spinWindowY->setValue(y);

    ui->spinWindowAltura->setMinimum(10);
    ui->spinWindowAltura->setMaximum(1000);
    ui->spinWindowLargura->setMinimum(30);
    ui->spinWindowLargura->setMaximum(1000);

    ui->spinWindowAltura->setValue(ui->frame->window->getAltura());
    ui->spinWindowLargura->setValue(ui->frame->window->getLargura());

    ui->spinWindowEscala->setMaximum(10);
    ui->spinWindowEscala->setMinimum(0);
    ui->spinWindowEscala->setValue(1.0);

    ui->spinWindowRotacao->setMinimum(-360);
    ui->spinWindowRotacao->setMaximum(360);

    ui->spinWindowRotacao->setValue(ui->frame->window->getGrausRotacionados());
    // Controles do Objeto
    ui->spinWindowX->setMinimum(-30000);
    ui->spinWindowX->setMaximum(30000);
    ui->spinWindowY->setMaximum(30000);
    ui->spinWindowY->setMinimum(-30000);

    ui->spinObjX->setMinimum(-30000);
    ui->spinObjY->setMinimum(-30000);
    ui->spinObjX->setMaximum(30000);
    ui->spinObjY->setMaximum(30000);
    ui->spinObjX->setValue(0.0);
    ui->spinObjY->setValue(0.0);

    ui->spinObjEscala->setMaximum(10);
    ui->spinObjEscala->setMinimum(0);
    ui->spinObjEscala->setValue(1.0);

    ui->spinObjRotacao->setMinimum(-360);
    ui->spinObjRotacao->setMaximum(360);
    ui->spinObjRotacao->setValue(0.0);

    ui->spinObjTransX->setMinimum(-1000);
    ui->spinObjTransY->setMinimum(-1000);
    ui->spinObjTransX->setMaximum(1000);
    ui->spinObjTransY->setMaximum(1000);

    ui->spinObjTransX->setSingleStep(10.0);
    ui->spinObjTransY->setSingleStep(10.0);

    ui->listaObjetos->addItem(ui->frame->displayFile.last()->getNome());
    ui->listaObjetos->setCurrentRow(0);
    lerArquivoOBJ("/home/pedro/Documentos/Dev/CompGraf/entrega04/Charizard.obj");
    lerArquivoOBJ("/home/pedro/Documentos/Dev/CompGraf/entrega04/Maxwell.obj");
    ui->frame->atualizarObjetos();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mudarCaneta(){
    // Atualiza o tamaho da caneta
    this->ui->frame->caneta = QPen(Qt::black, ui->tamanhoCaneta->value());
    this->ui->frame->update();
}

void MainWindow::desenharLinha(){
    ui->btnReta->setDown(true);
    ui->btnQuadrado->setDown(false);
    ui->btnTriangulo->setDown(false);
    ui->frame->setEstado(mainFrame::Desenhando::Linha);
}

void MainWindow::desenharQuadrado(){
    ui->btnReta->setDown(false);
    ui->btnQuadrado->setDown(true);
    ui->btnTriangulo->setDown(false);
    ui->frame->setEstado(mainFrame::Desenhando::Retangulo);
}

void MainWindow::desenharTriangulo(){
    ui->btnReta->setDown(false);
    ui->btnQuadrado->setDown(false);
    ui->btnTriangulo->setDown(true);
    ui->frame->setEstado(mainFrame::Desenhando::Triangulo);
}

void MainWindow::transformarWindow(){
    Transformes tr;
    // Transformações na window
    Window *window  = ui->frame->window;
    // Verifica se houve translação
    if( window->getOrigem().x != ui->spinWindowX->value() ||
        window->getOrigem().y != ui->spinWindowY->value()){
       // window->getOrigem().z != ui->spinWindowZ->value()){

        Vec4 passo;
        passo.x = -(ui->spinWindowX->value() - window->getOrigem().x);
        passo.y = -(ui->spinWindowY->value() - window->getOrigem().y);
       // passo.z = -(ui->spinWindowZ->value() - window->getOrigem().z);
        tr.transladar(passo);
    }
    // Aplicando rotação
    double dg = ui->spinWindowRotacao->value();
    tr.rotacionar(dg,EIXO::Z);
    // Movendo para a origem
    tr.transladar(Vec4(-window->getOrigem().x,
                       -window->getOrigem().y,
                       0));

    ui->spinWindowAltura->setValue(window->getAltura()*ui->spinWindowEscala->value());
    ui->spinWindowLargura->setValue(window->getLargura()*ui->spinWindowEscala->value());
    window->setAltura(ui->spinWindowAltura->value());
    window->setLargura(ui->spinWindowLargura->value());

    ui->spinWindowEscala->setValue(1.0);

    window->setGrausRotacionados(ui->spinWindowRotacao->value());

    window->setOrigem(Vec4(ui->spinWindowX->value(), ui->spinWindowY->value(), window->getOrigem().z));
   // window->calcularVetoresNormais();
    transformarDisplayFile(tr);
}

void MainWindow::transformarDisplayFile(Transformes tr){
    for(Objeto *obj : qAsConst(ui->frame->displayFile)){
        obj->transformar(tr);
    }
}

void MainWindow::transformarObjeto(){
    int i = ui->listaObjetos->currentRow();
    EIXO eixo = EIXO::Y;
    if(i < 0)
        return;
    Objeto *obj = ui->frame->displayFile.at(i);
    Transformes tr;
    // Aplicando as transformações
    // Voltando para o ponto 'inicial'
    tr.transladar(Vec4(
                      obj->getOrigem().getPosicaoNoMundo().x,
                      obj->getOrigem().getPosicaoNoMundo().y,
                      obj->getOrigem().getPosicaoNoMundo().z));
    // Rotacionando
    tr.rotacionar(ui->spinObjRotacao->value(), eixo);
    // Escalonando
    Vec4 v(ui->spinObjEscala->value(),ui->spinObjEscala->value(), ui->spinObjEscala->value());
    tr.escalonar(v);
     //Transladando
    v.x = ui->spinObjTransX->value();
    v.y = ui->spinObjTransY->value();
    v.z = 0;
    tr.transladar(v);
    // Movendo para a origem antes de aplicar as transformações
    tr.transladar(Vec4(
                      -obj->getOrigem().getPosicaoNoMundo().x,
                      -obj->getOrigem().getPosicaoNoMundo().y,
                      -obj->getOrigem().getPosicaoNoMundo().z));
    obj->transformar(tr);
    ui->spinObjEscala->setValue(1.0);
    selecionarObj();
}

void MainWindow::aplicarTransformacoes(){
    transformarWindow();
    transformarObjeto();
    // Atualizando o SCN do objetos
    ui->frame->atualizarObjetos();
}

void MainWindow::selecionarObj(){

    for(Objeto *obj : ui->frame->displayFile){
        obj->cor = QColor(0,0,0);
    }

    int i = ui->listaObjetos->currentRow();
    Objeto *obj = ui->frame->displayFile.at(i);

    ui->spinObjX->setValue(obj->getOrigem().getPosicaoNoMundo().x);
    ui->spinObjY->setValue(obj->getOrigem().getPosicaoNoMundo().y);

    obj->cor = QColor(0,0,255);
}

void MainWindow::adicionarObjetoUI(Objeto *obj){
    ui->listaObjetos->addItem(obj->getNome());
}

void MainWindow::pararDesenho(){
    ui->frame->setEstado(mainFrame::Desenhando::Ocioso);
    ui->frame->pararDesenho();
    ui->btnReta->setDown(false);
    ui->btnQuadrado->setDown(false);
    ui->btnTriangulo->setDown(false);
    ui->btnOcioso->setDown(true);
}

Objeto *MainWindow::lerArquivoOBJ(const char *caminho){
    FILE *arq = fopen(caminho, "r");

    if(arq == NULL){
        printf("Não foi possível abrir o Arquivo OBJ\n");
        return NULL;
    }
    char *linha = (char *)malloc(255);
    std::string aux = std::string();
   // std::regex regex_vertices(R"( -?[0-9]+\.[0-9]+)");
    std::regex regex_face(R"( [0-9]+)");

    std::smatch matches;

    Objeto *obj = new Objeto();
    QList<Ponto> pts = QList<Ponto>();

    while(!feof(arq)){
        fscanf(arq, "%s", linha);
        if(strcmp(linha, "v") == 0){
            Vec4 v = Vec4();
            fscanf(arq, "%lf %lf %lf\n", &v.x, &v.y, &v.z);
            pts.append(Ponto(v));
        }
        else if(strcmp(linha, "f") == 0){
            fgets(linha, 255, arq);
            aux = std::string(linha);
            std::sregex_iterator currentMatch(aux.begin(), aux.end(), regex_face);
            std::sregex_iterator lastMatch;

            obj->descFaces.append(QVector<int>());

            while(currentMatch != lastMatch){
                matches = *currentMatch;
                obj->descFaces.last().append(std::stoi(matches.str()) - 1);
                currentMatch++;
            }
        }
        else{
            fgets(linha, 255, arq);
        }
    }
    obj->setPontos(pts);
    obj->setFaces(obj->descFaces, obj->descFaces.length());
 //   obj->atualizar(ui->frame->width(), ui->frame->height(), *(ui->frame->window));
    obj->setNome(QString(caminho));
    adicionarObjetoUI(obj);
    ui->frame->displayFile.append(obj);
}
