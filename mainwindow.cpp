#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :   //Конструктор главного окна
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->CrystallWidget->update();
    int count = 3;
    crystalls = new Crystall*[count];
    crystalls[0] = new Smirnov1();
    crystalls[1] = new Smirnov2();
    crystalls[2] = new Romb();

    for(int i = 0; i < count; i++)
        ui->CrystallChoice->addItem(crystalls[i]->name);
    connect(ui->CrystallChoice, SIGNAL(activated(int)), this, SLOT(changeCrystall(int)));
    connect(ui->CrystallWidget, SIGNAL(updated()), this, SLOT(updateCrystall()));
    connect(ui->reflectXOY, SIGNAL(stateChanged(int)), this, SLOT(updateCrystall()));
    connect(ui->reflectXOZ, SIGNAL(stateChanged(int)), this, SLOT(updateCrystall()));
    connect(ui->reflectYOZ, SIGNAL(stateChanged(int)), this, SLOT(updateCrystall()));
    connect(ui->positionX, SIGNAL(valueChanged(int)), this, SLOT(updateCrystall()));
    connect(ui->positionY, SIGNAL(valueChanged(int)), this, SLOT(updateCrystall()));
    connect(ui->positionZ, SIGNAL(valueChanged(int)), this, SLOT(updateCrystall()));
    connect(ui->gradientCheck, SIGNAL(toggled(bool)), this, SLOT(updateCrystall()));
    connect(ui->numbersCheck, SIGNAL(toggled(bool)), this, SLOT(updateCrystall()));

    ui->CrystallWidget->show_numbers = ui->numbersCheck->isChecked();
    ui->CrystallWidget->is_gradient = ui->gradientCheck->isChecked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeCrystall(int index)
{

    if (index >= 0)
    {
        ui->CrystallWidget->crystall = crystalls[index];    
        ui->count->setText(QString::fromLocal8Bit("Вершин: ") + QString::number(ui->CrystallWidget->crystall->vertex_count) + QString::fromLocal8Bit("; Ребер: ") + QString::number(ui->CrystallWidget->crystall->edges_count));
    }
    updateCrystall();
}

void MainWindow::updateCrystall()
{
    ui->CrystallWidget->show_numbers = ui->numbersCheck->isChecked();
    ui->CrystallWidget->is_gradient = ui->gradientCheck->isChecked();
    ui->CoordsView->clear();
    if (ui->CrystallWidget->crystall == NULL)
        return;

    ui->CrystallWidget->crystall->reflectXOY = ui->reflectXOY->isChecked();
    ui->CrystallWidget->crystall->reflectXOZ = ui->reflectXOZ->isChecked();
    ui->CrystallWidget->crystall->reflectYOZ = ui->reflectYOZ->isChecked();

    ui->CrystallWidget->crystall->dx = ui->positionX->value();
    ui->CrystallWidget->crystall->dy = ui->positionY->value();
    ui->CrystallWidget->crystall->dz = -ui->positionZ->value();

    ui->CrystallWidget->crystall->Change();
    ui->CrystallWidget->update();

    for (int i = 0; i < ui->CrystallWidget->crystall->vertex_count; i++)
    {
        ui->CoordsView->addItem(QString::number(i) + ".\t" + ui->CrystallWidget->crystall->turned_vertexes[i].Show());
    }
}

canvas::canvas(QWidget* parent) : QWidget(parent)
{
    crystall = NULL;
    prev_pos.setX(0);
    prev_pos.setY(0);
    a = 0;
    b = 0;
    g = 0;
}

void canvas::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(0,0,width(),height(),QColor(255,255,255));
    if (crystall != NULL)
        crystall->Draw(&painter, is_gradient, show_numbers);
}

void canvas::mousePressEvent(QMouseEvent *event)
{
    prev_pos.setX(event->posF().x());
    prev_pos.setY(event->posF().y());
}

void canvas::mouseMoveEvent(QMouseEvent *event)
{
    if (crystall == NULL)
        return;
    double delta_x = event->posF().x() - prev_pos.x();
    double delta_y = - event->posF().y() + prev_pos.y();
    g += delta_x/100;
    a += delta_y/100;
    crystall->Turn(a,b,g);
    update();
    updated();
    prev_pos.setX(event->posF().x());
    prev_pos.setY(event->posF().y());
}

void canvas::wheelEvent(QWheelEvent *event)
{
    if (crystall == NULL)
        return;
    crystall->Scale(crystall->scale + (double)(event->delta())/1000);
    update();
    updated();
}
