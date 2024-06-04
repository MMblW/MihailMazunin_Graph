#include "dialog.h"
#include "ui_dialog.h"

#include "graph.h"
#include "TSPalgorithm.h"

#include <QDebug>
#include <QMouseEvent>

#include <QGraphicsEllipseItem>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog())
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_btnAddVertex_clicked()
{
    int num = vertices.size();
    vert = new graphVertex(num);
    vert->setGraph(&graph);
    vertices.push_back(vert);
    scene->addItem(vert);
    graph.insertVertex(num);
}

void Dialog::on_btnAdj_clicked()
{
    adj = new Adjacency;
    adj->setWindowTitle("Матрица смежности");
    adj->setAdj(graph.getAdj());
    adj->setSize(graph.getVertexList().size());
    QVector<QString> names;
    for (auto elem : graph.getVertexList())
    {
        names.push_back(QString::number(elem));
    }
    adj->rebuild(names);
    adj->show();
}


void Dialog::on_btnAddEdge_clicked()
{
    graphVertex *node1, *node2;
    int vert1, vert2;
    if (ui->graphicsView->scene()->selectedItems().size() == 1) {
        node1 = dynamic_cast<graphVertex *> (ui->graphicsView->scene()->selectedItems().at(0));
        node1->setPressed(true);
        if (for_draw.isEmpty()) for_draw.push_back(node1);
        else if (for_draw.front() != node1 && for_draw.front() != nullptr)
        {
            int result = ui->spinBox->value();
            node2 = for_draw.front();
            vert1 = node1->getNum(); vert2 = node2->getNum();
            graph.InsertEdge(vert2, vert1, result);
            edge = new Edge(node1, node2);
            edge->setGraph(&graph);
            if (node1->pressed() == true) node1->setPressed(false);
            if (node2->pressed() == true) node2->setPressed(false);
            edge->setContent(result);
            scene->addItem(edge);
            edges.push_back(edge);
            for_draw.pop_back();
        }
    }
    ui->graphicsView->scene()->clearSelection();
}

void Dialog::on_btnDelete_clicked()
{
    if (ui->graphicsView->scene()->selectedItems().size()!=0)
    {
        int last = ui->graphicsView->scene()->selectedItems().size()-1;
        auto current = ui->graphicsView->scene()->selectedItems().at(last);
        delete current;
    }
}

void Dialog::on_btnTSP_clicked()
{
    QString res = TSP(graph);
    ui->label->setText(res);
}

void Dialog::on_btnSW_clicked()
{
    int current = 0;
    current = ui->spinBox_2->value();
    if (current < graph.getVertexList().size())
    {
        QString res = "";
        graph.ShortestWay(current, res);
        ui->label->setText(res);
    }
}
