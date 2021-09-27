#include "mainwindow.h"

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
    dx = 0;
    dy = 0;
    dz = 0;
    reflectXOY = true;
    reflectXOZ = true;
    reflectYOZ = true;
    scale = 1;
}

void Crystall::Change() //Изменение фигуры
{

    //Инициализация
    for (int i = 0; i < vertex_count; i++)
    {
        turned_vertexes[i].x = vertexes[i].x;
        turned_vertexes[i].y = vertexes[i].y;
        turned_vertexes[i].z = vertexes[i].z;
    }
    //Поворот
    for (int i = 0; i < vertex_count; i++)
    {
        int y = turned_vertexes[i].y;
        int z = turned_vertexes[i].z;

        turned_vertexes[i].y = y * cos(a) + z * sin(a);
        turned_vertexes[i].z = -y * sin(a) + z * cos(a);
    }
    for (int i = 0; i < vertex_count; i++)
    {
        int x = turned_vertexes[i].x;
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
    //Отражение
    for (int i = 0; i < vertex_count; i++)
    {
        if (reflectYOZ)
            turned_vertexes[i].x = -turned_vertexes[i].x;
        if (reflectXOY)
            turned_vertexes[i].z = -turned_vertexes[i].z;
        if (reflectXOZ)
            turned_vertexes[i].y = -turned_vertexes[i].y;
    }
    //Масштаб
    for (int i = 0; i < vertex_count; i++)
    {
        turned_vertexes[i].x *= scale;
        turned_vertexes[i].y *= scale;
        turned_vertexes[i].z *= scale;
    }
    //Перенос
    for (int i = 0; i < vertex_count; i++)
    {
            turned_vertexes[i].x += dx;
            turned_vertexes[i].y += dy;
            turned_vertexes[i].z += dz;
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

    //Отрисовка осей
    Point3D center, x, y, z;
    center.Set(0, 0, 0);
    double x_k, y_k, z_k, k = 50;
    x_k = reflectYOZ?-k:k;
    y_k = reflectXOZ?-k:k;
    z_k = reflectXOY?-k:k;

    x.Set(x_k * cos(b)*cos(g), y_k * -cos(b)*sin(g), z_k * sin(b));
    y.Set(x_k * (sin(a)*sin(b)*cos(g)+cos(a)*sin(g)), y_k * (-sin(a)*sin(b)*sin(g)+cos(a)*cos(g)), z_k * -sin(a)*cos(b));
    z.Set(-x_k*(-cos(a)*sin(b)*cos(g)+sin(a)*sin(g)), -y_k*(cos(a)*sin(b)*sin(g)+sin(a)*cos(g)), -z_k*cos(a)*cos(b));  //Ось инвертирована

    drawLine3D(painter, &center, &x, width-55, height-55, -55, 55, QColor(255,0,0));
    drawLine3D(painter, &center, &y, width-55, height-55, -55, 55, QColor(0,255,0));
    drawLine3D(painter, &center, &z, width-55, height-55, -55, 55, QColor(0,0,255));

    painter->setPen(QColor(255,0,0));
    painter->drawText(width-55 + x.x, height-55 + x.z, "x");
    painter->setPen(QColor(0,255,0));
    painter->drawText(width-55 + y.x, height-55 + y.z, "y");
    painter->setPen(QColor(0,0,255));
    painter->drawText(width-55 + z.x, height-55 + z.z, "z");


    //Отражение

}

void Crystall::AddEdge(int ind, int from, int to)
{
    edges_to[ind] = to;
    edges_from[ind] = from;
}

Crystall::~Crystall()
{
    delete[] vertexes;
    delete[] edges_from;
    delete[] edges_to;
    delete[] turned_vertexes;
}
