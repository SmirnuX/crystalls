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

#define SCALE 0.5   //���������� ����������

class Point3D; class Polygon3D;

extern bool lowres;

struct param    //��������� ���������
{
    bool gradient_lines;    //����������� �����
    bool show_vertices;     //���������� ������ ������
    bool intersec_zbuf;     //��������� ��������� ����� ������� Z-������
    bool intersec_veyler;   //��������� ��������� ����� ������� �������-��������
    bool guro;              //�������� ���� ������� ����
    double k_diff;          //����������� ������� ��������� ���������
    double k_amb;           //����������� �������� ���������
    bool show_normals;      //���������� ������� (����� ����)
    bool show_zbuf;         //���������� Z-�����
    bool show_faces;    //������������ �����
    QRgb faces_color;   //���� ������
    bool faces_shade;   //"����" �� ������
    bool show_zbuf_numbers;  //�������� ��������� �������
    double light_x; //������� ��������� ����� ������������ OX
    double light_y;
    double light_z;
};

class Crystall  //������� ����� ����������
{
public:
    QString name;   //�������� ���������
    int vertex_count;   //���������� ������
    int edges_count;    //���������� �����
    int faces_count;    //���������� ������
    Point3D* vertexes;   //������ ������
    Point3D* turned_vertexes;   //������ ������ ����� ���������
    Point3D* normals;   //������� � ������
    Point3D* vert_normals;  //������� � ��������
    double* vert_intens;    //������������� ��������� ������
    Polygon3D* faces;   //������ ������
    double a, b, g;    //���� ��������
    double scale;  //�������
    double skewX, skewY, skewZ; //����������
    double dx, dy, dz;  //�������
    bool reflectXOY, reflectXOZ, reflectYOZ;    //���������
    Matrix<double>* Quaternion;
    Matrix<double>** Points4D;
    std::vector<Polygon3D*> vec;    //��� �������� ������
    //������� ������ - edges_from - �������� ������ ��������� ����� �� vertexes, edged_to - ������ �������� �����.
    //�� ����, ������ ����� ���� �� vertexes[edges_from[0]] � vertexes[edges_to[0]]
    int* edges_from;
    int* edges_to;

    double** temp_z_buffer;
    QImage* temp_image;

    Crystall(int vertex, int edges, int n_faces, QString _name);
    void Draw(QPainter* painter, struct param* settings, QImage** img, double*** z_buffer);
    void DrawWeyler(QPainter* painter);
    void DrawPseudoWeyler(QPainter* painter);
    void DrawInterpolated(QImage* img, int num, double** z, QRgb color);   //��������� ������� ��� ������ ����
    //void DrawZ(QImage* img, double** z, int x, int y, bool shade, bool show_zbuf, QRgb faces_color);
    virtual void CalculatePoints() = 0;
    void AddEdge(int ind, int from, int to);
    void Change();
    void Reset();
    void Turn(double alpha, double beta, double gamma);
    void Scale(double _scale);
    ~Crystall();
};

//  ===���������������� ���������===
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

class MainWindow : public QMainWindow   //����� �������� ����
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

class canvas : public QWidget   //����� "������", �� ������� ����� �������������� �����������
{
    Q_OBJECT
public:
    double a, b, g;
    Crystall* crystall;
    QPointF prev_pos;
    //����������� � ����������
    canvas(QWidget* parent);
    struct param parameters;
    QImage* img;
    double** z_buffer;
    QTime timer;
    int frame_count;
    //������ ������
    void paintEvent(QPaintEvent *); //��������� ������ � ����
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
    int intersec;   //-1 - ���� � �������������, 1 - �����, 0 - �� ��������� � ������������
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

#define det(a,b,c,d)  (a*d-b*c) //������������ ��������� �������

class Point3D   //����� � 3D ������������
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

class Polygon3D //����������� � 3D-������������
{
public:
    int size;
    int* points;
    Point3D* vertices;

    Polygon3D()
    {
        size = 0;
    }

    void Set(int a, int b, int c)                           //������ �����������
    {
        size = 3;
        points = new int[3];
        points[0] = a;
        points[1] = b;
        points[2] = c;
    }

    void Set(int a, int b, int c, int d)                //������ ���������������
    {
        size = 4;
        points = new int[4];
        points[0] = a;
        points[1] = b;
        points[2] = c;
        points[3] = d;
    }
    void Set(int a, int b, int c, int d, int e)     //������ ������������
    {
        size = 5;
        points = new int[5];
        points[0] = a;
        points[1] = b;
        points[2] = c;
        points[3] = d;
        points[4] = e;
    }
    void Set(int* arr, int count)   //������ n-��������
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

double OrientedArea(Polygon3D in);   //Вычисляет ориентированную площадь проекции многоугольника на XOZ
double OrientedArea(std::vector<Point2D>& in);
std::vector<Point2D> Polygon3Dto2D(Polygon3D in);



void drawLine3D(QPainter* painter, Point3D* a, Point3D* b, int dx = 0, int dy = 0, int y_min = -100, int y_max = 100, QColor color = QColor(0, 0, 0));
void drawLine3D(QImage* painter, Point3D* a, Point3D* b, int dx = 0, int dy = 0, int y_min = -100, int y_max = 100, QColor color = QColor(0, 0, 0), double** z = NULL);
void drawLine3D(QImage* painter, Point3D* a, Point3D* b, int dx, int dy, QRgb color, double** z, double a_int, double b_int);   //��������� ����� � �������������
void transformToMatrix(double a[4][4]);


#endif // MAINWINDOW_H
