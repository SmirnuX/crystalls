//РђР»РіРѕСЂРёС‚РјС‹ Р’РµР№Р»РµСЂР°-РђР·РµСЂС‚РѕРЅР°
#include "mainwindow.h"

void Crystall::DrawVeyler(QPainter* painter)
{
    std::vector<Polygon3D*> vec;    //Все полигоны фигуры
    std::vector<std::vector<Point2D> > inner;
    std::vector<std::vector<Point2D> > outer;
    //1.Предварительная сортировка -
    for (int i = 0; i < faces_count; i++)
    {
        vec.push_back(faces+i);
        for(int j = vec.size() - 1; j > 0 ; j--)
            if (vec[j]->minY() < vec[j-1]->minY())
            {
                Polygon3D* temp = vec[j];
                vec[j] = vec [j-1];
                vec[j-1] = temp;
            }
    }
    //2.XY-сортировка
    for (int i = 1; i < faces_count; i++)
    {
        weiler_cut(*(vec[0]), *(vec[i]), inner, outer);
    }
    //3. Работа с внутренним списком - пока не делается, предположим что её нет
    /*for (int i = inner.size(); i >= 0; i--)
    {
        if (cutter.maxY() < inner[i].minY())
            DrawVeyler(inner);  //Р РµРєСѓСЂСЃРёРІРЅРѕ РїРѕРІС‚РѕСЂСЏРµРј РґР»СЏ РІРЅСѓС‚СЂРµРЅРЅРёС… РїРµСЂРµРґ РѕС‚СЃРµРєР°С‚РµР»РµРј
    }*/
    inner.clear();
    //4. Работа с внешним списком - пока не делается, пусть вырезается только самый ближний

    //5. Финальная отрисовка
    double dx = painter->window().width()/2;
    double dy = painter->window().height()/2;
    //Отрисовка задников
    for (int i = 0; i < outer.size(); i++)
    {
        for (int j = 0; j < outer[i].size(); j++)
        {
            if (j == outer[i].size() - 1)
                painter->drawLine(outer[i][j].x + dx, outer[i][j].y + dy, outer[i][0].x + dx, outer[i][0].y + dy);
            else
                painter->drawLine(outer[i][j].x + dx, outer[i][j].y + dy, outer[i][j+1].x + dx, outer[i][j+1].y + dy);
        }
    }
    //Отрисовка передника
    for (int j = 0; j < vec[0]->size; j++)
    {
        if (j == vec[0]->size - 1)
            drawLine3D(painter, &(vec[0]->Point(j)), &(vec[0]->Point(0)), dx, dy, -100, 100, QColor(255,0,0));
        else
            drawLine3D(painter, &(vec[0]->Point(j)), &(vec[0]->Point(j+1)), dx, dy, -100, 100, QColor(255,0,0));
    }


}




bool intersect(Point2D A1, Point2D B1, Point2D A2, Point2D B2, Point2D& result)
{
    //Первичная проверка
    if (!proj_intersec(A1.x, B1.x, A2.x, B2.x) || !proj_intersec(A1.y, B1.y, A2.y, B2.y))
        return false;
    line l1(A1, B1);
    line l2(A2, B2);
    double zn = det (l1.a, l1.b, l2.a, l2.b);
    if (fabs (zn) < 0.00001) {
        //if (fabs (l1.dist (A2)) > 0.00001 || fabs (l2.dist (A1)) > 0.00001)
                return false;
        result.x = A1.x;
        result.y = A1.y;
        return true;
    }
    else {
    result.x = - det (l1.c, l1.b, l2.c, l2.b) / zn;
    result.y = - det (l1.a, l1.c, l2.a, l2.c) / zn;
    return betw (A1.x, B1.x, result.x)
            && betw (A1.y, B1.y, result.y)
            && betw (A2.x, B2.x, result.x)
            && betw (A2.y, B2.y, result.y);
    }
}



void weiler_cut(Polygon3D _cutter,
                Polygon3D _cutting,
                std::vector<std::vector<Point2D> >& inner,
                std::vector<std::vector<Point2D> >& outer)
{
    std::vector<Point2D> cutter = Polygon3Dto2D(_cutter);   //РћР±СЂРµР·Р°СЋС‰РёР№ РјРЅРѕРіРѕСѓРіРѕР»СЊРЅРёРє
    std::vector<Point2D> cutting = Polygon3Dto2D(_cutting); //РћР±СЂРµР·Р°РµРјС‹Р№ РјРЅРѕРіРѕСѓРіРѕР»СЊРЅРёРє
    weiler_cut(cutter, cutting, inner, outer);
}

