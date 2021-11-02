#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :   //Конструктор главного окна
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->CrystallWidget->update();
    int count = 8;
    crystalls = new Crystall*[count];
    crystalls[0] = new Smirnov1();
    crystalls[1] = new Smirnov2();
    crystalls[2] = new Volodina1();
    crystalls[3] = new Volodina2();
    crystalls[4] = new Borodina1();
    crystalls[5] = new Borodina2();
    crystalls[6] = new Romb();
    crystalls[7] = new Cube();

    for (int i = 0; i < count; i++)
    {
        ui->CrystallChoice->addItem(crystalls[i]->name);
    }
    //Выбор кристаллов
    connect(ui->CrystallChoice, SIGNAL(activated(int)), this, SLOT(changeCrystall(int)));
    connect(ui->CrystallWidget, SIGNAL(updated()), this, SLOT(mouseUpdateCrystall()));
    //Отражение кристалла
    connect(ui->reflectXOY, SIGNAL(stateChanged(int)), this, SLOT(updateCrystall()));
    connect(ui->reflectXOZ, SIGNAL(stateChanged(int)), this, SLOT(updateCrystall()));
    connect(ui->reflectYOZ, SIGNAL(stateChanged(int)), this, SLOT(updateCrystall()));
    //Перенос кристалла
    connect(ui->positionX, SIGNAL(valueChanged(int)), this, SLOT(updateCrystall()));
    connect(ui->positionY, SIGNAL(valueChanged(int)), this, SLOT(updateCrystall()));
    connect(ui->positionZ, SIGNAL(valueChanged(int)), this, SLOT(updateCrystall()));
    //Растяжение кристалла
    connect(ui->skewX, SIGNAL(valueChanged(int)), this, SLOT(updateCrystall()));
    connect(ui->skewY, SIGNAL(valueChanged(int)), this, SLOT(updateCrystall()));
    connect(ui->skewZ, SIGNAL(valueChanged(int)), this, SLOT(updateCrystall()));
    //Изменение цвета кристалла
    connect(ui->colorSliderR, SIGNAL(valueChanged(int)), this, SLOT(updateCrystall()));
    connect(ui->colorSliderR, SIGNAL(valueChanged(int)), ui->colorCounterR, SLOT(setValue(int)));
    connect(ui->colorSliderG, SIGNAL(valueChanged(int)), this, SLOT(updateCrystall()));
    connect(ui->colorSliderG, SIGNAL(valueChanged(int)), ui->colorCounterG, SLOT(setValue(int)));
    connect(ui->colorSliderB, SIGNAL(valueChanged(int)), this, SLOT(updateCrystall()));
    connect(ui->colorSliderB, SIGNAL(valueChanged(int)), ui->colorCounterB, SLOT(setValue(int)));

    connect(ui->shadowBox, SIGNAL(toggled(bool)), this, SLOT(updateCrystall()));
    connect(ui->paintFacesBox, SIGNAL(toggled(bool)), this, SLOT(updateCrystall()));    //Заливка кристалла
    connect(ui->gradientCheck, SIGNAL(toggled(bool)), this, SLOT(updateCrystall()));    //Градиентные линии
    connect(ui->numbersCheck, SIGNAL(toggled(bool)), this, SLOT(updateCrystall()));     //Отображение номеров
    connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(Reset()));                   //Сброс положения кристалла
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(changeLab(int)));    //Выбор лабораторной работы
    connect(ui->ZbufferBox, SIGNAL(toggled(bool)), this, SLOT(updateCrystall()));       //Показать Z-буфер
    connect(ui->zbuf_number_box, SIGNAL(toggled(bool)), this, SLOT(updateCrystall()));  //Скрывать вершины, не видимые по Z-буферу

    connect(ui->ShowNormals, SIGNAL(toggled(bool)), this, SLOT(updateCrystall()));      //Показать нормали
    //Изменение угла света
    connect(ui->lightX, SIGNAL(valueChanged(int)), this, SLOT(updateCrystall()));
    connect(ui->lightZ, SIGNAL(valueChanged(int)), this, SLOT(updateCrystall()));
    //Изменение яркости
    connect(ui->Brightness, SIGNAL(valueChanged(int)),  this, SLOT(updateCrystall()));
    connect(ui->Ambient, SIGNAL(valueChanged(int)),  this, SLOT(updateCrystall()));
    //Изменение значений в счетчиках
    connect(ui->angleA, SIGNAL(valueChanged(double)), this, SLOT(angleAUpdateCrystall()));
    connect(ui->angleB, SIGNAL(valueChanged(double)), this, SLOT(updateCrystall()));
    connect(ui->angleG, SIGNAL(valueChanged(double)), this, SLOT(angleGUpdateCrystall()));
    connect(ui->scale, SIGNAL(valueChanged(double)), this, SLOT(updateCrystall()));
    connect(ui->counterX, SIGNAL(valueChanged(int)), ui->positionX, SLOT(setValue(int)));
    connect(ui->counterY, SIGNAL(valueChanged(int)), ui->positionY, SLOT(setValue(int)));
    connect(ui->counterZ, SIGNAL(valueChanged(int)), ui->positionZ, SLOT(setValue(int)));

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

    ui->angleA->setValue(0);
    ui->angleB->setValue(0);
    ui->angleG->setValue(0);
    ui->scale->setValue(1);



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

