#include "mainwindow.h"

Romb::Romb() : Crystall(8,12,1,QString::fromLocal8Bit("Ромбоэдр"))
{
    CalculatePoints();
    for (int i = 0; i < vertex_count; i++)
        turned_vertexes[i].Set(vertexes[i].x, vertexes[i].y, vertexes[i].z);
}

void Romb::CalculatePoints()
{
    //Базис: e1(1,0,0); e2(cos a, sin a, 0); e3(cos a, 0, sin a)
    double _a = M_PI / 4;  //Какой то угол
    double w = 70;  //Сторона
    double diag_x = w * (1 + cos(_a)*2);
    double diag_y = w * sin(_a);
    double diag_z = w * sin(_a);
    for (int i = 0; i < 8; i++)
    {
        double x, y, z;
        if (i > 3)  //1_x_x
            x = w;
        else
            x = 0;
        y = 0;
        z = 0;
        if ((i % 4) > 1)    //x_1_x
        {
            x+=w*cos(_a);
            y+=w*sin(_a);
        }
        if (i%2 == 1)   //x_x_1
        {
            x+=w*cos(_a);
            z+=w*sin(_a);
        }
        vertexes[i].Set(x - diag_x/2, y-diag_y/2, z-diag_z/2);
    }
    AddEdge(0, 1, 3);   //Первая грань
    AddEdge(1, 3, 7);
    AddEdge(2, 7, 5);
    AddEdge(3, 5, 1);

    AddEdge(4, 2, 3);
    AddEdge(5, 0, 4);
    AddEdge(6, 1, 0);
    AddEdge(7, 0, 2);

    AddEdge(8, 2, 6);
    AddEdge(9, 6, 7);
    AddEdge(10, 5, 4);

    AddEdge(11, 6, 4);

    faces[0].Set(4, 5, 7, 6);
    /*faces[3].Set(1, 3, 7, 5);
    faces[1].Set(7, 3, 2, 6);
    faces[2].Set(0, 4, 5, 1);

    faces[4].Set(0, 2, 6, 4);

    faces[5].Set(1, 3, 2, 0);*/

    for (int i = 0; i < faces_count; i++)
        faces[i].SetSource(turned_vertexes);

}