void weiler_cut(std::vector<Point2D>& cutter, std::vector<Point2D>& cutting,
                std::vector<std::vector<Point2D> >& inner,
                std::vector<std::vector<Point2D> >& outer)
{
    //0. РџСЂРµРѕР±СЂР°Р·РѕРІР°РЅРёРµ РёР· Polygon3D РІ РІРµРєС‚РѕСЂР° С‚РѕС‡РµРє, РѕСЂРіР°РЅРёР·РѕРІР°РЅРЅС‹Рµ РїРѕ С‡Р°СЃРѕРІРѕР№ СЃС‚СЂРµР»РєРµ
    //std::vector<Point2D> cutter = Polygon3Dto2D(_cutter);   //РћР±СЂРµР·Р°СЋС‰РёР№ РјРЅРѕРіРѕСѓРіРѕР»СЊРЅРёРє
    //std::vector<Point2D> cutting = Polygon3Dto2D(_cutting); //РћР±СЂРµР·Р°РµРјС‹Р№ РјРЅРѕРіРѕСѓРіРѕР»СЊРЅРёРє
    std::vector<int> entering;
    std::vector<int> exiting;
    //1. Точки пересечения и их классификация
    for (int i = 0; i < cutting.size(); i++) //Обрезаемый
    {
        //Начальная и конечная вершины
        Point2D A = cutting[i];
        Point2D B = (i < cutting.size()-1)?cutting[i+1]:cutting[0];
        qDebug()<<"Cutting " << A.x << "," << A.y << " <>"<<B.x << "," << B.y;
        for (int j = 0; j < cutter.size(); j++)    //Обрезатель
        {
            //Начальная и конечная вершины
            Point2D Start = cutter[j];
            Point2D Finish = (j < cutter.size()-1)?cutter[j+1]:cutter[0];
            qDebug()<<"Cutter " << Start.x << "," << Start.y << " <>"<<Finish.x << "," << Finish.y;
            Point2D intersection;
            if (intersect(A, B, Start, Finish, intersection))
            {
                Point2D AB(B.x - A.x, B.y - A.y, 0);
                Point2D CD(Finish.x - Start.x, Finish.y - Start.y, 0);
                Point2D Normal(-AB.y, AB.x);
                if (scalar(CD, Normal) < 0) //Входящая вершина
                {
                    cutting.insert(cutting.begin()+i+1, Point2D(intersection.x, intersection.y, -1));
                    entering.push_back(i+1);
                    cutter.insert(cutter.begin()+j+1, Point2D(intersection.x, intersection.y, -1));
                    i++;
                    j++;
                }
                else    //Выходящая вершина
                {
                    cutting.insert(cutting.begin()+i+1, Point2D(intersection.x, intersection.y, 1));
                    exiting.push_back(i+1);
                    cutter.insert(cutter.begin()+j+1, Point2D(intersection.x, intersection.y, 1));
                    i++;
                    j++;
                }
            }
        }
    }
    qDebug() << "<end>";
    //2. Получение внутреннего список
    for (int i = 0; i < entering.size(); i++)
    {
        if (entering[i]>= 0)    //Если вершина еще не посещена
        {
            std::vector<Point2D> inside;
            int start = entering[i];
            entering[i] = -entering[i]; //ПОМЕЧАЕМ КАК ПРОЙДЕННУЮ
            inside.push_back(cutting[start]);

            bool finished = false;
            for (int enter = start; !finished ;)
            {
                int exit;
                std::vector<Point2D> outside;   //ВНЕШНИЙ ПОЛИГОН
                outside.push_back(cutting[enter]);

                int enter_cutter = 0;   //Вершина enter в обрезателе
                for (int j = 0; j < cutter.size(); j++) //Поиск аналога вершины ENTER в обрезателе
                {
                    if (cutter[j].x == cutting[enter].x && cutter[j].y == cutting[enter].y)
                    {
                        enter_cutter = j;
                        break;
                    }
                }
                for (int j = enter_cutter; ; (j==cutter.size()-1)?j=0:j++)  //Добавление внутренних вершин
                {
                    if (j == enter_cutter)
                        continue;
                    inside.push_back(cutter[j]);
                    outside.push_back(cutter[j]);
                    if (cutter[j].intersec == 1)
                    {
                        exit = j;
                        break;
                    }
                }
                int exit_cutting = 0;   //Выходная вершина в обрезаемом
                for (int j = 0; j < exiting.size(); j++)    //Поиск точки выхода в обрезаемом
                {
                    if (cutting[exiting[j]].x == cutter[exit].x && cutting[exiting[j]].y == cutter[exit].y)
                        exit_cutting = exiting[j];
                }
                for (int j = exit_cutting; ;(j==0)?j=cutting.size()-1:j--)  //ЗАКРЫТИЕ ВНЕШНЕГО ПОЛИГОНА
                {
                    if (j != enter)
                        outside.push_back(cutting[j]);
                    else
                        break;
                }
                outer.push_back(outside);
                for(int j = exit_cutting; ; (j==cutting.size()-1)?j=0:j++)  //Добавление внешних вершин
                {
                    if (j == exit_cutting)
                        continue;
                    if (cutting[j].intersec == -1)  //Если найдена входная вершина
                    {
                        if (j == enter)
                        {
                            finished = true;    //"ЗАКРЫТИЕ" полигона
                            break;
                        }
                        else
                        {
                            enter = j;  //ПЕРЕХОД К СЛЕДУЮЩЕЙ ВХОДНОЙ ВЕРШИНЕ
                            inside.push_back(cutting[j]);
                            for (int i = 0; i < entering.size(); i++)
                            {
                                if (entering[i] == enter)
                                    entering[i] = -enter;   //ПОМЕТКА ВЕРШИНЫ КАК ПРОЙДЕННОЙ
                            }
                            break;
                        }
                    }
                    else
                        inside.push_back(cutting[j]);   //ДОБАВЛЕНИЕ ВНЕШНЕЙ ВЕРШИНЫ
                }
            }
            inner.push_back(inside);
        }
    }
}

