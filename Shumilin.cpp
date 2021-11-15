#include "mainwindow.h"

Shumilin1::Shumilin1() : Crystall(9,16,9,QString::fromLocal8Bit("Дитетрагональная пирамида"))
{
    CalculatePoints();
    for (int i = 0; i < vertex_count; i++)
        turned_vertexes[i].Set(vertexes[i].x, vertexes[i].y, vertexes[i].z);
}

void Shumilin1::CalculatePoints()
{

    vertexes[0].Set(0, 0, 200);
    vertexes[1].Set(38, -92, 0);
    vertexes[2].Set(92, -38, 0);
    vertexes[3].Set(92, 38, 0);
    vertexes[4].Set(38, 92, 0);
    vertexes[5].Set(-38, 92, 0);
    vertexes[6].Set(-92, 38,0);
    vertexes[7].Set(-92, -38, 0);
    vertexes[8].Set(-38, -92, 0);

    AddEdge(0, 0, 1);
    AddEdge(1, 0, 2);
    AddEdge(2, 0, 3);
    AddEdge(3, 0, 4);
    AddEdge(4, 0, 5);
    AddEdge(5, 0, 6);
    AddEdge(6, 0, 7);
    AddEdge(7, 0, 8);
    AddEdge(8, 8, 1);
    AddEdge(9, 1, 2);
    AddEdge(10, 2, 3);
    AddEdge(11, 3, 4);
    AddEdge(12, 4, 5);
    AddEdge(13, 5, 6);
    AddEdge(14, 6, 7);
    AddEdge(15, 7, 8);

    faces[0].Set(0,1,2);
    faces[1].Set(0,2,3);
    faces[2].Set(0,3,4);
    faces[3].Set(0,4,5);
    faces[4].Set(0,5,6);
    faces[5].Set(0,6,7);
    faces[6].Set(0,7,8);
    faces[7].Set(0,8,1);

    int pts[8] = {8, 7, 6, 5, 4, 3, 2, 1};

    faces[8].Set(pts, 8);

    for (int i = 0; i < faces_count; i++)
        faces[i].SetSource(turned_vertexes);

}

Shumilin2::Shumilin2() : Crystall(26, 144, 24, QString::fromLocal8Bit("Гексоктаэдр"))
{
    CalculatePoints();
    for (int i = 0; i < vertex_count; i++)
        turned_vertexes[i].Set(vertexes[i].x, vertexes[i].y, vertexes[i].z);
}

