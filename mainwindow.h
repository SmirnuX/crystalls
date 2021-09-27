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


class Point3D;

class Crystall  //������� ����� ����������
{
public:
    QString name;   //�������� ���������
    int vertex_count;   //���������� ������
    int edges_count;    //���������� �����
    Point3D* vertexes;   //������ ������
    Point3D* turned_vertexes;   //������ ������ ����� ���������
    double a, b, g;    //���� ��������
    double scale;  //�������
    double skewX, skewY, skewZ; //����������
    double dx, dy, dz;  //�������
    bool reflectXOY, reflectXOZ, reflectYOZ;    //���������
    Matrix<double>* Quaternion;
    Matrix<double>** Points4D;
    //������� ������ - edges_from - �������� ������ ��������� ����� �� vertexes, edged_to - ������ �������� �����.
    //�� ����, ������ ����� ���� �� vertexes[edges_from[0]] � vertexes[edges_to[0]]
    int* edges_from;
    int* edges_to;
    Crystall(int vertex, int edges, QString _name);
    void Draw(QPainter* painter, bool gradient, bool numbers);
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
    bool show_numbers;
    bool is_gradient;
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
};



void drawLine3D(QPainter* painter, Point3D* a, Point3D* b, int dx = 0, int dy = 0, int y_min = -100, int y_max = 100, QColor color = QColor(0, 0, 0));
void transformToMatrix(double a[4][4]);


#endif // MAINWINDOW_H
