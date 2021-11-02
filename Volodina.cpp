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

Volodina2::Volodina2() : Crystall(20, 30, 12, QString::fromLocal8Bit("Пентагонтритетраэдр"))
{
    CalculatePoints();
    for (int i = 0; i < vertex_count; i++)
        turned_vertexes[i].Set(vertexes[i].x, vertexes[i].y, vertexes[i].z);
}

void Volodina2::CalculatePoints()
{
    double a = 10;
    double b = 20;
    double c = 30;    //a<=b<=c
    double n = a*a*c - b*c*c;   //a2c-bc2
    double d1 = a*a - a*b + b*b + a*c - 2*b*c;
    double d2 = a*a + a*b + b*b - a*c - 2*b*c;

    //A, B, C

    vertexes[1].Set(a, b, c);
    vertexes[3].Set(-a, b, -c);
    vertexes[4].Set(a, -b, -c);
    vertexes[7].Set(-a, -b, c);

    vertexes[2].Set(c, a, b);
    vertexes[5].Set(c, -a, -b);
    vertexes[8].Set(-c, a, -b);
    vertexes[0].Set(-c, -a, b);

    vertexes[6].Set(b, c, a);
    vertexes[9].Set(b, -c, -a);
    vertexes[10].Set(-b, -c, a);
    vertexes[11].Set(-b, c, -a);

    vertexes[12].Set(n/d1, n/d1, n/d1);
    vertexes[13].Set(-n/d1, -n/d1, n/d1);
    vertexes[14].Set(-n/d1, n/d1, -n/d1);
    vertexes[15].Set(n/d1, -n/d1, -n/d1);

    vertexes[16].Set(-n/d2, -n/d2, -n/d2); //15/8
    vertexes[17].Set(-n/d2, n/d2, n/d2);
    vertexes[18].Set(n/d2, -n/d2, n/d2);
    vertexes[19].Set(n/d2, n/d2, -n/d2);

    AddEdge(0, 7, 1);   //Первая грань
    AddEdge(1, 1, 12);
    AddEdge(2, 12, 2);
    AddEdge(3, 2, 18);
    AddEdge(4, 18, 7);
    AddEdge(5, 3, 4);   //Вторая грань
    AddEdge(6, 4, 15);
    AddEdge(7, 15, 5);
    AddEdge(8, 5, 19);
    AddEdge(9, 19, 3);
    AddEdge(10, 5, 2);   //Третья грань
    AddEdge(11, 12, 6);
    AddEdge(12, 6, 19);
    AddEdge(13, 0, 8);   //Четвертая грань
    AddEdge(14, 8, 14);
    AddEdge(15, 1, 17);
    AddEdge(16, 4, 16);
    AddEdge(17, 17, 0);
    AddEdge(18, 11, 6);   //Пятая грань
    AddEdge(19, 11, 17);

    AddEdge(22, 7, 13);

    AddEdge(20, 13, 0);

    AddEdge(21, 3, 14);
    AddEdge(23, 8, 16);
    AddEdge(24, 14, 11);
    AddEdge(25, 9, 18);
    AddEdge(26, 13, 10);
    AddEdge(27, 10, 16);
    AddEdge(28, 15, 9);
    AddEdge(29, 9, 10);


    faces[0].Set(18, 2, 12, 1, 7);
    faces[1].Set(19, 5, 15, 4, 3);
    faces[2].Set(19, 6, 12, 2, 5);
    faces[3].Set(17, 11, 14, 8, 0);
    faces[4].Set(17, 1, 12, 6, 11);
    faces[5].Set(18, 7, 13, 10, 9);
    faces[6].Set(16, 4, 15, 9, 10);
    faces[7].Set(19, 3, 14, 11, 6);
    faces[8].Set(16, 10, 13, 0, 8);
    faces[9].Set(18, 9, 15, 5, 2);
    faces[10].Set(16, 8, 14, 3, 4);
    faces[11].Set(17, 0, 13, 7, 1);

    for (int i = 0; i < faces_count; i++)
        faces[i].SetSource(turned_vertexes);

}
