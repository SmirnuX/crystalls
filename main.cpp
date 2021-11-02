#include <QtGui/QApplication>
#include "mainwindow.h"
#include "matrix.h"

bool lowres;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    lowres = false;
    MainWindow w;
    w.show();

    return a.exec();
}

Matrix<double>* Point3D::to4D() //Преобразует точку в четырехмерную матрицу
{
    double* data = new double[4];
    data[0] = x;
    data[1] = y;
    data[2] = z;
    data[3] = 1;
    Matrix<double>* res = new Matrix<double>(1,4,&data);
    delete[] data;
    return res;
}

QString Point3D::Show()
{
    QString a = "{" + QString::number(x, 'g', 3).rightJustified(8, ' ', true) + "; "
                    + QString::number(y, 'g', 3).rightJustified(8, ' ', true) + "; "
                    + QString::number(z, 'g', 3).rightJustified(8, ' ', true) + "}";
    return a;
}

Point3D::Point3D()
{
    x = 0;
    y = 0;
    z = 0;
}

void Point3D::Set(double _x, double _y, double _z)
{
    x = _x;
    y = _y;
    z = _z;
}

void Point3D::Set(Matrix<double>* vec)
{
    x = vec->operator [](0)[0];
    y = vec->operator [](0)[1];
    z = vec->operator [](0)[2];
}

void drawLine3D(QPainter* painter, Point3D* a, Point3D* b, int dx, int dy, int y_min, int y_max, QColor color)
{
    double z_delta = b->z - a->z;   //Разница в координатах z
    double x_delta = b->x - a->x;   //Разница в координатах x
    if (x_delta == 0)
        x_delta = 1;
    double d = z_delta / qAbs(x_delta);
    double d_y; //Производная y (dy/dx OR dy/dz)
    bool x_tick = true;
    double c, c_max, qt, y;    //
    if (d < -1 || d > 1)    //Обратная производная
    {
        x_tick = false;
        if (z_delta == 0)
            z_delta = 1;
        d = x_delta/qAbs(z_delta);
        d_y = (b->y - a->y)/z_delta;
        if (z_delta < 0)
        {
            c = b->z;
            c_max = a->z;
            d = -d;
            qt = b->x;
            y = b->y;
        }
        else
        {
            c = a->z;
            c_max = b->z;
            qt = a->x;
            y = a->y;
        }
    }
    else
    {
        d_y = (b->y - a->y)/x_delta;
        if (x_delta < 0)
        {
            c = b->x;
            c_max = a->x;
            d = -d;
            qt = b->z;
            y = b->y;
        }
        else
        {
            c = a->x;
            c_max = b->x;
            qt = a->z;
            y = a->y;
        }
    }

    for (; c < c_max; c++)
    {
        float k = (y - y_min) / (y_max - y_min);
        if (k < 0.1)
            k = 0.1;
        if (k > 1)
            k = 1;
        painter->setPen(QColor(color.red(), color.green(), color.blue(), 255 * (1-k)));
        if (x_tick)
            painter->drawPoint(c + dx, qt + dy);
        else
            painter->drawPoint(qt + dx, c + dy);
        qt += d;
        y += d_y;
    }
}

