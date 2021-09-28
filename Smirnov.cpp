#include "mainwindow.h"

Smirnov1::Smirnov1() : Crystall(8,18,12,QString::fromLocal8Bit("Дитригональная дипирамида"))
{
    //В базовом конструкторе Crystall указываются - количество вершин, количетсво ребер, количетсво граней и название кристалла
    //Остальной конструктор изменению не подлежит
    CalculatePoints();
    for (int i = 0; i < vertex_count; i++)
        turned_vertexes[i].Set(vertexes[i].x, vertexes[i].y, vertexes[i].z);
}

void Smirnov1::CalculatePoints()
{
    int height = 200;
    int radius = 90;
    int smaller_radius = 75;
    //Необходимо вычислить все вершины, ребра и грани

    vertexes[0].Set(0, 0, height);  //Пример вершины: vertexes[i].Set(x, y, z)
    vertexes[1].Set(radius * sin(0), radius * cos(0), 0);
    vertexes[3].Set(radius * sin(2 * M_PI / 3), radius * cos(2 * M_PI / 3), 0);
    vertexes[5].Set(radius * sin(4 * M_PI / 3), radius * cos(4 * M_PI / 3), 0);

    vertexes[2].Set(smaller_radius * sin(M_PI/3), smaller_radius * cos(M_PI/3), 0);
    vertexes[4].Set(smaller_radius * sin(M_PI), smaller_radius * cos(M_PI), 0);
    vertexes[6].Set(smaller_radius * sin(5 * M_PI / 3), smaller_radius * cos(5 * M_PI / 3), 0);

    vertexes[7].Set(0, 0, -height);

    AddEdge(0, 1, 2);   //Пример ребра: AddEdge(i, a, b); где i - индекс ребра, a и b - индексы начальной и конечной вершин
    AddEdge(1, 2, 3);
    AddEdge(2, 3, 4);
    AddEdge(3, 4, 5);
    AddEdge(4, 5, 6);
    AddEdge(5, 6, 1);

    AddEdge(6, 7, 1);
    AddEdge(7, 7, 2);
    AddEdge(8, 7, 3);
    AddEdge(9, 7, 4);
    AddEdge(10, 7, 5);
    AddEdge(11, 7, 6);

    AddEdge(12, 0, 1);
    AddEdge(13, 0, 2);
    AddEdge(14, 0, 3);
    AddEdge(15, 0, 4);
    AddEdge(16, 0, 5);
    AddEdge(17, 0, 6);

    faces[0].Set(0, 1, 2); //Пример грани: faces[i].Set(a, b, c), где a,b,c... - номера вершин, эту грань образующую
    //Есть варианты facesSet для трех, четырех и пяти вершин. Если нужно больше, есть faces[i].Set(int* points, int count)
    faces[1].Set(0, 2, 3);
    faces[2].Set(0, 3, 4);
    faces[3].Set(0, 4, 5);
    faces[4].Set(0, 5, 6);
    faces[5].Set(0, 6, 1);

    faces[6].Set(7, 1, 2);
    faces[7].Set(7, 2, 3);
    faces[8].Set(7, 3, 4);
    faces[9].Set(7, 4, 5);
    faces[10].Set(7, 5, 6);
    faces[11].Set(7, 6, 1);


    for (int i = 0; i < faces_count; i++)
        faces[i].SetSource(turned_vertexes);

}

Smirnov2::Smirnov2() : Crystall(38, 60, 24, QString::fromLocal8Bit("Пентагонтриоктаэдр")) //24
{
    CalculatePoints();
    for (int i = 0; i < vertex_count; i++)
        turned_vertexes[i].Set(vertexes[i].x, vertexes[i].y, vertexes[i].z);
}

