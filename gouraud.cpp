#include "mainwindow.h"

void Crystall::DrawInterpolated(QImage* img, int num, double** z, QRgb color)   //Отрисовка грани методом Гуро
{
    int min_y = INT_MAX, max_y = INT_MIN, min_x = INT_MAX, max_x = INT_MIN; //Границы 2D полигона
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

    double** z_buffer = new double*[max_y - min_y + 2*padding]; //Инициализация локального Z-буфера
    for (int i = 0; i < max_y - min_y + 2*padding; i++)
    {
        z_buffer[i] = new double[max_x - min_x + 2*padding];
        for (int j = 0; j < max_x - min_x + 2*padding; j++)
            z_buffer[i][j] = INT_MAX;
    }

    for (int i = 0; i < faces[num].size; i++)  //Отрисовка границ многоугольника - линейно интерполируя яркость
    {
        Point3D a, b;
        int j = i+1;
        a = faces[num].Point(i);
        if (i == faces[num].size-1)
        {
            b = faces[num].Point(0);
            j = 0;
        }
        else
            b = faces[num].Point(i+1);
        drawLine3D(&buffer, &a, &b, -min_x + padding,   -min_y + padding, color, z_buffer,
                                                        vert_intens[faces[num].points[i]],
                                                        vert_intens[faces[num].points[j]]);
    }

    //Закраска с линейной интерполяцией цвета
    for (int i = 0; i < buffer.height(); i++)
    {
        bool inside = false;
        bool stripe = false;
        int sx=0, ex=0;
        int sr = 0, sg = 0, sb = 0; //Цвета на старте
        int er = 0, eg = 0, eb = 0; //Цвета на выходе
        double sz=0, ez=0;
        for (int j = 0; j < buffer.width(); j++)
        {
            if (qAlpha(buffer.pixel(j, i)) > 0)
            {
                if (!stripe)    //Вход и выход в замкнутую область
                {
                    inside = !inside;
                    stripe = true;
                }
                if (inside)    //Начало замкнутой области
                {
                    sx = j+1;
                    sz = z_buffer[i][j];
                    sr = qRed(buffer.pixel(j, i));
                    sg = qGreen(buffer.pixel(j, i));
                    sb = qBlue(buffer.pixel(j, i));
                }
                else
                {   //Конец
                    ex = j;
                    ez = z_buffer[i][j];
                    er = qRed(buffer.pixel(j, i));
                    eg = qGreen(buffer.pixel(j, i));
                    eb = qBlue(buffer.pixel(j, i));
                    int r = sr, g = sg, b = sb;
                    for (int k = sx; k < ex; k++)   //Непосредственно закраска
                    {
                        if (sx != ex)
                        {
                            z_buffer[i][k] = (sz + (ez-sz) * (k - sx)/(ex-sx));
                            r = (sr + (er - sr) * (k - sx)/(ex-sx));
                            g = (sg + (eg - sg) * (k - sx)/(ex-sx));
                            b = (sb + (eb - sb) * (k - sx)/(ex-sx));
                        }
                        else
                        {
                            z_buffer[i][k] = sz;
                        }
                        buffer.setPixel(k, i, qRgb(r, g, b));
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
                    img->setPixel(screen_x, screen_y, buffer.pixel(j, i));
                    /*
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
                    delete[] dist;*/


                }
            }
        }
    }

    for (int i = 0; i < max_y - min_y + 2*padding; i++)
        delete z_buffer[i];
    delete[] z_buffer;
}


void drawLine3D(QImage* painter, Point3D* a, Point3D* b, int dx, int dy, QRgb color, double** z, double a_int, double b_int)
{
    double z_delta = b->z - a->z;   //Разница в координатах z
    double x_delta = b->x - a->x;   //Разница в координатах x
    double int_delta = b_int - a_int;   //Разница между яркостями
    if (x_delta == 0)
        x_delta = 0.1;
    double d = z_delta / qAbs(x_delta); //Производная dz/dx
    bool x_tick = true; //Отсчет идет по оси X
    double d_y; //Производная y (dy/dx OR dy/dz) (т.е. глубины)
    double d_i; //Производная яркости
    double c, c_max;    //Начальное и конечное значения итерационной переменной
    double qt, y, in;       //Стартовые значения интерполируемых значений
    if (d < -1 || d > 1)    //Вычисление обратной производной для вертикальных линий
    {
        x_tick = false;
        if (z_delta == 0)
            z_delta = 0.1;
        d = x_delta/qAbs(z_delta);  //Производная dx/dz
        d_y = (b->y - a->y)/z_delta;    //Производная dy/dz
        d_i = int_delta/z_delta;    //  di/dz
        if (z_delta < 0)
        {
            c = b->z;
            c_max = a->z;
            d = -d;
            qt = b->x;
            y = b->y;
            in = b_int;
        }
        else
        {
            c = a->z;
            c_max = b->z;
            qt = a->x;
            y = a->y;
            in = a_int;
        }
    }
    else
    {
        d_y = (b->y - a->y)/x_delta;
        d_i = int_delta/x_delta;    //  di/dz
        if (x_delta < 0)
        {
            c = b->x;
            c_max = a->x;
            d = -d;
            qt = b->z;
            y = b->y;
            in = b_int;
        }
        else
        {
            c = a->x;
            c_max = b->x;
            qt = a->z;
            y = a->y;
            in = a_int;
        }
    }

    //Отрисовка начальной и конечной точки
    painter->setPixel(dx + a->x, dy + a->z, qRgb(qRed(color)* a_int, qGreen(color)* a_int, qBlue(color)* a_int));

    painter->setPixel(dx + b->x, dy + b->z, qRgb(qRed(color)* b_int, qGreen(color)* b_int, qBlue(color)* b_int));
    if (z != NULL)
    {
        if ((int)(dy + a->z) >= 0 && (int)(dy + a->z) < painter->height() && (int)(dx + a->x) >= 0 && (int)(dx + a->x) < painter->width())
            z[(int)(dy + a->z)][(int)(dx + a->x)]= a->y;
        if ((int)(dy + b->z) >= 0 && (int)(dy + b->z) < painter->height() && (int)(dx + b->x) >= 0 && (int)(dx + b->x) < painter->width())
            z[(int)(dy + b->z)][(int)(dx + b->x)]= b->y;
    }

    for (; c <= ceil(c_max); c++)
    {
        if (in < 0)
            in = 0;
        if (in > 1)
            in = 1;
        QRgb _color = qRgb(qRed(color)* in, qGreen(color)* in, qBlue(color)* in);
        //if (z != NULL)
            //_color = qRgba(0, 0, 0, 255);
        int _x, _y;
        if (x_tick)
        {
            _x = c+dx;
            _y = qt+dy;
        }
        else
        {
            _x = qt+dx;
            _y = c+dy;
        }
        if (_x >= 0 && _x< painter->width() && _y >= 0 && _y < painter->height())
        {
            painter->setPixel(_x, _y, _color);
            if (z != NULL)
                z[_y][_x]=y;
        }
        qt += d;
        y += d_y;
        in += d_i;
    }
}

