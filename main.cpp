#include <QtGui/QApplication>
#include "mainwindow.h"
#include "matrix.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

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
    QString a = "{" + QString::number(x) + "; " + QString::number(y) + "; " + QString::number(z) + "}";
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
        painter->setPen(QColor(color.red(), color.green(), color.blue(), 255*k));
        if (x_tick)
            painter->drawPoint(c + dx, qt + dy);
        else
            painter->drawPoint(qt + dx, c + dy);
        qt += d;
        y += d_y;
    }
}

