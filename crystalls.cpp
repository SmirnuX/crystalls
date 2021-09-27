#include "mainwindow.h"

Crystall::Crystall(int vertex, int edges, int n_faces, QString _name)
{
    vertex_count = vertex;   //Количество вершин
    edges_count = edges;    //Количество ребер
    faces_count = n_faces;
    name = _name;
    vertexes = new Point3D[vertex_count];
    edges_from = new int[edges_count];
    edges_to = new int[edges_count];
    faces = new Polygon3D[faces_count];
    turned_vertexes = new Point3D[vertex_count];   //Массив вершин после изменений
    Quaternion = new Matrix <double>(4,4);
    Points4D = new Matrix<double>*[vertex_count];
    for (int i = 0; i < vertex_count; i++)
        Points4D[i] = new Matrix<double>(1,4);
    Reset();
}

void Crystall::Reset()
{
    a = 0;
    b = 0;
    g = 0;
    dx = 0;
    dy = 0;
    dz = 0;
    reflectXOY = false;
    reflectXOZ = false;
    reflectYOZ = false;
    scale = 1;
    skewX = 1;
    skewY = 1;
    skewZ = 1;
}

void Crystall::Change() //Изменение фигуры
{
    //Инициализация
    for (int i = 0; i < vertex_count; i++)
    {
        Points4D[i]->data[0][0] = vertexes[i].x;
        Points4D[i]->data[0][1] = vertexes[i].y;
        Points4D[i]->data[0][2] = vertexes[i].z;
        Points4D[i]->data[0][3] = 1;
    }
    double cosa = cos(a);
    double sina = sin(a);
    double cosb = cos(b);
    double sinb = sin(b);
    double cosg  =cos(g);
    double sing = sin(g);
    //Поворот
    double TurnOX[4][4] = {     {   1,      0,      0,  0},
                                {   0, cosa,  sina, 0},
                                {   0,-sina,  cosa, 0},
                                {   0,      0,       0, 1}
                                                            };
    double TurnOY[4][4] = {     {  cosb,      0,-sinb, 0},
                                {       0,      1,      0, 0},
                                {  sinb,      0, cosb, 0},
                                {       0,      0,      0, 1}
                                                            };
    double TurnOZ[4][4] = {     {  cosg, sing,  0, 0},
                                { -sing, cosg,  0, 0},
                                {       0,      0,  1, 0},
                                {       0,      0,  0, 1}
                                                            };
    double Coefs[4][4] = { { scale * skewX * (reflectYOZ?-1.0:1.0),                               0,                               0,     0},
                           {                               0, scale * skewY * (reflectXOZ?-1.0:1.0),                               0,     0},
                           {                               0,                               0,  scale * skewZ* (reflectXOY?-1.0:1.0),     0},
                           {                              0,                               0,                             0,     1}};


    Quaternion->SetFromStaticArray(Coefs);
    for (int i = 0; i < vertex_count; i++)
        *Points4D[i]*=*Quaternion;
    Quaternion->SetFromStaticArray(TurnOX);
    for (int i = 0; i < vertex_count; i++) //По оси OX
        *Points4D[i]*=*Quaternion;
    Quaternion->SetFromStaticArray(TurnOY);
    for (int i = 0; i < vertex_count; i++)
        *Points4D[i] *= *Quaternion;
    Quaternion->SetFromStaticArray(TurnOZ);
    for (int i = 0; i < vertex_count; i++)
        *Points4D[i] *= *Quaternion;
    //� астяжение и масштаб
    double delta[4][4] = { { 1, 0, 0, 0},
                           { 0, 1, 0, 0},
                           { 0, 0, 1, 0},
                           {dx,dy,dz, 1}};
    Quaternion->SetFromStaticArray(delta);
    for (int i = 0; i < vertex_count; i++)
    {
        *Points4D[i]*=*Quaternion;
        turned_vertexes[i].Set(Points4D[i]);
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


void Crystall::Draw(QPainter* painter, struct param* settings)
{
    painter->setPen(QColor(0,0,0));
    int width = painter->window().width();
    int height = painter->window().height();
    if (settings->intersec_zbuf)   //����� �������� � ������� z-������
    {
        QImage img(width, height, QImage::Format_ARGB32);   //������������� �����������
        img.fill(0);
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
                z_buffer[i][j] = INT_MAX;
        }
        for (int i = 0; i < faces_count; i++)
        {
            faces[i].DrawZ(&img, z_buffer, width/2, height/2, true, settings->show_zbuf);
        }
        painter->drawImage(0,0,img);
    }
    else
    {
    for (int i = 0; i < edges_count; i++)
    {
            if (settings->gradient_lines)
                drawLine3D(painter, &turned_vertexes[edges_from[i]], &turned_vertexes[edges_to[i]], width/2, height/2);
            else
            {
                QPoint a(width/2 + turned_vertexes[edges_from[i]].x, height/2 + turned_vertexes[edges_from[i]].z);
                QPoint b(width/2 + turned_vertexes[edges_to[i]].x, height/2 + turned_vertexes[edges_to[i]].z);
                painter->drawLine(a, b);
            }
        }
    }
    if (settings->show_vertices)
    {
        painter->setPen(QColor(200,0,0));
        for (int i = 0; i < vertex_count; i++)
        {
            painter->drawEllipse(turned_vertexes[i].x - 2 + width/2, turned_vertexes[i].z + height/2 - 2, 4, 4);
            painter->drawText(turned_vertexes[i].x + 4 + width/2, turned_vertexes[i].z + height/2, QString::number(i));
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

    drawLine3D(painter, &center, &x, width-60, height-60, -60, 60, QColor(255,0,0));
    drawLine3D(painter, &center, &y, width-60, height-60, -60, 60, QColor(0,180,0));
    drawLine3D(painter, &center, &z, width-60, height-60, -60, 60, QColor(0,0,255));

    painter->setPen(QColor(255,0,0));
    painter->drawText(width-60 + x.x, height-60 + x.z, "x");
    painter->setPen(QColor(0,180,0));
    painter->drawText(width-60 + y.x, height-60 + y.z, "y");
    painter->setPen(QColor(0,0,255));
    painter->drawText(width-60 + z.x, height-60 + z.z, "z");


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
