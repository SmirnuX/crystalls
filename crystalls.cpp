#include "mainwindow.h"

Crystall::Crystall(int vertex, int edges, int n_faces, QString _name)
{
    vertex_count = vertex;   //РљРѕР»РёС‡РµСЃС‚РІРѕ РІРµСЂС€РёРЅ
    edges_count = edges;    //РљРѕР»РёС‡РµСЃС‚РІРѕ СЂРµР±РµСЂ
    faces_count = n_faces;
    name = _name;
    vertexes = new Point3D[vertex_count];
    edges_from = new int[edges_count];
    edges_to = new int[edges_count];
    faces = new Polygon3D[faces_count];
    turned_vertexes = new Point3D[vertex_count];   //РњР°СЃСЃРёРІ РІРµСЂС€РёРЅ РїРѕСЃР»Рµ РёР·РјРµРЅРµРЅРёР№
    normals = new Point3D[faces_count]; ;   //Нормали к граням
    vert_normals = new Point3D[vertex_count]; ;  //Нормали к вершинам
    vert_intens = new double[vertex_count];
    Quaternion = new Matrix <double>(4,4);
    Points4D = new Matrix<double>*[vertex_count];

    temp_z_buffer = NULL;
    temp_image = NULL;

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

void Crystall::Change() //РР·РјРµРЅРµРЅРёРµ С„РёРіСѓСЂС‹
{
    //РРЅРёС†РёР°Р»РёР·Р°С†РёСЏ
    for (int i = 0; i < vertex_count; i++)
    {
        Points4D[i]->data[0][0] = vertexes[i].x;
        Points4D[i]->data[0][1] = vertexes[i].y;
        Points4D[i]->data[0][2] = vertexes[i].z;
        Points4D[i]->data[0][3] = 1;
    }
    double cosa = cos(a);
    double sina = sin(a);
    double cosb = cos(-b);
    double sinb = sin(-b);
    double cosg  =cos(g);
    double sing = sin(g);
    //РџРѕРІРѕСЂРѕС‚
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
    for (int i = 0; i < vertex_count; i++) //РџРѕ РѕСЃРё OX
        *Points4D[i]*=*Quaternion;
    Quaternion->SetFromStaticArray(TurnOY);
    for (int i = 0; i < vertex_count; i++)
        *Points4D[i] *= *Quaternion;
    Quaternion->SetFromStaticArray(TurnOZ);
    for (int i = 0; i < vertex_count; i++)
        *Points4D[i] *= *Quaternion;
    //Р Р°СЃС‚СЏР¶РµРЅРёРµ Рё РјР°СЃС€С‚Р°Р±
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


void Crystall::Draw(QPainter* painter, struct param* settings, QImage** img, double*** z_buffer)
{
    painter->setPen(QColor(0,0,0));
    int width = painter->window().width();
    int height = painter->window().height();
    //Инициализация изображения и Z-буфера
    if (*img == NULL)
        *img = new QImage(width, height, QImage::Format_ARGB32);
    if (*z_buffer == NULL)
    {
        *z_buffer = new double*[height];
        for (int i = 0; i < height; i++)
            (*z_buffer)[i] = new double[width];
    }
    if (settings->intersec_zbuf)   //Метод удаления с помощью z-буфера
    {
        (*img)->fill(0);
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
                (*z_buffer)[i][j] = INT_MAX;
        }
        for (int i = 0; i < faces_count; i++)
        {
            if (settings->show_faces)
                faces[i].DrawZ(*img, *z_buffer, width/2, height/2, settings->faces_shade, settings->show_zbuf, settings->faces_color);
            else
                faces[i].DrawZ(*img, *z_buffer, width/2, height/2, false, settings->show_zbuf, qRgb(255, 255, 255));
        }
        painter->drawImage(0,0,**img);
    }
    else if (settings->intersec_veyler) //Метод Вейлера-Азертона
    {  
        (*img)->fill(0);    //Инициализация изображения
        DrawPseudoWeyler(painter);
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
                (*z_buffer)[i][j] = INT_MAX;
        }
        for (int i = 0; i < faces_count; i++)
        {
            faces[i].DrawZ(*img, *z_buffer, width/2, height/2, false, false, qRgb(255, 255, 255));
        }
        painter->drawImage(0,0,**img);
    }
    else if (settings->guro)    //Метод Гуро
    {
        (*img)->fill(0);    //Инициализация изображения
        for (int i = 0; i < height; i++)    //Инициализация Z-буфера
        {
            for (int j = 0; j < width; j++)
                (*z_buffer)[i][j] = INT_MAX;
        }
        int* count = new int[vertex_count];
        for (int i = 0; i < vertex_count; i++)
        {
            vert_normals[i].x = 0;
            vert_normals[i].y = 0;
            vert_normals[i].z = 0;
            count[i] = 0;
        }
        //Подсчет нормалей к граням
        for (int i = 0; i < faces_count; i++)
        {
            //faces[i].SetSource(turned_vertexes);
            Point3D A(faces[i].Point(1).x - faces[i].Point(0).x,
                      faces[i].Point(1).y - faces[i].Point(0).y,
                      faces[i].Point(1).z - faces[i].Point(0).z);
            Point3D B(faces[i].Point(2).x - faces[i].Point(1).x,
                      faces[i].Point(2).y - faces[i].Point(1).y,
                      faces[i].Point(2).z - faces[i].Point(1).z);
            Point3D Norm(det(A.y, A.z, B.y, B.z),
                         -det(A.x, A.z, B.x, B.z),
                         det(A.x, A.y, B.x, B.y));
            double len = sqrt(Norm.x * Norm.x + Norm.y * Norm.y + Norm.z * Norm.z);
            normals[i].Set(Norm.x / len, Norm.y / len, Norm.z / len);
            for (int j = 0; j < faces[i].size; j++)
            {
                int k = faces[i].points[j];
                count[k]++;
                vert_normals[k].x += normals[i].x;
                vert_normals[k].y += normals[i].y;
                vert_normals[k].z += normals[i].z;
            }
        }
        //Подсчет нормалей к вершинам

        for (int i = 0; i < vertex_count; i++)
        {
            if (count[i] > 0)   //Нормализация
            {
                /*
                vert_normals[i].x /= count[i];
                vert_normals[i].y /= count[i];
                vert_normals[i].z /= count[i];
                */
                double len = vert_normals[i].x * vert_normals[i].x +
                             vert_normals[i].y * vert_normals[i].y +
                             vert_normals[i].z * vert_normals[i].z;
                vert_normals[i].x /= len;
                vert_normals[i].y /= len;
                vert_normals[i].z /= len;
            }

            Point3D dist(   settings->light_x - turned_vertexes[i].x,
                            settings->light_y - turned_vertexes[i].y,
                            settings->light_z - turned_vertexes[i].z);
            double dist_len = sqrt(dist.x * dist.x + dist.y * dist.y + dist.z * dist.z);
            dist.Set(dist.x / dist_len, dist.y / dist_len, dist.z / dist_len);
            double diffuse = vert_normals[i].x * dist.x +
                             vert_normals[i].y * dist.y +
                             vert_normals[i].z * dist.z;
            vert_intens[i] = settings->k_amb + settings->k_diff * diffuse;
            if (vert_intens[i] < 0)
                vert_intens[i] = 0;
            if (vert_intens[i] > 1)
                vert_intens[i] = 1;
        }


        for (int i = 0; i < faces_count; i++)
        {
            DrawInterpolated(*img, i, *z_buffer, settings->faces_color);
        }
        painter->drawImage(0,0,**img);

        if (settings->show_normals)
        {
            for (int i = 0; i < vertex_count; i++)
            {
                int vx = turned_vertexes[i].x + width/2;
                int vy = turned_vertexes[i].z + height/2;
                int epsilon = 5;
                if (vx < 0 || vx >= width || vy < 0 || vy >= height)
                    continue;
                if (turned_vertexes[i].y - epsilon > (*z_buffer)[vy][vx])
                    continue;
                painter->setBrush(QColor((int)(vert_intens[i] * 255), (int)(vert_intens[i] * 255), (int)(vert_intens[i] * 255)));
                painter->drawEllipse(width/2 + turned_vertexes[i].x - 5, height/2 + turned_vertexes[i].z - 5, 10, 10);
                Point3D NormalAdd(turned_vertexes[i].x + 50 * vert_normals[i].x,
                                  turned_vertexes[i].y + 50 * vert_normals[i].y,
                                  turned_vertexes[i].z + 50 * vert_normals[i].z);
                drawLine3D(painter, turned_vertexes + i, &NormalAdd, width/2, height/2, -100, 100, QColor(255,0,0));
            }
        }



        /*for (int i = 0; i < edges_count; i++)   //Тестовая отрисовка многогранника
        {
            drawLine3D(painter, &turned_vertexes[edges_from[i]], &turned_vertexes[edges_to[i]], width/2, height/2);
        }*/

        delete[] count;
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
    if (settings->show_vertices)    //Отрисовка вершин
    {
        painter->setPen(QColor(0,0,200));
        for (int i = 0; i < vertex_count; i++)
        {
            int vx = turned_vertexes[i].x + width/2;
            int vy = turned_vertexes[i].z + height/2;
            int epsilon = 5;
            if (settings->show_zbuf_numbers && settings->intersec_zbuf)
            {
                if (vx < 0 || vx >= width || vy < 0 || vy >= height)
                    continue;
                if (turned_vertexes[i].y - epsilon > (*z_buffer)[vy][vx])
                    continue;
            }
            painter->drawEllipse(vx-1, vy-1, 2, 2);
            painter->drawText(vx+3, vy, QString::number(i));
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
    z.Set(-x_k*(-cos(a)*sin(b)*cos(g)+sin(a)*sin(g)), -y_k*(cos(a)*sin(b)*sin(g)+sin(a)*cos(g)), -z_k*cos(a)*cos(b));  //РћСЃСЊ РёРЅРІРµСЂС‚РёСЂРѕРІР°РЅР°

    drawLine3D(painter, &center, &x, width-60, height-60, 0, 60, QColor(255,0,0));
    drawLine3D(painter, &center, &y, width-60, height-60, 0, 60, QColor(0,180,0));
    drawLine3D(painter, &center, &z, width-60, height-60, 0, 60, QColor(0,0,255));

    /*
    if (settings->guro)
    {
        Point3D light(settings->light_x/20, settings->light_y/20, settings->light_z/20);
        drawLine3D(painter, &center, &light, width-59, height-59, 0, 60, QColor(200,200,0));
        drawLine3D(painter, &center, &light, width-60, height-60, 0, 60, QColor(255,255,0));
    }*/

    painter->setPen(QColor(255,0,0));
    painter->drawText(width-60 + x.x, height-60 + x.z, "x");
    painter->setPen(QColor(0,180,0));
    painter->drawText(width-60 + y.x, height-60 + y.z, "y");
    painter->setPen(QColor(0,0,255));
    painter->drawText(width-60 + z.x, height-60 + z.z, "z");


    //РћС‚СЂР°Р¶РµРЅРёРµ

}

/*
void Crystall::DrawInterpolated(QImage* img, int num, double** z, QRgb color)
{
    int min_y = INT_MAX, max_y = INT_MIN, min_x = INT_MAX, max_x = INT_MIN;
    for (int i = 0; i < faces[num].size; i++)
    {
        if (faces[num].Point(i).x < min_x)
            min_x = faces[num].Point(i).x;
        if (faces[num].Point(i).x > max_x)
            max_x = faces[num].Point(i).x;
        if (faces[num].Point(i).z < min_y)
            min_y = faces[num].Point(i).z;
        if (faces[num].Point(i).z > max_y)
            max_y = faces[num].Point(i).z;
    }
    int padding = 10;
    QImage buffer(max_x - min_x + 2*padding, max_y - min_y + 2*padding, QImage::Format_ARGB32);
    buffer.fill(qRgba(0, 0, 0, 0));

    double** z_buffer = new double*[max_y - min_y + 2*padding];
    for (int i = 0; i < max_y - min_y + 2*padding; i++)
    {
        z_buffer[i] = new double[max_x - min_x + 2*padding];
        for (int j = 0; j < max_x - min_x + 2*padding; j++)
            z_buffer[i][j] = INT_MAX;
    }

    for (int i = 0; i < faces[num].size; i++)  //Отрисовка границ многоугольника
    {
        Point3D a, b;
        a = faces[num].Point(i);
        if (i == faces[num].size-1)
            b = faces[num].Point(0);
        else
            b = faces[num].Point(i+1);
        drawLine3D(&buffer, &a, &b, -min_x + padding, -min_y + padding, 0, 100, QColor(0,0,0), z_buffer);
    }
    //Закраска
    for (int i = 0; i < buffer.height(); i++)
    {
        bool inside = false;
        bool stripe = false;
        int sx=0, ex=0;
        double sz=0, ez=0;
        for (int j = 0; j < buffer.width(); j++)
        {
            if (qAlpha(buffer.pixel(j, i)) > 0)
            {
                if (!stripe)
                {
                    inside = !inside;
                    stripe = true;
                }
                if (inside)    //Закраска изнутри
                {
                    sx = j+1;
                    sz = z_buffer[i][j];
                }
                else
                {
                    ex = j;
                    ez = z_buffer[i][j];
                    for (int k = sx; k < ex; k++)
                    {
                        if (sx != ex)
                            z_buffer[i][k] = (sz + (ez-sz) * (k - sx)/(ex-sx));
                        else
                            z_buffer[i][k] = sz;
                        //buffer.setPixel(k, i, faces_color);
                    }
                    inside = !inside;
                }
                continue;
            } else if (stripe)
            {
                stripe = false;
            }
        }
    }
    double max_dist = 0;
    for (int i = 0; i < faces[num].size; i++)
    {
        for (int j = i+1; j < faces[num].size; j++)
        {
            double dist =   (faces[num].Point(i).x - faces[num].Point(j).x)*(faces[num].Point(i).x - faces[num].Point(j).x) +
                            (faces[num].Point(i).z - faces[num].Point(j).z)*(faces[num].Point(i).z - faces[num].Point(j).z);
            if (dist > max_dist)
                max_dist = dist;
        }
    }
    //qDebug() << max_dist;

    for (int i = 0; i < buffer.height(); i++)   //Применение к изображению
    {
        for (int j = 0; j < buffer.width(); j++)
        {
            int screen_x = j + min_x + img->width()/2 - padding;
            int screen_y = i + min_y + img->height()/2 - padding;

            if (screen_x >= 0 && screen_x < img->width() && screen_y >= 0 && screen_y < img->height())
            {
                if (z_buffer[i][j] < z[screen_y][screen_x])
                {
                    //Отрисовка пикселя
                    z[screen_y][screen_x] = z_buffer[i][j]; //Перезапись z-буфера

                    //Вычисление яркости
                    int r = qRed(color);
                    int g = qGreen(color);
                    int b = qBlue(color);
                    double* dist = new double[faces[num].size];
                    double sum = 0;
                    for (int j = 0; j < faces[num].size; j++)   //Суммарная дистанция
                    {
                        //dist[j] = sqrt( (screen_x - faces[num].Point(j).x)*(screen_x - faces[num].Point(j).x) + (screen_y - faces[num].Point(j).z)*(screen_y - faces[num].Point(j).z));
                        dist[j] = sqrt( (screen_x - faces[num].Point(j).x)*(screen_x - faces[num].Point(j).x) + (screen_y - faces[num].Point(j).z)*(screen_y - faces[num].Point(j).z) );
                        //sum += dist[j];
                    }
                    double intensity = 0;
                    for (int j = 0; j < faces[num].size; j++)
                        intensity += vert_intens[faces[num].points[j]] * ( 1 - 2 * dist[j]/max_dist);

                    if (intensity < 0)
                        intensity = 0;
                    if (intensity > 1)
                        intensity = 1;

                    img->setPixel(screen_x, screen_y, qRgb(intensity * r,
                                                           intensity * g,
                                                           intensity * b
                                                           ));
                    delete[] dist;


                }
            }
        }
    }

    for (int i = 0; i < max_y - min_y + 2*padding; i++)
        delete z_buffer[i];
    delete[] z_buffer;
}*/


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
    delete[] normals;
    delete[] vert_normals;
    delete[] vert_intens;
}
