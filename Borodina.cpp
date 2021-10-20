#include "mainwindow.h"
Borodina1::Borodina1() : Crystall(10,16,8,QString::fromLocal8Bit("Тетрагональный трапецоэдр"))
{
    CalculatePoints();
    for (int i = 0; i < vertex_count; i++)
        turned_vertexes[i].Set(vertexes[i].x, vertexes[i].y, vertexes[i].z);
}

void Borodina1::CalculatePoints()
{
    vertexes[0].Set(0, 0, 30);
    vertexes[1].Set(-20, 0, 10);
    vertexes[2].Set(0, -20, 10);
    vertexes[3].Set(20, 0, 10);
    vertexes[4].Set(0, 20, 10);
    vertexes[5].Set(-14.2, -14.2, -10);
    vertexes[6].Set(14.2, -14.2, -10);
    vertexes[7].Set(14.2, 14.2, -10);
    vertexes[8].Set(-14.2, 14.2, -10);
    vertexes[9].Set(0, 0, -30);

    AddEdge(0, 0, 1);
    AddEdge(1, 0, 2);
    AddEdge(2, 0, 3);
    AddEdge(3, 0, 4);

    AddEdge(4, 9, 5);
    AddEdge(5, 9, 6);
    AddEdge(6, 9, 7);
    AddEdge(7, 9, 8);

    AddEdge(8, 1, 5);
    AddEdge(9, 1, 8);
    AddEdge(10, 2, 6);
    AddEdge(11, 3, 6);

    AddEdge(12, 3, 7);
    AddEdge(13, 4, 7);
    AddEdge(14, 4, 8);
    AddEdge(15, 5, 2);


    faces[0].Set(0, 1, 5, 2);
    faces[1].Set(0, 2, 6, 3);
    faces[2].Set(0, 3, 7, 4);
    faces[3].Set(0, 4, 8, 1);
    faces[4].Set(9, 6, 2, 5);
    faces[5].Set(9, 5, 1, 8);
    faces[6].Set(9, 8, 4, 7);
    faces[7].Set(9, 7, 3, 6);

    for (int i = 0; i < faces_count; i++)
        faces[i].SetSource(turned_vertexes);

}

Borodina2::Borodina2() : Crystall(20, 30, 12, QString::fromLocal8Bit("Пентагондодекаэдр"))
{
    CalculatePoints();
    for (int i = 0; i < vertex_count; i++)
        turned_vertexes[i].Set(vertexes[i].x, vertexes[i].y, vertexes[i].z);
}

void Borodina2::CalculatePoints()
{
    vertexes[0].Set(-10, -10, 10);
    vertexes[1].Set(0, -6.18, 16.18);
    vertexes[2].Set(0, 6.18, 16.18);

    vertexes[3].Set(-10, 10, 10);
    vertexes[4].Set(-16.18, 0, 6.18);
    vertexes[5].Set(-6.18, -16.18, 0);

    vertexes[6].Set(6.18, -16.18, 0);
    vertexes[7].Set(10, -10, 10);
    vertexes[8].Set(16.18, 0, 6.18);

    vertexes[9].Set(10, 10, 10);
    vertexes[10].Set(6.18, 16.18, 0);
    vertexes[11].Set(-6.18, 16.18, 0);

    vertexes[12].Set(-10, 10, -10);
    vertexes[13].Set(-16.18, 0, -6.18);
    vertexes[14].Set(-10, -10, -10);

    vertexes[15].Set(10, -10, -10);
    vertexes[16].Set(16.18, 0, -6.18);
    vertexes[17].Set(10, 10, -10);

    vertexes[18].Set(0, 6.18, -16.18);
    vertexes[19].Set(0, -6.18, -16.18);

    for (int i = 0; i < vertex_count; i++)
    {
        vertexes[i].x *= 2;
        vertexes[i].y *= 2;
        vertexes[i].z *= 2;
    }

    AddEdge(0, 0, 1);
    AddEdge(1, 0, 5);
    AddEdge(2, 0, 4);

    AddEdge(3, 1, 2);
    AddEdge(4, 1, 7);
    AddEdge(5, 2, 3);
    AddEdge(6, 2, 9);
    AddEdge(7, 3, 4);

    AddEdge(8, 3, 11);
    AddEdge(9, 4, 13);
    AddEdge(10, 5, 6);
    AddEdge(11, 5, 14);

    AddEdge(12, 6, 7);
    AddEdge(13, 6, 15);
    AddEdge(14, 7, 8);
    AddEdge(15, 8, 9);

    AddEdge(16, 8, 16);
    AddEdge(17, 9, 10);
    AddEdge(18, 10, 11);
    AddEdge(19, 10, 17);

    AddEdge(20, 11, 12);
    AddEdge(21, 12, 13);
    AddEdge(22, 12, 18);
    AddEdge(23, 13, 14);

    AddEdge(24, 14, 19);
    AddEdge(25, 15, 16);
    AddEdge(26, 15, 19);
    AddEdge(27, 16, 17);

    AddEdge(28, 17, 18);
    AddEdge(29, 18, 19);

    faces[0].Set(5, 14, 19, 15, 6);

    faces[1].Set(14, 13, 12, 18, 19);
    faces[2].Set(19, 18, 17, 16, 15);
    faces[3].Set(15, 16, 8, 7, 6);
    faces[4].Set(5, 6, 7, 1, 0);
    faces[5].Set(0, 4, 13, 14, 5);

    faces[6].Set(11, 12, 13, 4, 3);
    faces[7].Set(0, 1, 2, 3, 4);
    faces[8].Set(1, 7, 8, 9, 2);
    faces[9].Set(8, 16, 17, 10, 9);
    faces[10].Set(11, 10, 17, 18, 12);

    faces[11].Set(2, 9, 10, 11, 3);

    for (int i = 0; i < faces_count; i++)
        faces[i].SetSource(turned_vertexes);

}