void Smirnov2::CalculatePoints()
{
    double t = 1.839286;    //Константы Трибоначчи
    double scale = 10;
    double x, y, z;
    x = scale;
    y = scale*(2*t +1);
    z = scale*(t * t);
    //Первые 12 вершин - четные перестановки (+-1; +-2t+1; +-t^2)с четным кол-вом минусов
    //Четные перестановки - x y z, z x y, y z x
    for (int i = 0; i < 4; i++)
    {
        switch (i)
        {
        case 1: //000 -> 110
        case 3: //011 -> 101
            x = -x;
            y = -y;
            break;
        case 2: //110 -> 011
            x = -x;
            z = -z;
            break;
        }
        vertexes[i * 3].Set(x, y, z);
        vertexes[i * 3 + 1].Set(z, x, y);
        vertexes[i * 3 + 2].Set(y, z, x);
    }
    x = -scale;
    y = scale*(2*t +1);
    z = scale*(t * t);
    //Вторые 12 вершин - четные перестановки (+-1; +-2t+1; +-t^2)с четным кол-вом минусов
    //Нечетные перестановки - y x z, x z y, z y x
    for (int i = 0; i < 4; i++)
    {
        switch (i)
        {
        case 1: //100 -> 010
        case 3: //001 -> 111
            x = -x;
            y = -y;
            break;
        case 2: //010 -> 001
            y = -y;
            z = -z;
            break;
        }
        vertexes[12 + i * 3].Set(y, x, z);
        vertexes[12 + i * 3 + 1].Set(x, z, y);
        vertexes[12 + i * 3 + 2].Set(z, y, x);
    }
    //6 граней октаэдра
    double t3 = scale*(t * t * t);
    vertexes[24].Set(t3, 0, 0);
    vertexes[25].Set(-t3,0,0);
    vertexes[26].Set(0, t3, 0);
    vertexes[27].Set(0, -t3, 0);
    vertexes[28].Set(0, 0, t3);
    vertexes[29].Set(0, 0, -t3);
    //8 вершин куба
    double t2 = scale*(t*t);
    vertexes[30].Set(t2, t2, t2);
    vertexes[31].Set(-t2, t2, t2);
    vertexes[32].Set(t2, -t2, t2);
    vertexes[33].Set(-t2, -t2, t2);
    vertexes[34].Set(t2, t2, -t2);
    vertexes[35].Set(-t2, t2, -t2);
    vertexes[36].Set(t2, -t2, -t2);
    vertexes[37].Set(-t2, -t2, -t2);

    //Грани - всего 60
    //Первый птяиугольник
    AddEdge(0, 1, 30);
    AddEdge(1,30,0);
    AddEdge(2,0,13);
    AddEdge(3,13,28);
    AddEdge(4,28,1);
    //Второй пятиугольник
    AddEdge(5,2,30);
    AddEdge(6,2,14);
    AddEdge(7,14,26);
    AddEdge(8,26,0);
    //Третий пятиугольник
    AddEdge(9,12,24);
    AddEdge(10,24,2);
    AddEdge(11,27,3);
    AddEdge(12,1,12);
    //Четвертый пятиугольник
    AddEdge(13,13,31);
    AddEdge(14,31,20);
    AddEdge(15,20,26);
    //Пятый
    AddEdge(16,14,34);
    AddEdge(17,34,18);
    AddEdge(18,18,24);
    //Шестой
    AddEdge(19,26,9);
    AddEdge(20,9,16);
    AddEdge(21,16,34);
    //7
    AddEdge(22,20,5);
    AddEdge(23,5,35);
    AddEdge(24,35,9);
    //8
    AddEdge(25,35,7);
    AddEdge(26,7,29);
    AddEdge(27,29,16);
    //9 - 11
    AddEdge(28,29,4);
    AddEdge(29,4,18);
    AddEdge(30,28,19);
    AddEdge(31,19,32);
    AddEdge(32,32,12);
    AddEdge(33,28,10);
    AddEdge(34,10,15);
    AddEdge(35,15,31);
    //12 - 15
    AddEdge(36,25,15);
    AddEdge(37,25,5);
    AddEdge(38,25,21);
    AddEdge(39,21,7);
    AddEdge(40,4,36);
    AddEdge(41,36,11);
    AddEdge(42,11,24);
    AddEdge(43,32,17);
    AddEdge(44,17,11);
    //?
    AddEdge(45,10,33);
    AddEdge(46,33,8);
    AddEdge(47,8,25);
    AddEdge(48,8,23);
    AddEdge(49,23,37);
    AddEdge(50,37,21);
    AddEdge(51,37,22);
    AddEdge(52,22,29);
    AddEdge(53,22,6);
    AddEdge(54,6,36);
    AddEdge(55,6,27);
    AddEdge(56,27,23);
    AddEdge(57,27,17);
    AddEdge(58,33,3);
    AddEdge(59,19,3);

    faces[0].Set(1, 30, 0, 13, 28);
    faces[1].Set(24, 12, 32, 17, 11);
    faces[2].Set(32, 17, 27, 3, 19);
    faces[3].Set(32, 12, 1, 28, 19);
    faces[4].Set(1, 12, 24, 2, 30);

    faces[5].Set(27, 6, 36, 11, 17);
    faces[6].Set(36, 11, 24, 18, 4);
    faces[7].Set(24, 18, 34, 14, 2);
    faces[8].Set(30, 2, 14, 26, 0);

    faces[9].Set(6, 36, 4, 29, 22);
    faces[10].Set(4, 29, 16, 34, 18);
    faces[11].Set(34, 16, 9, 26, 14);
    faces[12].Set(0, 13, 31, 20, 26);

    faces[13].Set(19, 3, 33, 10, 28);
    faces[14].Set(33, 10, 15, 25, 8);
    faces[15].Set(25, 21, 37, 23, 8);
    faces[16].Set(25, 5, 20, 31, 15);

    faces[17].Set(10, 15, 31, 13, 28);
    faces[18].Set(33, 3, 27, 23, 8);
    faces[19].Set(23, 27, 6, 22, 37);
    faces[20].Set(37, 22, 29, 7, 21);

    faces[21].Set(35, 9, 16, 29, 7);
    faces[22].Set(5, 35, 9, 26, 20);

    faces[23].Set(25, 5, 35, 7, 21);

    for (int i = 0; i < faces_count; i++)
        faces[i].SetSource(turned_vertexes);
}