void drawLine3D(QImage* painter, Point3D* a, Point3D* b, int dx, int dy, int y_min, int y_max, QColor color, double** z)
{
    double z_delta = b->z - a->z;   //Разница в координатах z
    double x_delta = b->x - a->x;   //Разница в координатах x
    if (x_delta == 0)
        x_delta = 0.1;
    double d = z_delta / qAbs(x_delta);
    bool x_tick = true;
    double d_y; //Производная y (dy/dx OR dy/dz)
    double c, c_max, qt, y;    //
    if (d < -1 || d > 1)    //Обратная производная
    {
        x_tick = false;
        if (z_delta == 0)
            z_delta = 0.1;
        d = x_delta/qAbs(z_delta);
        d_y = (b->y - a->y)/z_delta;
        if (z_delta < 0)
        {
            c = b->z;
            c_max = a->z;
            d = -d;
            qt = b->x;
            y = b->y;
        }
        else
        {
            c = a->z;
            c_max = b->z;
            qt = a->x;
            y = a->y;
        }
    }
    else
    {
        d_y = (b->y - a->y)/x_delta;
        if (x_delta < 0)
        {
            c = b->x;
            c_max = a->x;
            d = -d;
            qt = b->z;
            y = b->y;
        }
        else
        {
            c = a->x;
            c_max = b->x;
            qt = a->z;
            y = a->y;
        }
    }
    float k;
    //Отрисовка начальной и конечной точки
    k = (a->y - y_min) / (y_max - y_min);
    if (k < 0.1)
        k = 0.1;
    if (k > 1)
        k = 1;
    painter->setPixel(dx + a->x, dy + a->z, qRgba(color.red(), color.green(), color.blue(), 255 * (1-k)));
    k = (b->y - y_min) / (y_max - y_min);
        if (k < 0.1)
            k = 0.1;
        if (k > 1)
            k = 1;
    painter->setPixel(dx + b->x, dy + b->z, qRgba(color.red(), color.green(), color.blue(), 255 * (1-k)));
    if (z != NULL)
    {
        if ((int)(dy + a->z) >= 0 && (int)(dy + a->z) < painter->height() && (int)(dx + a->x) >= 0 && (int)(dx + a->x) < painter->width())
            z[(int)(dy + a->z)][(int)(dx + a->x)]= a->y;
        if ((int)(dy + b->z) >= 0 && (int)(dy + b->z) < painter->height() && (int)(dx + b->x) >= 0 && (int)(dx + b->x) < painter->width())
            z[(int)(dy + b->z)][(int)(dx + b->x)]= b->y;
    }

    for (; c <= ceil(c_max); c++)
    {
        k = (y - y_min) / (y_max - y_min);
        if (k < 0.1)
            k = 0.1;
        if (k > 1)
            k = 1;
        QRgb _color = qRgba(color.red(), color.green(), color.blue(), 255 * (1-k));
        if (z != NULL)
            _color = qRgba(0, 0, 0, 255);
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
    }

}

void Polygon3D::Draw(QPainter* painter, int x, int y, bool edges)
{
    //Нахождение границ
    int min_y = INT_MAX, max_y = INT_MIN, min_x = INT_MAX, max_x = INT_MIN;
    for (int i = 0; i < size; i++)
    {
        if (Point(i).x < min_x)
            min_x = Point(i).x;
        if (Point(i).x > max_x)
            max_x = Point(i).x;
        if (Point(i).z < min_y)
            min_y = Point(i).z;
        if (Point(i).z > max_y)
            max_y = Point(i).z;
    }
    if (max_y > painter->window().height())  //ВРЕМЕННО
        max_y = painter->window().height();
    if (max_x > painter->window().width())
        max_x = painter->window().width();
    if (max_x < 0)
    {
        max_x = 0;
    }
    if (max_y < 0)
    {
        max_y = 0;
    }
    QImage buffer(max_x - min_x + 1, max_y - min_y + 1, QImage::Format_ARGB32);
    buffer.fill(qRgba(0, 0, 0, 0));
    //Отрисовка границ
    for (int i = 0; i < size; i++)
    {
        Point3D a, b;
        a = Point(i);
        if (i < size -1)
            b = Point(i+1);
        else
            b = Point(0);
        drawLine3D(&buffer, &a, &b, -min_x, -min_y, 0, 100);
    }
    QRgb _color = qRgba(180, 255, 180, 150);
    //Отрисовка внутренностей
    for (int i = 0; i < buffer.height(); i++)
    {
        bool inside = false;
        bool stripe = false;
        int sx=0, ex=0; //Начало и конец полосы
        for (int j = 0; j < buffer.width(); j++)    //Двигаемся по строке вправо
        {
            if (qAlpha(buffer.pixel(j, i)) > 0)
            {
                if (!stripe)
                {
                    stripe = true;
                }
                if (!inside)
                    sx = j;
                else
                {
                    ex = j+1;
                    for (int k = sx; k < ex; k++)
                        buffer.setPixel(k, i, _color);
                }
                if (edges)
                    continue;
            }
            if (stripe)
            {
                stripe = false;
                inside = !inside;
            }
        }
    }
    painter->drawImage(x + min_x, y + min_y, buffer);
}