double OrientedArea(Polygon3D in)   //Р’С‹С‡РёСЃР»СЏРµС‚ РѕСЂРёРµРЅС‚РёСЂРѕРІР°РЅРЅСѓСЋ РїР»РѕС‰Р°РґСЊ РїСЂРѕРµРєС†РёРё РјРЅРѕРіРѕСѓРіРѕР»СЊРЅРёРєР° РЅР° XOZ
{
    double sum = 0;
    int last = in.size - 1;
    for (int i = 0; i < in.size; i++)
    {
        sum += in.Point(last).x * in.Point(i).z - in.Point(last).z * in.Point(i).x;
        last = i;
    }
    return sum;
}

std::vector<Point2D> Polygon3Dto2D(Polygon3D in)    //Проекция 3Д полигона в 2Д с порядком вершин по часовой стрелке
{
    std::vector<Point2D> result;
    if (OrientedArea(in) > 0)   //Обход против часовой стрелки - инвертируется
    {
        for (int i = in.size-1; i >= 0; i--)
            result.push_back(Point2D(in.Point(i).x, in.Point(i).z, 0));
    }
    else
    {
        for (int i = 0; i < in.size; i++)
            result.push_back(Point2D(in.Point(i).x, in.Point(i).z, 0));
    }

    return result;
}

double scalar(Point2D a, Point2D b)
{
    return a.x * b.x + a.y + b.y;
}

bool proj_intersec(double a, double b, double c, double d)  //Пересекаются ли отрезки ab и cd на одной прямой
{
    if (a>b)
    {
        double t = a;
        a = b;
        b = t;
    }
    if (c>d)
    {
        double t = c;
        c = d;
        d = t;
    }
    double _min = (a > c)?a:c;
    double _max = (b < d)?b:d;
    return _min <= _max + 0.00001;
}

double min(double a, double b)
{
    return (a<b)?a:b;
}

double max(double a, double b)
{
    return (a>b)?a:b;
}

bool betw (double l, double r, double x) {
    return min(l,r) <= x + 0.00001 && x <= max(l,r) + 0.00001;
}



