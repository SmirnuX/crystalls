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

struct param    //��������� ���������
{
    bool gradient_lines;    //����������� �����
    bool show_vertices;     //���������� ������ ������
    bool intersec_zbuf;     //��������� ��������� ����� ������� Z-������
    bool show_zbuf;         //���������� Z-�����
    bool show_faces;    //������������ �����
    QRgb faces_color;   //���� ������
    bool faces_shade;   //"����" �� ������
    bool show_zbuf_numbers;  //�������� ��������� �������
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
    Polygon3D* faces;   //������ ������
    double a, b, g;    //���� ��������
    double scale;  //�������
    double skewX, skewY, skewZ; //����������
    double dx, dy, dz;  //�������
    bool reflectXOY, reflectXOZ, reflectYOZ;    //���������
    Matrix<double>* Quaternion;
    Matrix<double>** Points4D;
    double** z_buffer;
    //������� ������ - edges_from - �������� ������ ��������� ����� �� vertexes, edged_to - ������ �������� �����.
    //�� ����, ������ ����� ���� �� vertexes[edges_from[0]] � vertexes[edges_to[0]]
    int* edges_from;
    int* edges_to;

    Crystall(int vertex, int edges, int n_faces, QString _name);
    void Draw(QPainter* painter, struct param* settings);
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

class Romb : public Crystall
{
public:
    Romb();
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

class Point3D   //����� � 3D ������������
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

    void Draw(QPainter* painter, int x, int y, bool edges = true);
    void DrawZ(QImage* img, double** z, int x, int y, bool shade = true, bool show_zbuf = true, QRgb faces_color = qRgb(255,255,255));
};



void drawLine3D(QPainter* painter, Point3D* a, Point3D* b, int dx = 0, int dy = 0, int y_min = -100, int y_max = 100, QColor color = QColor(0, 0, 0));
void drawLine3D(QImage* painter, Point3D* a, Point3D* b, int dx = 0, int dy = 0, int y_min = -100, int y_max = 100, QColor color = QColor(0, 0, 0), double** z = NULL);
void transformToMatrix(double a[4][4]);


#endif // MAINWINDOW_H