void Polygon3D::DrawZ(QImage* img, double** z, int x, int y, bool shade, bool show_zbuf, QRgb faces_color)
{
    int min_y = INT_MAX, max_y = INT_MIN, min_x = INT_MAX, max_x = INT_MIN;
    for (int i = 0; i < size; i++)
    {
        if (Point(i).x < min_x)
            min_x = Point(i).x;
        if (Point(i).x > max_x)
            max_x = Point(i).x;
        if (Point(i).z < min_y)
            min_y = Point(i).z;
        if (Point(i).z > max_y)
            max_y = Point(i).z;
    }
    int padding = 10;
    QImage buffer(max_x - min_x + 2*padding, max_y - min_y + 2*padding, QImage::Format_ARGB32); //I?E CAIENE N?AA I?EAAAE?OU PADDING
    buffer.fill(qRgba(0, 0, 0, 0));

    double** z_buffer = new double*[max_y - min_y + 2*padding]; // [no?iee][noieaou]
    for (int i = 0; i < max_y - min_y + 2*padding; i++)
    {
        z_buffer[i] = new double[max_x - min_x + 2*padding];
        for (int j = 0; j < max_x - min_x + 2*padding; j++)
            z_buffer[i][j] = INT_MAX;
    }

    //Io?eniaea a?aieo
    for (int i = 0; i < size; i++)
    {
        Point3D a, b;
        a = Point(i);
        if (i == size-1)
            b = Point(0);
        else
            b = Point(i+1);
        drawLine3D(&buffer, &a, &b, -min_x + padding, -min_y + padding, 0, 100, QColor(0,0,0), z_buffer);
    }
    //Io?eniaea aioo?aiiinoae
    for (int i = 0; i < buffer.height(); i++)
    {
        bool inside = false;
        bool stripe = false;
        int sx=0, ex=0; //Ia?aei e eiiao iieinu
        double sz=0, ez=0;  //Ia?aeuiu e eiia?iua cia?aiey z
        for (int j = 0; j < buffer.width(); j++)    //Aaeaaainy ii no?iea ai?aai
        {
            if (qAlpha(buffer.pixel(j, i)) > 0)
            {
                if (!stripe)
                {
                    inside = !inside;
                    stripe = true;
                }
                if (inside)    //Ia?aei "iieinu"
                {
                    sx = j+1;
                    sz = z_buffer[i][j];
                }
                else
                {
                    ex = j;   //Eiiao "iieinu"
                    ez = z_buffer[i][j];
                    for (int k = sx; k < ex; k++)   //Cae?anea iieinu
                    {
                        if (sx != ex)
                            z_buffer[i][k] = (sz + (ez-sz) * (k - sx)/(ex-sx));
                        else
                            z_buffer[i][k] = sz;
                        buffer.setPixel(k, i, faces_color);
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
    //I?eiaiaiea e o?a eia?uaiony ecia?a?aie?
    for (int i = 0; i < buffer.height(); i++)
    {
        for (int j = 0; j < buffer.width(); j++)    //Aaeaaainy ii no?iea ai?aai
        {
            int screen_x = j + min_x + x - padding;
            int screen_y = i + min_y + y - padding;

            if (screen_x >= 0 && screen_x < img->width() && screen_y >= 0 && screen_y < img->height())
            {
                if (z_buffer[i][j] < z[screen_y][screen_x])
                {
                    z[screen_y][screen_x] = z_buffer[i][j];
                    int intensity;
                    if (shade)
                    {
                        intensity = 255 - (((double)z_buffer[i][j] + 250) / 500 * 255);
                        if (intensity < 0)
                            intensity = 0;
                        if (intensity > 255)
                            intensity = 255;
                        img->setPixel(screen_x, screen_y, qRgb((double)intensity/255 * qRed(buffer.pixel(j, i)),
                                                               (double)intensity/255 * qGreen(buffer.pixel(j, i)),
                                                               (double)intensity/255 * qBlue(buffer.pixel(j, i))
                                                               ));
                    }
                    else
                        img->setPixel(screen_x, screen_y, buffer.pixel(j, i));
                    if (show_zbuf)  //Iieacaou z-aooa?
                    {
                        if (!shade)
                        {
                            intensity = 255 - (((double)z_buffer[i][j] + 250) / 500 * 255);
                            if (intensity < 0)
                                intensity = 0;
                            if (intensity > 255)
                                intensity = 255;
                        }
                        img->setPixel(screen_x, screen_y, qRgb(255,intensity,0.7 * (255-intensity)));
                    }
                }
            }
        }
    }

    for (int i = 0; i < max_y - min_y + 2*padding; i++)
        delete z_buffer[i];
    delete[] z_buffer;
}




