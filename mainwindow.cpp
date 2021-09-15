#include "mainwindow.h"
#include "ui_mainwindow.h"

Crystall::Crystall(int vertex, int edges, QString _name)
{
    vertex_count = vertex;   //Количество вершин
    edges_count = edges;    //Количество ребер
    name = _name;
    vertexes = new Point3D[vertex_count];
    edges_from = new int[edges_count];
    edges_to = new int[edges_count];
    turned_vertexes = new Point3D[vertex_count];   //Массив вершин после изменений
    a = 0;
    b = 0;
    g = 0;
    scale = 1;
}

void Crystall::Change()
{
    for (int i = 0; i < vertex_count; i++)
    {
        int y = vertexes[i].y;
        int z = vertexes[i].z;

        turned_vertexes[i].y = y * cos(a) + z * sin(a);
        turned_vertexes[i].z = -y * sin(a) + z * cos(a);
    }
    for (int i = 0; i < vertex_count; i++)
    {
        int x = vertexes[i].x;
        int z = turned_vertexes[i].z;

        turned_vertexes[i].x = x * cos(b) - z * sin(b);
        turned_vertexes[i].z = x * sin(b) + z * cos(b);
    }
    for (int i = 0; i < vertex_count; i++)
    {
        int x = turned_vertexes[i].x;
        int y = turned_vertexes[i].y;

        turned_vertexes[i].x = x * cos(g) + y * sin(g);
        turned_vertexes[i].y = -x * sin(g) + y * cos(g);
    }
    for (int i = 0; i < vertex_count; i++)
    {
        turned_vertexes[i].x *= scale;
        turned_vertexes[i].y *= scale;
        turned_vertexes[i].z *= scale;
    }
}

void Crystall::Scale(double _scale)
{
    scale = _scale;
    Change();
}

void Crystall::Turn(double alpha, double beta, double gamma)
{
    a = alpha;
    b = beta;
    g = gamma;
    Change();
}


void Crystall::Draw(QPainter* painter, bool gradient, bool numbers)
{
    painter->setPen(QColor(0,0,255));
    int width = painter->window().width();
    int height = painter->window().height();
    for (int i = 0; i < edges_count; i++)
    {
        if (gradient)
            drawLine3D(painter, &turned_vertexes[edges_from[i]], &turned_vertexes[edges_to[i]], width/2, height/2);
        else
        {
            QPoint a(width/2 + turned_vertexes[edges_from[i]].x, height/2 + turned_vertexes[edges_from[i]].z);
            QPoint b(width/2 + turned_vertexes[edges_to[i]].x, height/2 + turned_vertexes[edges_to[i]].z);
            painter->drawLine(a, b);
        }
    }
    if (numbers)
    {
        painter->setPen(QColor(100,0,0));
        for (int i = 0; i < vertex_count; i++)
        {
            painter->drawText(turned_vertexes[i].x + width/2, turned_vertexes[i].z + height/2, QString::number(i));
        }
    }
}

Crystall::~Crystall()
{
    delete[] vertexes;
    delete[] edges_from;
    delete[] edges_to;
    delete[] turned_vertexes;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->CrystallWidget->update();
    int count = 2;
    crystalls = new Crystall*[count];
    crystalls[0] = new Smirnov1();
    crystalls[1] = new Smirnov2();

    for(int i = 0; i < count; i++)
        ui->CrystallChoice->addItem(crystalls[i]->name);
    connect(ui->CrystallChoice, SIGNAL(activated(int)), this, SLOT(changeCrystall(int)));
    connect(ui->CrystallWidget, SIGNAL(updated(int)), this, SLOT(changeCrystall(int)));

    ui->CrystallWidget->show_numbers = ui->numbersCheck->isChecked();
    ui->CrystallWidget->is_gradient = ui->gradientCheck->isChecked();
}

MainWindow::~MainWindow()
{
    delete ui;
}


QString Point3D::Show()
{
    QString a = "{" + QString::number(x) + "; " + QString::number(y) + "; " + QString::number(z) + "}";
    return a;
}

Point3D::Point3D()
{
    x = 0;
    y = 0;
    z = 0;
}

void Point3D::Set(double _x, double _y, double _z)
{
    x = _x;
    y = _y;
    z = _z;
}

void Crystall::AddEdge(int ind, int from, int to)
{
    edges_to[ind] = to;
    edges_from[ind] = from;
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

void MainWindow::changeCrystall(int index)
{
    ui->CrystallWidget->show_numbers = ui->numbersCheck->isChecked();
    ui->CrystallWidget->is_gradient = ui->gradientCheck->isChecked();
    if (index >= 0)
    {
        ui->CrystallWidget->crystall = crystalls[index];
        ui->CrystallWidget->update();
        ui->count->setText(QString::fromLocal8Bit("Вершин: ") + QString::number(ui->CrystallWidget->crystall->vertex_count) + QString::fromLocal8Bit("; Ребер: ") + QString::number(ui->CrystallWidget->crystall->edges_count));
    }
    ui->CoordsView->clear();
    for (int i = 0; i < ui->CrystallWidget->crystall->vertex_count; i++)
    {
        ui->CoordsView->addItem(QString::number(i) + ".\t" + ui->CrystallWidget->crystall->turned_vertexes[i].Show());
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
    crystall->Turn(a,b,g);
    update();
    updated(-1);
    prev_pos.setX(event->posF().x());
    prev_pos.setY(event->posF().y());
}

void canvas::wheelEvent(QWheelEvent *event)
{
    if (crystall == NULL)
        return;
    crystall->Scale(crystall->scale + (double)(event->delta())/1000);
    update();
    updated(-1);
}

void drawLine3D(QPainter* painter, Point3D* a, Point3D* b, int dx, int dy, int y_min, int y_max)
{
    double z_delta = b->z - a->z;   //Разница в координатах z
    double x_delta = b->x - a->x;   //Разница в координатах x
    if (x_delta == 0)
        x_delta = 1;
    double d = z_delta / qAbs(x_delta);
    double d_y; //Производная y (dy/dx OR dy/dz)
    bool x_tick = true;
    double c, c_max, qt, y;    //
    if (d < -1 || d > 1)    //Обратная производная
    {
        x_tick = false;
        if (z_delta == 0)
            z_delta = 1;
        d = x_delta/qAbs(z_delta);
        d_y = (b->y - a->y)/z_delta;
        if (z_delta < 0)
        {
            c = b->z;
            c_max = a->z;
            d = -d;
            qt = b->x;
            y = b->y;
        }
        else
        {
            c = a->z;
            c_max = b->z;
            qt = a->x;
            y = a->y;
        }
    }
    else
    {
        d_y = (b->y - a->y)/x_delta;
        if (x_delta < 0)
        {
            c = b->x;
            c_max = a->x;
            d = -d;
            qt = b->z;
            y = b->y;
        }
        else
        {
            c = a->x;
            c_max = b->x;
            qt = a->z;
            y = a->y;
        }
    }

    for (; c < c_max; c++)
    {
        float k = (y - y_min) / (y_max - y_min);
        if (k < 0.1)
            k = 0.1;
        if (k > 1)
            k = 1;
        painter->setPen(QColor(0, 0, 0, 255*k));
        if (x_tick)
            painter->drawPoint(c + dx, qt + dy);
        else
            painter->drawPoint(qt + dx, c + dy);
        qt += d;
        y += d_y;
    }
}
