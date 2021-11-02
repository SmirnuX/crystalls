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
#include <QTime>
#include <qmath.h>
#include <math.h>
#include "matrix.h"

#define SCALE 0.5   //Пониженное разрешение

class Point3D; class Polygon3D;

extern bool lowres;

struct param    //Настройки отрисовки
{
    bool gradient_lines;    //Градиентные линии
    bool show_vertices;     //Показывать номера вершин
    bool intersec_zbuf;     //Отсечение невидимых линий методом Z-буфера
    bool intersec_veyler;   //Отсечение невидимых линий методом Вейлера-Азертона
    bool guro;              //Закраска тела методом Гуро
    double k_diff;          //Коэффициент яркости источника освещения
    double k_amb;           //Коэффициент фонового освещения
    bool show_normals;      //Показывать нормали (метод Гуро)
    bool show_zbuf;         //Показывать Z-буфер
    bool show_faces;    //Отрисовывать грани
    QRgb faces_color;   //Цвет граней
    bool faces_shade;   //"Тени" на гранях
    bool show_zbuf_numbers;  //Скрывать невидимые вершины
    double light_x; //Поворот источника света относительно OX
    double light_y;
    double light_z;
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
    Point3D* normals;   //Нормали к граням
    Point3D* vert_normals;  //Нормали к вершинам
    double* vert_intens;    //Интенсивность освещения вершин
    Polygon3D* faces;   //Массив граней
    double a, b, g;    //Углы поворота
    double scale;  //Масштаб
    double skewX, skewY, skewZ; //Растяжение
    double dx, dy, dz;  //Перенос
    bool reflectXOY, reflectXOZ, reflectYOZ;    //Отражение
    Matrix<double>* Quaternion;
    Matrix<double>** Points4D;
    std::vector<Polygon3D*> vec;    //Все полигоны фигуры
    //Массивы граней - edges_from - содержит номера начальных точек из vertexes, edged_to - номера конечных точек.
    //То есть, первая грань идет из vertexes[edges_from[0]] в vertexes[edges_to[0]]
    int* edges_from;
    int* edges_to;

    double** temp_z_buffer;
    QImage* temp_image;

    Crystall(int vertex, int edges, int n_faces, QString _name);
    void Draw(QPainter* painter, struct param* settings, QImage** img, double*** z_buffer);
    void DrawWeyler(QPainter* painter);
    void DrawPseudoWeyler(QPainter* painter);
    void DrawInterpolated(QImage* img, int num, double** z, QRgb color);   //Отрисовка вершины для метода Гуро
    //void DrawZ(QImage* img, double** z, int x, int y, bool shade, bool show_zbuf, QRgb faces_color);
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

class Borodina1: public Crystall
{
public:
    Borodina1();
    void CalculatePoints();
};

class Borodina2: public Crystall
{
public:
    Borodina2();
    void CalculatePoints();
};

class Romb : public Crystall
{
public:
    Romb();
    void CalculatePoints();
};

class Cube : public Crystall
{
public:
    Cube();
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
    void mouseUpdateCrystall();
    void angleAUpdateCrystall();
    void angleGUpdateCrystall();
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
    QImage* img;
    double** z_buffer;
    QTime timer;
    int frame_count;
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
        entered = false;
    }
    Point2D(double a, double b, int i)
    {
        x = a;
        y = b;
        intersec = i;
        entered = false;
    }

    Point2D()
    {
        entered = false;
    }

    double x, y;
    int intersec;   //-1 - вход в многоугольник, 1 - выход, 0 - не относится к пересечениям
    bool entered;
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
    Point3D(double _x, double _y, double _z)
    {
        Set(_x, _y, _z);
    }

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
    void DrawZ(QImage* img, double** z, int x, int y, bool shade, bool show_zbuf, QRgb faces_color);
};

double scalar(Point2D a, Point2D b);
bool proj_intersec(double a, double b, double c, double d);
double min(double a, double b);
double max(double a, double b);
bool betw (double l, double r, double x);
bool aq(double a, double b);
void weiler_cut(std::vector<Point2D>& cutter, std::vector<Point2D>& cutting,
                std::vector<std::vector<Point2D> >& inner,
                std::vector<std::vector<Point2D> >& outer);

void weiler_cut(Polygon3D _cutter,
                Polygon3D _cutting,
                std::vector<std::vector<Point2D> >& inner,
                std::vector<std::vector<Point2D> >& outer);

double OrientedArea(Polygon3D in);   //Р’С‹С‡РёСЃР»СЏРµС‚ РѕСЂРёРµРЅС‚РёСЂРѕРІР°РЅРЅСѓСЋ РїР»РѕС‰Р°РґСЊ РїСЂРѕРµРєС†РёРё РјРЅРѕРіРѕСѓРіРѕР»СЊРЅРёРєР° РЅР° XOZ
double OrientedArea(std::vector<Point2D>& in);
std::vector<Point2D> Polygon3Dto2D(Polygon3D in);



void drawLine3D(QPainter* painter, Point3D* a, Point3D* b, int dx = 0, int dy = 0, int y_min = -100, int y_max = 100, QColor color = QColor(0, 0, 0));
void drawLine3D(QImage* painter, Point3D* a, Point3D* b, int dx = 0, int dy = 0, int y_min = -100, int y_max = 100, QColor color = QColor(0, 0, 0), double** z = NULL);
void drawLine3D(QImage* painter, Point3D* a, Point3D* b, int dx, int dy, QRgb color, double** z, double a_int, double b_int);   //Отрисовка линии с интерполяцией
void transformToMatrix(double a[4][4]);


#endif // MAINWINDOW_H