void Shumilin2::CalculatePoints()
{
    vertexes[0].Set(-97, 97, -97);
    vertexes[1].Set(0, 113, -113);
    vertexes[2].Set(0, 0, -184);
    vertexes[3].Set(0, 0, 184);
    vertexes[4].Set(0, 113, 113);
    vertexes[5].Set(-97, 97, 97);
    vertexes[6].Set(0, -113,-113);
    vertexes[7].Set(-97, -97, -97);
    vertexes[8].Set(-97, -97, 97);
    vertexes[9].Set(0, -131, 131);
    vertexes[10].Set(0, -184, 0);
    vertexes[11].Set(0, 184, 0);
    vertexes[12].Set(97, 97, -97);
    vertexes[13].Set(97, 97, 97);
    vertexes[14].Set(97, -97, -97);
    vertexes[15].Set(97, -97, 97);
    vertexes[16].Set(131, 0, -131);
    vertexes[17].Set(131, 0, 131);
    vertexes[18].Set(131, -131, 0);
    vertexes[19].Set(131, 131, 0);
    vertexes[20].Set(-131, 0, -131);
    vertexes[21].Set(-131, 0, 131);
    vertexes[22].Set(-131, -131, 0);
    vertexes[23].Set(-131, 131, 0);
    vertexes[24].Set(-184, 0, 0);
    vertexes[25].Set(184, 0, 0);

    AddEdge(0, 0, 1);
    AddEdge(1, 1, 2);
    AddEdge(2, 2, 0);

    AddEdge(3, 3, 4);
    AddEdge(4, 4, 5);
    AddEdge(5, 5, 3);

    AddEdge(6, 6, 7);
    AddEdge(7, 7, 2);
    AddEdge(8, 2, 6);

    AddEdge(9, 3, 8);
    AddEdge(10, 8, 9);
    AddEdge(11, 9, 3);

    AddEdge(12, 10, 9);
    AddEdge(13, 9, 8);
    AddEdge(14, 8, 10);

    AddEdge(15, 7, 6);
    AddEdge(16, 6, 10);
    AddEdge(17, 10, 7);

    AddEdge(18, 11, 5);
    AddEdge(19, 5, 4);
    AddEdge(20, 4, 11);
    AddEdge(21, 1, 0);
    AddEdge(22, 0, 11);

    AddEdge(23, 11, 1);
    AddEdge(24, 2, 1);
    AddEdge(25, 1, 12);

    AddEdge(26, 12, 2);
    AddEdge(27, 13, 4);
    AddEdge(28, 4, 3);

    AddEdge(29, 3, 13);
    AddEdge(30, 6, 2);
    AddEdge(31, 2, 14);

    AddEdge(32, 14, 6);
    AddEdge(33, 15, 3);
    AddEdge(34, 3, 9);

    AddEdge(35, 9, 15);
    AddEdge(36, 15, 9);
    AddEdge(37, 9, 10);

    AddEdge(38, 10, 15);
    AddEdge(39, 10, 6);
    AddEdge(40, 6, 14);

    AddEdge(41, 14, 10);
    AddEdge(42, 13, 11);
    AddEdge(43, 11, 4);

    AddEdge(44, 4, 13);
    AddEdge(45, 1, 11);
    AddEdge(46, 11, 12);
    AddEdge(47, 12, 1);

    AddEdge(48, 14, 2);
    AddEdge(49, 2, 16);
    AddEdge(50, 16, 14);

    AddEdge(51, 17, 3);
    AddEdge(52, 3, 15);
    AddEdge(53, 15, 17);

    AddEdge(54, 2, 12);
    AddEdge(55, 12, 16);
    AddEdge(56, 16, 2);

    AddEdge(57, 17, 13);
    AddEdge(58, 13, 3);
    AddEdge(59, 3, 17);

    AddEdge(60, 10, 14);
    AddEdge(61, 14, 18);
    AddEdge(62, 18, 10);

    AddEdge(63, 18, 15);
    AddEdge(64, 15, 10);
    AddEdge(65, 10, 18);

    AddEdge(66, 12, 11);
    AddEdge(67, 11, 19);
    AddEdge(68, 19, 12);

    AddEdge(69, 19, 11);
    AddEdge(70, 11, 13);
    AddEdge(71, 13, 19);
    AddEdge(72, 7, 20);

    AddEdge(73, 20, 2);
    AddEdge(74, 2, 7);
    AddEdge(75, 3, 21);

    AddEdge(76, 21, 8);
    AddEdge(77, 8, 3);
    AddEdge(78, 20, 0);

    AddEdge(79, 0, 2);
    AddEdge(80, 2, 20);
    AddEdge(81, 3, 5);

    AddEdge(82, 5, 21);
    AddEdge(83, 21, 3);
    AddEdge(84, 22, 7);

    AddEdge(85, 7, 10);
    AddEdge(86, 10, 22);
    AddEdge(87, 10, 8);

    AddEdge(88, 8, 22);
    AddEdge(89, 22, 10);
    AddEdge(90, 0, 23);

    AddEdge(91, 23, 11);
    AddEdge(92, 11, 0);
    AddEdge(93, 11, 23);

    AddEdge(94, 23, 5);
    AddEdge(95, 5, 11);
    AddEdge(96, 8, 21);

    AddEdge(97, 21, 24);
    AddEdge(98, 24, 8);
    AddEdge(99, 24, 20);

    AddEdge(100, 20, 7);
    AddEdge(101, 7, 24);
    AddEdge(102, 5, 24);

    AddEdge(103, 24, 21);
    AddEdge(104, 21, 5);
    AddEdge(105, 20, 24);

    AddEdge(106, 24, 0);
    AddEdge(107, 0, 20);
    AddEdge(108, 24, 23);

    AddEdge(109, 23, 0);
    AddEdge(110, 0, 24);
    AddEdge(111, 5, 23);

    AddEdge(112, 23, 24);
    AddEdge(113, 24, 5);
    AddEdge(114, 22, 24);

    AddEdge(115, 24, 7);
    AddEdge(116, 7, 22);
    AddEdge(117, 8, 24);
    AddEdge(118, 24, 22);

    AddEdge(119, 22, 8);
    AddEdge(120, 25, 17);
    AddEdge(121, 17, 15);

    AddEdge(122, 15, 25);
    AddEdge(123, 14, 16);
    AddEdge(124, 16, 25);
    AddEdge(125, 25, 14);

    AddEdge(126, 25, 13);
    AddEdge(127, 13, 17);
    AddEdge(128, 17, 25);

    AddEdge(129, 16, 12);
    AddEdge(130, 12, 25);
    AddEdge(131, 25, 16);

    AddEdge(132, 12, 19);
    AddEdge(133, 19, 25);
    AddEdge(134, 25, 12);

    AddEdge(135, 25, 19);
    AddEdge(136, 19, 13);
    AddEdge(137, 13, 25);

    AddEdge(138, 18, 14);
    AddEdge(139, 14, 25);
    AddEdge(140, 25, 18);

    AddEdge(141, 25, 15);
    AddEdge(142, 15, 18);
    AddEdge(143, 18, 25);

    faces[0].Set(10,6,14,18);
    faces[1].Set(10,18,15,9);
    faces[2].Set(10,9,8,22);
    faces[3].Set(10,22,7,6);

    faces[4].Set(24,20,7,22);
    faces[5].Set(24,22,8,21);
    faces[6].Set(24,21,5,23);
    faces[7].Set(24,23,0,20);

    faces[8].Set(2,1,12,16);
    faces[9].Set(2,16,14,6);
    faces[10].Set(2,6,7,20);
    faces[11].Set(2,20,0,1);

    faces[12].Set(25,19,13,17);
    faces[13].Set(25,17,15,18);
    faces[14].Set(25,18,14,16);
    faces[15].Set(25,16,12,19);

    faces[16].Set(3,4,5,21);
    faces[17].Set(3,21,8,9);
    faces[18].Set(3,9,15,17);
    faces[19].Set(3,17,13,4);

    faces[20].Set(11,1,0,23);
    faces[21].Set(11,23,5,4);
    faces[22].Set(11,4,13,19);
    faces[23].Set(11,19,12,1);


    for (int i = 0; i < faces_count; i++)
        faces[i].SetSource(turned_vertexes);


}
