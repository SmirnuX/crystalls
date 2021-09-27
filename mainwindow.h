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

class Point3D;

class Crystall  //Базовый класс кристаллов
{
public:
    QString name;   //Название кристалла
    int vertex_count;   //Количество вершин
    int edges_count;    //Количество ребер
    Point3D* vertexes;   //Массив вершин
    Point3D* turned_vertexes;   //Массив вершин после изменений
    double a, b, g;    //Углы поворота
    double scale;  //Масштаб
    double dx, dy, dz;  //Перенос
    bool reflectXOY, reflectXOZ, reflectYOZ;    //Отражение
    //Массивы граней - edges_from - содержит номера начальных точек из vertexes, edged_to - номера конечных точек.
    //То есть, первая грань идет из vertexes[edges_from[0]] в vertexes[edges_to[0]]
    int* edges_from;
    int* edges_to;
    Crystall(int vertex, int edges, QString _name);
    void Draw(QPainter* painter, bool gradient, bool numbers);
    virtual void CalculatePoints() = 0;
    void AddEdge(int ind, int from, int to);
    void Change();
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
    bool show_numbers;
    bool is_gradient;
    //Методы класса
    void paintEvent(QPaintEvent *); //Отрисовка буфера в окне
signals:
    void updated();
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
};

class Point3D   //Точка в 3D пространстве
{
public:
    Point3D();
    double x;
    double y;
    double z;
    QString Show();
    void Set(double _x, double _y, double _z);
};

template <typename T>
class Matrix    //Матрицы
{
public:
    Matrix(int row, int col, T** array = NULL);
    ~Matrix();
    Matrix operator * (Matrix<T> b);
    T* operator [](size_t row); //Получить строку матрицы
    void print();

    size_t rows;
    size_t cols;
    T** data;
};

void drawLine3D(QPainter* painter, Point3D* a, Point3D* b, int dx = 0, int dy = 0, int y_min = -100, int y_max = 100, QColor color = QColor(0, 0, 0));

#endif // MAINWINDOW_H