void MainWindow::mouseUpdateCrystall()
{
    ui->angleA->setValue(ui->CrystallWidget->a);
    ui->angleG->setValue(ui->CrystallWidget->g);
    ui->scale->setValue(ui->CrystallWidget->crystall->scale);
    updateCrystall();
}

void MainWindow::angleAUpdateCrystall()
{
    ui->CrystallWidget->a = ui->angleA->value();
    updateCrystall();
}

void MainWindow::angleGUpdateCrystall()
{
    ui->CrystallWidget->g = ui->angleG->value();
    updateCrystall();
}

void MainWindow::updateCrystall()
{
    ui->CrystallWidget->parameters.show_vertices = ui->numbersCheck->isChecked();
    ui->CrystallWidget->parameters.gradient_lines = ui->gradientCheck->isChecked();
    ui->CrystallWidget->parameters.show_zbuf = ui->ZbufferBox->isChecked();
    ui->CrystallWidget->parameters.show_zbuf_numbers = ui->zbuf_number_box->isChecked();
    ui->CrystallWidget->parameters.show_faces = ui->paintFacesBox->isChecked();
    ui->CrystallWidget->parameters.faces_shade = ui->shadowBox->isChecked();
    ui->CrystallWidget->parameters.faces_color = qRgb(ui->colorSliderR->value(),
                                                      ui->colorSliderG->value(),
                                                      ui->colorSliderB->value());

    ui->CrystallWidget->parameters.show_normals = ui->ShowNormals->isChecked();
    ui->CrystallWidget->parameters.k_diff = (double)ui->Brightness->value() / 10.0;
    ui->CrystallWidget->parameters.k_amb = (double)ui->Ambient->value() / 10.0;

    double radius = 1000;

    ui->CrystallWidget->parameters.light_x = radius * (sin((double)ui->lightX->value()/100.0));
    ui->CrystallWidget->parameters.light_y = - radius * (cos((double)ui->lightX->value()/100.0));
    ui->CrystallWidget->parameters.light_z = - radius * sin((double)ui->lightZ->value()/200.0);

    ui->CrystallWidget->crystall->scale = ui->scale->value();

    ui->CrystallWidget->b = ui->angleB->value();

    ui->CrystallWidget->crystall->Turn(ui->CrystallWidget->a, ui->CrystallWidget->b, ui->CrystallWidget->g);


    ui->zbuf_number_box->setEnabled(ui->numbersCheck->isChecked());
    ui->shadowBox->setEnabled(ui->paintFacesBox->isChecked());
    ui->CoordsView->clear();
    ui->FacesView->clear();
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

    for (int i = 0; i < ui->CrystallWidget->crystall->vertex_count; i++)    //Занесение вершин в список
    {
        QString coord = QString::number(i) + ".  ";
        if (coord.size() > 4)
            coord.chop(1);
        ui->CoordsView->addItem(coord + ui->CrystallWidget->crystall->turned_vertexes[i].Show());
    }

    if (ui->CrystallWidget->parameters.intersec_veyler) //Занесение граней в список
    {
        for (int i = 0; i < ui->CrystallWidget->crystall->vec.size(); i++)
        {
            QString new_item = QString::number(i + 1) + ".  ";
            if (new_item.size() > 4)
                new_item.chop(1);
            for (int j = 0; j < ui->CrystallWidget->crystall->vec[i]->size; j++)
            {
                new_item += QString::number(ui->CrystallWidget->crystall->vec[i]->points[j]);
                if (j < ui->CrystallWidget->crystall->vec[i]->size - 1)
                    new_item += "-";
            }
            ui->FacesView->addItem(new_item);
        }
    }
}

