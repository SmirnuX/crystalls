#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFrame>
#include <QDebug>
#include <QString>
#include <QPointF>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <qmath.h>
#include <math.h>
#include "matrix.h"

class Point3D; class Polygon3D;

struct param    //Настройки отрисовки
{
    bool gradient_lines;    //Градиентные линии
    bool show_vertices;     //Показывать номера вершин
    bool intersec_zbuf;     //Отсечение невидимых линий методом Z-буфера
    bool intersec_veyler;   //Отсечение невидимых линий методом Вейлера-Азертона
    bool show_zbuf;         //Показывать Z-буфер
    bool show_faces;    //Отрисовывать грани
    QRgb faces_color;   //Цвет граней
    bool faces_shade;   //"Тени" на гранях
    bool show_zbuf_numbers;  //Скрывать невидимые вершины
};

class Crystall  //Базовый класс кристаллов
{
public:
    QString name;   //Название кристалла
    int vertex_count;   //Количество вершин
    int edges_count;    //Количество ребер
    int faces_count;    //Количество граней
    Point3D* vertexes;   //Массив вершин
    Point3D* turned_vertexes;   //Массив вершин после изменений
    Polygon3D* faces;   //Массив граней
    double a, b, g;    //Углы поворота
    double scale;  //Масштаб
    double skewX, skewY, skewZ; //Растяжение
    double dx, dy, dz;  //Перенос
    bool reflectXOY, reflectXOZ, reflectYOZ;    //Отражение
    Matrix<double>* Quaternion;
    Matrix<double>** Points4D;
    double** z_buffer;
    //Массивы граней - edges_from - содержит номера начальных точек из vertexes, edged_to - номера конечных точек.
    //То есть, первая грань идет из vertexes[edges_from[0]] в vertexes[edges_to[0]]
    int* edges_from;
    int* edges_to;

    Crystall(int vertex, int edges, int n_faces, QString _name);
    void Draw(QPainter* painter, struct param* settings);
    void DrawVeyler(QPainter* painter);
    virtual void CalculatePoints() = 0;
    void AddEdge(int ind, int from, int to);
    void Change();
    void Reset();
    void Turn(double alpha, double beta, double gamma);
    void Scale(double _scale);
    ~Crystall();
};

//  ===Пользовательские кристаллы===
class Smirnov1 : public Crystall
{
public:
    Smirnov1();
    void CalculatePoints();
};

class Smirnov2 : public Crystall
{
public:
    Smirnov2();
    void CalculatePoints();
};

class Volodina1: public Crystall
{
public:
    Volodina1();
    void CalculatePoints();
};

class Volodina2: public Crystall
{
public:
    Volodina2();
    void CalculatePoints();
};

class Romb : public Crystall
{
public:
    Romb();
    void CalculatePoints();
};


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow   //Класс главного окна
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Crystall** crystalls;


private:
    Ui::MainWindow *ui;

public slots:
    void changeCrystall(int index);
    void updateCrystall();
    void changeLab(int index);
    void Reset();
};

class canvas : public QWidget   //Класс "Холста", на котором будет отрисовываться изображение
{
    Q_OBJECT
public:
    double a, b, g;
    Crystall* crystall;
    QPointF prev_pos;
    //Конструктор и деструктор
    canvas(QWidget* parent);
    struct param parameters;
    //Методы класса
    void paintEvent(QPaintEvent *); //Отрисовка буфера в окне
signals:
    void updated();
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
};

template <class T>
class Matrix;

struct Point2D
{
    Point2D(double a, double b)
    {
        x = a;
        y = b;
        intersec = 0;
    }
    Point2D(double a, double b, int i)
    {
        x = a;
        y = b;
        intersec = i;
    }

    Point2D()
    {

    }

    double x, y;
    int intersec;   //-1 - вход в многоугольник, 1 - выход, 0 - не относится к пересечениям
};

struct line {
        double a, b, c;

        line() {}
        line (Point2D p, Point2D q) {
                a = p.y - q.y;
                b = q.x - p.x;
                c = - a * p.x - b * p.y;
                norm();
        }

