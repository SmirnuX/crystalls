#include "mainwindow.h"
Volodina1::Volodina1() : Crystall(8,12,6,QString::fromLocal8Bit("Тригональный трапецоэдр (левый)"))
{
    //32 - hkil
    CalculatePoints();
    for (int i = 0; i < vertex_count; i++)
        turned_vertexes[i].Set(vertexes[i].x, vertexes[i].y, vertexes[i].z);
}

double rad(double dg)
{
    return (dg*M_PI)/180.0;
}

double t(double a, double b, double g) //вычисляет пересечение прямой и плоскости в параметрическом виде
{
    a = rad(a);
    b = rad(b);
    g = rad(g);

    double zn = sin(a)*cos(b) - cos(a)*sin(b) + sin(a)*cos(g) - cos(a)*sin(g)-sin(b)*cos(g)+cos(b)*sin(g);
    double ch = 2*sin(a)*cos(b)-2*cos(a)*sin(b);
    return ch/zn;
}

void Volodina1::CalculatePoints()
{
    double rd_ax = 100;
    double h = 200;

    double delta = -5;


    for(int i = 0; i < 3; i++)
    {
        //параметр
        double t_ = t(i*120, (i+1)*120, i*120+delta);
        //координаты в параметрическом виде
        double x = rd_ax*cos(rad(i*120+delta))*t_;
        double y = rd_ax*sin(rad(i*120+delta))*t_;
        double z = h-h*t_;

        vertexes[1+i*2].Set(x,y,z);

        //параметр
        t_ = t(i*120-delta, (i+1)*120-delta, i*120);
        //координаты в параметрическом виде
        x = rd_ax*cos(rad(i*120))*t_;
        y = rd_ax*sin(rad(i*120))*t_;
        z = -h+h*t_;

        vertexes[2+i*2].Set(x,y,z);
    }


    vertexes[0].Set(0, 0, h);
    vertexes[7].Set(0, 0, -h);

    AddEdge(0, 7, 1);
    AddEdge(1, 7, 3);
    AddEdge(2, 7, 5);
    AddEdge(3, 0, 2);
    AddEdge(4, 0, 4);
    AddEdge(5, 0, 6);

    AddEdge(6, 1, 2);
    AddEdge(7, 2, 3);
    AddEdge(8, 3, 4);
    AddEdge(9, 4, 5);
    AddEdge(10, 5, 6);
    AddEdge(11, 6, 1);

    faces[0].Set(1, 2, 0, 6);
    faces[1].Set(5, 6, 0, 4);
    faces[2].Set(2, 3, 4, 0);
    faces[3].Set(1, 7, 3, 2);
    faces[4].Set(5, 7, 1, 6);
    faces[5].Set(3, 7, 5, 4);

    for (int i = 0; i < faces_count; i++)
        faces[i].SetSource(turned_vertexes);

}

Volodina2::Volodina2() : Crystall(38, 60, 0, QString::fromLocal8Bit("Пентагонтритетраэдр"))
{
    CalculatePoints();
    for (int i = 0; i < vertex_count; i++)
        turned_vertexes[i].Set(vertexes[i].x, vertexes[i].y, vertexes[i].z);
}

void Volodina2::CalculatePoints()
{

}
