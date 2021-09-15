#include "mainwindow.h"

Smirnov1::Smirnov1() : Crystall(8,18,QString::fromLocal8Bit("�������������� ����������"))
{
    CalculatePoints();
    for (int i = 0; i < vertex_count; i++)
        turned_vertexes[i].Set(vertexes[i].x, vertexes[i].y, vertexes[i].z);
}

void Smirnov1::CalculatePoints()
{
    int height = 200;
    int radius = 90;
    int smaller_radius = 75;
    vertexes[0].Set(0, 0, height);
    //��� �������������� �������������� � ������� ��������� ��������� �����������
    vertexes[1].Set(radius * sin(0), radius * cos(0), 0);
    vertexes[3].Set(radius * sin(2 * M_PI / 3), radius * cos(2 * M_PI / 3), 0);
    vertexes[5].Set(radius * sin(4 * M_PI / 3), radius * cos(4 * M_PI / 3), 0);

    vertexes[2].Set(smaller_radius * sin(M_PI/3), smaller_radius * cos(M_PI/3), 0);
    vertexes[4].Set(smaller_radius * sin(M_PI), smaller_radius * cos(M_PI), 0);
    vertexes[6].Set(smaller_radius * sin(5 * M_PI / 3), smaller_radius * cos(5 * M_PI / 3), 0);

    vertexes[7].Set(0, 0, -height);

    AddEdge(0, 1, 2);
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

}

Smirnov2::Smirnov2() : Crystall(38, 60, QString::fromLocal8Bit("������������������"))
{
    CalculatePoints();
    for (int i = 0; i < vertex_count; i++)
        turned_vertexes[i].Set(vertexes[i].x, vertexes[i].y, vertexes[i].z);
}

void Smirnov2::CalculatePoints()
{
    double t = 1.839286;    //��������� ����������
    double scale = 10;
    double x, y, z;
    x = scale;
    y = scale*(2*t +1);
    z = scale*(t * t);
    //������ 12 ������ - ������ ������������ (+-1; +-2t+1; +-t^2)� ������ ���-��� �������
    //������ ������������ - x y z, z x y, y z x
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
    //������ 12 ������ - ������ ������������ (+-1; +-2t+1; +-t^2)� ������ ���-��� �������
    //�������� ������������ - y x z, x z y, z y x
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
    //6 ������ ��������
    double t3 = scale*(t * t * t);
    vertexes[24].Set(t3, 0, 0);
    vertexes[25].Set(-t3,0,0);
    vertexes[26].Set(0, t3, 0);
    vertexes[27].Set(0, -t3, 0);
    vertexes[28].Set(0, 0, t3);
    vertexes[29].Set(0, 0, -t3);
    //8 ������ ����
    double t2 = scale*(t*t);
    vertexes[30].Set(t2, t2, t2);
    vertexes[31].Set(-t2, t2, t2);
    vertexes[32].Set(t2, -t2, t2);
    vertexes[33].Set(-t2, -t2, t2);
    vertexes[34].Set(t2, t2, -t2);
    vertexes[35].Set(-t2, t2, -t2);
    vertexes[36].Set(t2, -t2, -t2);
    vertexes[37].Set(-t2, -t2, -t2);

    //����� - ����� 60
    //������ ������������
    AddEdge(0, 1, 30);
    AddEdge(1,30,0);
    AddEdge(2,0,13);
    AddEdge(3,13,28);
    AddEdge(4,28,1);
    //������ ������������
    AddEdge(5,2,30);
    AddEdge(6,2,14);
    AddEdge(7,14,26);
    AddEdge(8,26,0);
    //������ ������������
    AddEdge(9,12,24);
    AddEdge(10,24,2);
    AddEdge(11,27,3);
    AddEdge(12,1,12);
    //��������� ������������
    AddEdge(13,13,31);
    AddEdge(14,31,20);
    AddEdge(15,20,26);
    //�����
    AddEdge(16,14,34);
    AddEdge(17,34,18);
    AddEdge(18,18,24);
    //������
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
}