        void norm() {
                double z = sqrt (a*a + b*b);
                if (fabs(z) > 0.000001)
                        a /= z,  b /= z,  c /= z;
        }

        double dist (Point2D p) const {
                return a * p.x + b * p.y + c;
        }
};

#define det(a,b,c,d)  (a*d-b*c) //Определитель двумерной матрицы

class Point3D   //Точка в 3D пространстве
{
public:
    Point3D();
    double x;
    double y;
    double z;
    Matrix<double>* to4D();
    QString Show();
    void Set(double _x, double _y, double _z);
    void Set(Matrix<double>* vec);
    void Set(Point3D& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
    }
};

class Polygon3D //поверхность в 3D-пространстве
{
public:
    int size;
    int* points;
    Point3D* vertices;

    Polygon3D()
    {
        size = 0;
    }

    void Set(int a, int b, int c)                           //Задать треугольник
    {
        size = 3;
        points = new int[3];
        points[0] = a;
        points[1] = b;
        points[2] = c;
    }

    void Set(int a, int b, int c, int d)                //Задать четырехугольник
    {
        size = 4;
        points = new int[4];
        points[0] = a;
        points[1] = b;
        points[2] = c;
        points[3] = d;
    }
    void Set(int a, int b, int c, int d, int e)     //Задать пятиугольник
    {
        size = 5;
        points = new int[5];
        points[0] = a;
        points[1] = b;
        points[2] = c;
        points[3] = d;
        points[4] = e;
    }
    void Set(int* arr, int count)   //Задать n-угольник
    {
        points = arr;
        size = count;
    }

    void SetSource(Point3D* vertix)
    {
        vertices = vertix;
    }

    Point3D Point(int i)
    {
        return vertices[points[i]];
    }

    double minY()
    {
        double min = 10000;
        for (int i = 0; i < size; i++)
        {
            if (Point(i).y < min)
                min = Point(i).y;
        }
        return min;
    }

    double maxY()
    {
        double max = -10000;
        for (int i = 0; i < size; i++)
        {
            if (Point(i).y > max)
                max = Point(i).y;
        }
        return max;
    }

    void Draw(QPainter* painter, int x, int y, bool edges = true);
    void DrawZ(QImage* img, double** z, int x, int y, bool shade = true, bool show_zbuf = true, QRgb faces_color = qRgb(255,255,255));
};

double scalar(Point2D a, Point2D b);
bool proj_intersec(double a, double b, double c, double d);
double min(double a, double b);
double max(double a, double b);
bool betw (double l, double r, double x);
void weiler_cut(std::vector<Point2D>& cutter, std::vector<Point2D>& cutting,
                std::vector<std::vector<Point2D> >& inner,
                std::vector<std::vector<Point2D> >& outer);

void weiler_cut(Polygon3D _cutter,
                Polygon3D _cutting,
                std::vector<std::vector<Point2D> >& inner,
                std::vector<std::vector<Point2D> >& outer);

double OrientedArea(Polygon3D in);   //Р’С‹С‡РёСЃР»СЏРµС‚ РѕСЂРёРµРЅС‚РёСЂРѕРІР°РЅРЅСѓСЋ РїР»РѕС‰Р°РґСЊ РїСЂРѕРµРєС†РёРё РјРЅРѕРіРѕСѓРіРѕР»СЊРЅРёРєР° РЅР° XOZ
std::vector<Point2D> Polygon3Dto2D(Polygon3D in);



void drawLine3D(QPainter* painter, Point3D* a, Point3D* b, int dx = 0, int dy = 0, int y_min = -100, int y_max = 100, QColor color = QColor(0, 0, 0));
void drawLine3D(QImage* painter, Point3D* a, Point3D* b, int dx = 0, int dy = 0, int y_min = -100, int y_max = 100, QColor color = QColor(0, 0, 0), double** z = NULL);
void transformToMatrix(double a[4][4]);


#endif // MAINWINDOW_H
