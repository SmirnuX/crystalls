#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :   //Конструктор главного окна
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->CrystallWidget->update();
    int count = 4;
    crystalls = new Crystall*[count];
    crystalls[0] = new Smirnov1();
    crystalls[1] = new Smirnov2();
    crystalls[2] = new Volodina1();
    crystalls[3] = new Romb();
    double** z_buffer = new double*[ui->CrystallWidget->height()];  //Z-буфер для каждого кристалла
    for (int i = 0; i < ui->CrystallWidget->height(); i++)
        z_buffer[i] = new double[ui->CrystallWidget->width()];
    for(int i = 0; i < count; i++)
    {
        ui->CrystallChoice->addItem(crystalls[i]->name);
        crystalls[i]->z_buffer = z_buffer;
    }
    connect(ui->CrystallChoice, SIGNAL(activated(int)), this, SLOT(changeCrystall(int)));
    connect(ui->CrystallWidget, SIGNAL(updated()), this, SLOT(updateCrystall()));
    connect(ui->reflectXOY, SIGNAL(stateChanged(int)), this, SLOT(updateCrystall()));
    connect(ui->reflectXOZ, SIGNAL(stateChanged(int)), this, SLOT(updateCrystall()));
    connect(ui->reflectYOZ, SIGNAL(stateChanged(int)), this, SLOT(updateCrystall()));
    connect(ui->positionX, SIGNAL(valueChanged(int)), this, SLOT(updateCrystall()));
    connect(ui->positionY, SIGNAL(valueChanged(int)), this, SLOT(updateCrystall()));
    connect(ui->positionZ, SIGNAL(valueChanged(int)), this, SLOT(updateCrystall()));
    connect(ui->skewX, SIGNAL(valueChanged(int)), this, SLOT(updateCrystall()));
    connect(ui->skewY, SIGNAL(valueChanged(int)), this, SLOT(updateCrystall()));
    connect(ui->skewZ, SIGNAL(valueChanged(int)), this, SLOT(updateCrystall()));
    connect(ui->gradientCheck, SIGNAL(toggled(bool)), this, SLOT(updateCrystall()));
    connect(ui->numbersCheck, SIGNAL(toggled(bool)), this, SLOT(updateCrystall()));
    connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(Reset()));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(changeLab(int)));
    connect(ui->ZbufferBox, SIGNAL(toggled(bool)), this, SLOT(updateCrystall()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Reset()
{
    ui->CrystallWidget->crystall->Reset();

    ui->reflectXOY->setChecked(false);
    ui->reflectXOZ->setChecked(false);
    ui->reflectYOZ->setChecked(false);

    ui->positionX->setValue(0);
    ui->positionY->setValue(0);
    ui->positionZ->setValue(0);

    ui->skewX->setValue(100);
    ui->skewY->setValue(100);
    ui->skewZ->setValue(100);

    ui->CrystallWidget->a = 0;
    ui->CrystallWidget->b = 0;
    ui->CrystallWidget->g = 0;

    ui->CrystallWidget->crystall->Change();
    updateCrystall();
}

void MainWindow::changeCrystall(int index)
{

    if (index >= 0)
    {
        ui->CrystallWidget->crystall = crystalls[index];    
        ui->count->setText( QString::fromLocal8Bit("Вершин: ") + QString::number(ui->CrystallWidget->crystall->vertex_count) +
                            QString::fromLocal8Bit("; Ребер: ") + QString::number(ui->CrystallWidget->crystall->edges_count) +
                            QString::fromLocal8Bit("; Граней: ") + QString::number(ui->CrystallWidget->crystall->faces_count));
    }
    updateCrystall();
}

void MainWindow::updateCrystall()
{
    ui->CrystallWidget->parameters.show_vertices = ui->numbersCheck->isChecked();
    ui->CrystallWidget->parameters.gradient_lines = ui->gradientCheck->isChecked();
    ui->CrystallWidget->parameters.show_zbuf = ui->ZbufferBox->isChecked();
    ui->CoordsView->clear();
    if (ui->CrystallWidget->crystall == NULL)
        return;

    ui->CrystallWidget->crystall->reflectXOY = ui->reflectXOY->isChecked();
    ui->CrystallWidget->crystall->reflectXOZ = ui->reflectXOZ->isChecked();
    ui->CrystallWidget->crystall->reflectYOZ = ui->reflectYOZ->isChecked();

    ui->CrystallWidget->crystall->dx = ui->positionX->value();
    ui->CrystallWidget->crystall->dy = ui->positionY->value();
    ui->CrystallWidget->crystall->dz = -ui->positionZ->value();

    ui->counterX->setValue(ui->positionX->value());
    ui->counterY->setValue(ui->positionY->value());
    ui->counterZ->setValue(ui->positionZ->value());

    ui->counterSkewX->setValue((double)ui->skewX->value()/100);
    ui->counterSkewY->setValue((double)ui->skewY->value()/100);
    ui->counterSkewZ->setValue((double)ui->skewZ->value()/100);

    ui->CrystallWidget->crystall->skewX = (double)ui->skewX->value()/100;
    ui->CrystallWidget->crystall->skewY = (double)ui->skewY->value()/100;
    ui->CrystallWidget->crystall->skewZ = (double)ui->skewZ->value()/100;

    ui->CrystallWidget->crystall->Change();
    ui->CrystallWidget->update();

    for (int i = 0; i < ui->CrystallWidget->crystall->vertex_count; i++)
    {
        QString coord = QString::number(i) + ".  ";
        if (coord.size() > 4)
            coord.chop(1);
        ui->CoordsView->addItem(coord + ui->CrystallWidget->crystall->turned_vertexes[i].Show());
    }
}

void MainWindow::changeLab(int index)
{
    switch (index)
    {
    case 0:
        ui->CrystallWidget->parameters.intersec_zbuf = false;
        break;
    case 1:
        ui->CrystallWidget->parameters.intersec_zbuf = true;
        break;
    }
    updateCrystall();
}

canvas::canvas(QWidget* parent) : QWidget(parent)
{
    crystall = NULL;
    prev_pos.setX(0);
    prev_pos.setY(0);
    parameters.gradient_lines = true;
    parameters.intersec_zbuf = false;
    parameters.show_zbuf = false;
    parameters.show_vertices = false;
    a = 0;
    b = 0;
    g = 0;
}

void canvas::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(0,0,width(),height(),QColor(255,255,255));
    if (crystall != NULL)
        crystall->Draw(&painter, &parameters);
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
