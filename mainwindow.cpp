#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "graph.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    graph = new Graph(scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionAdd_triggered()
{
    QPointF scenePos = ui->graphicsView->mapToScene(ui->graphicsView->viewport()->rect().center());
    static int nextId = 1;
    graph->add_node(nextId++, scenePos);
}


void MainWindow::on_actionZoom_In_triggered()
{
    constexpr double factor = 1.2;
    ui->graphicsView->scale(factor, factor);
}


void MainWindow::on_actionZoom_Out_triggered()
{
    constexpr double factor = 1/1.2;
    ui->graphicsView->scale(factor, factor);
}