void MainWindow::changeLab(int index)
{
    switch (index)
    {
    case 0:
    case 1:
        ui->CrystallWidget->parameters.intersec_zbuf = false;
        ui->CrystallWidget->parameters.intersec_veyler = false;
        ui->CrystallWidget->parameters.guro = false;
        break;
    case 2:
        ui->CrystallWidget->parameters.intersec_zbuf = true;
        ui->CrystallWidget->parameters.intersec_veyler = false;
        ui->CrystallWidget->parameters.guro = false;
        break;
    case 3:
        ui->CrystallWidget->parameters.intersec_zbuf = false;
        ui->CrystallWidget->parameters.intersec_veyler = true;
        ui->CrystallWidget->parameters.guro = false;
        break;
    case 4:
        ui->CrystallWidget->parameters.intersec_zbuf = false;
        ui->CrystallWidget->parameters.intersec_veyler = false;
        ui->CrystallWidget->parameters.guro = true;
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
    parameters.show_zbuf_numbers = true;
    parameters.faces_color = qRgb(0, 0, 0);
    parameters.show_faces = false;
    parameters.faces_shade = false;
    parameters.intersec_veyler = false;
    parameters.guro = false;              //Закраска тела методом Гуро
    parameters.show_normals = false;
    parameters.light_x = 0;
    parameters.light_y = -1000;
    parameters.light_z = 0;
    parameters.k_diff = 2.5;          //Коэффициент яркости источника освещения
    parameters.k_amb = 0.1;           //Коэффициент фонового освещения
    a = 0;
    b = 0;
    g = 0;  
    img = NULL;   //Инициализация изображения
    z_buffer = NULL;
}

void canvas::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(0,0,width(),height(),QColor(255,255,255));
    if (crystall != NULL)
        crystall->Draw(&painter, &parameters, &img, &z_buffer);
    if (frame_count == 0)
        timer.start();
    else
    {
        painter.setPen(QColor(0,0,0));
        painter.drawText(0, 0, 100, 100, 0, "FPS: " + QString::number(frame_count / (float)timer.elapsed() * 1000.0, 'f', 0));
    }
    frame_count++;
    if (timer.elapsed() > 1000)
    {
        timer.restart();
        frame_count = 1;
    }
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
    //qDebug()<<delta_x<<" "<<g;
    prev_pos.setX(event->posF().x());
    prev_pos.setY(event->posF().y());
    //crystall->Turn(a,b,g);
    //update();
    updated();
}

void canvas::wheelEvent(QWheelEvent *event)
{
    if (crystall == NULL)
        return;
    if (crystall->scale + (double)(event->delta())/1000 < 0.1)
        crystall->Scale(0.1);
    else if (crystall->scale + (double)(event->delta())/1000 > 4)
        crystall->Scale(4);
    else
        crystall->Scale(crystall->scale + (double)(event->delta())/1000);

    update();
    updated();
}
