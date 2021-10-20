//РђР»РіРѕСЂРёС‚РјС‹ Р’РµР№Р»РµСЂР°-РђР·РµСЂС‚РѕРЅР°
#include "mainwindow.h"

void Crystall::DrawWeyler(QPainter* painter)
{
    double dx = painter->window().width()/2;
    double dy = painter->window().height()/2;
    vec.clear();
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

    /*
    for (int i = 0; i < faces_count; i++)   //ТЕСТОВОЕ - закинуть все во внешний список
    {
        outer.push_back(Polygon3Dto2D(faces[i]));
    }*/
    /*
    for (int j = outer.size() - 1; j >= 0; j--)  //Очистка списка
    {
        if (outer[j].size() == 0)
        {
            outer.erase(outer.begin() + j);
            continue;
        }
        if (fabs(OrientedArea(outer[j])) < 10)
        {
            outer.erase(outer.begin() + j);
            continue;
        }
    }

    painter->setPen(QColor(220, 220, 220));
    /*
    for (int k = 0; k < outer.size(); k++)
    {
        for (int j = 0; j < outer[k].size(); j++)
        {
            if (j == outer[k].size() - 1)
                painter->drawLine(outer[k][j].x + dx, outer[k][j].y + dy, outer[k][0].x + dx, outer[k][0].y + dy);
            else
                painter->drawLine(outer[k][j].x + dx, outer[k][j].y + dy, outer[k][j+1].x + dx, outer[k][j+1].y + dy);
        }
    }*/

    //2.XY-сортировка
    for (int i = 0; i < faces_count; i++)   //Первичная сортировка
    {
        weiler_cut(*(vec[0]), *(vec[i]), inner, outer);
        inner.clear();
    }
    //3. Работа с внутренним списком - пока не делается, предположим что её нет
    /*for (int i = inner.size(); i >= 0; i--)
    {
        if (cutter.maxY() < inner[i].minY())
            DrawVeyler(inner);  //Р РµРєСѓСЂСЃРёРІРЅРѕ РїРѕРІС‚РѕСЂСЏРµРј РґР»СЏ РІРЅСѓС‚СЂРµРЅРЅРёС… РїРµСЂРµРґ РѕС‚СЃРµРєР°С‚РµР»РµРј
    }*/
    /*
    std::vector<Point2D> cutter;
    std::vector<Point2D> cutting;
    //4. Работа с внешним списком
    for (int i = 0; i < 1; i++)
    {
        cutter = Polygon3Dto2D(*(vec[i]));

        int outer_size = outer.size();
        for (int j = outer_size-1; j >= 0; j--)
        {
            cutting = outer[j];
            weiler_cut(cutter, cutting, inner, outer);
            outer.erase(outer.begin() + j);

        }

        for (int j = outer.size() - 1; j >= 0; j--)  //Очистка списка
        {
            if (outer[j].size() == 0)
            {
                outer.erase(outer.begin() + j);
                continue;
            }
            if (fabs(OrientedArea(outer[j])) < 0.1)
            {
                outer.erase(outer.begin() + j);
                continue;
            }
        }
        /*
        painter->setPen(QColor(255,0,255));
        for (int k = 0; k < outer.size(); k++)
        {
            for (int j = 0; j < outer[k].size(); j++)
            {
                if (j == outer[k].size() - 1)
                    painter->drawLine(outer[k][j].x + dx, outer[k][j].y + dy, outer[k][0].x + dx, outer[k][0].y + dy);
                else
                    painter->drawLine(outer[k][j].x + dx, outer[k][j].y + dy, outer[k][j+1].x + dx, outer[k][j+1].y + dy);
            }
        }
        qDebug() << outer.size();
        inner.clear();

    }*/
    //5. Финальная отрисовка

    //Отрисовка задников

    for (int i = 0; i < outer.size(); i++)
    {
        for (int j = 0; j < outer[i].size(); j++)
        {
            painter->setPen(QColor(0, 0, 0));

            if (j == outer[i].size() - 1)
                painter->drawLine(outer[i][j].x + dx, outer[i][j].y + dy, outer[i][0].x + dx, outer[i][0].y + dy);
            else
                painter->drawLine(outer[i][j].x + dx, outer[i][j].y + dy, outer[i][j+1].x + dx, outer[i][j+1].y + dy);
            if (outer[i][j].intersec == 1)
            {
                painter->setBrush(QColor(255, 0, 0, 200));   //КРАСНЫЙ - ВЫХОД
                painter->drawEllipse(outer[i][j].x + dx - 3, outer[i][j].y + dy - 3, 6, 6);
            }
            else if (outer[i][j].intersec == -1)
            {
                painter->setBrush(QColor(0, 255, 0, 200));   //Зеленыц- ВХОД
                painter->drawEllipse(outer[i][j].x + dx - 3, outer[i][j].y + dy - 3, 6, 6);
            }
            painter->drawText(dx + outer[i][j].x, dy + outer[i][j].y, QString::number(j));
        }
    }
    for (int j = 0; j < vec[0]->size; j++)  //Отрисовка отсекателя
    {
        //painter->drawText(dx + vec[0]->Point(j).x, dy + vec[0]->Point(j).z, QString::number(j));
        if (j == vec[0]->size - 1)
        {
            drawLine3D(painter, &(vec[0]->Point(j)), &(vec[0]->Point(0)), dx, dy, -100, 100, QColor(255,0,0));
        }
        else
        {
            drawLine3D(painter, &(vec[0]->Point(j)), &(vec[0]->Point(j+1)), dx, dy, -100, 100, QColor(255,0,0));
        }
    }
    /*
    painter->setPen(QColor(0,0,255));
    for (int i = 0; i < inner.size(); i++)
    {
        for (int j = 0; j < inner[i].size(); j++)
        {
            if (j == inner[i].size() - 1)
                painter->drawLine(inner[i][j].x + dx, inner[i][j].y + dy, inner[i][0].x + dx, inner[i][0].y + dy);
            else
                painter->drawLine(inner[i][j].x + dx, inner[i][j].y + dy, inner[i][j+1].x + dx, inner[i][j+1].y + dy);
        }
    }
    //Отрисовка передника
    inner.clear();
    outer.clear();
    */
}


void Crystall::DrawPseudoWeyler(QPainter* painter)
{
    double dx = painter->window().width()/2;
    double dy = painter->window().height()/2;
    //std::vector<Polygon3D*> vec;    //Все полигоны фигуры
    if (vec.size() == 0)
    {
        for (int i = 0; i < faces_count; i++)
            vec.push_back(faces+i);
    }

    //1.Предварительная сортировка
    for (int i = 0; i < faces_count; i++)
    {
        //vec.push_back(faces+i);
        for(int j = vec.size() - 1; j > 0 ; j--)
            if (vec[j]->minY() < vec[j-1]->minY())
            {
                Polygon3D* temp = vec[j];
                vec[j] = vec [j-1];
                vec[j-1] = temp;
            }
    }
    //2. Отрисовка видимых вершин
    /*for (int i = 0; i < faces_count/2; i++)
    for (int i = 0; i < faces_count; i++)
    {
        //Вычисление коэффициента плоскости
        double a1, b1, c1;
        a1 = 0;
        b1 = 0;
        c1 = 0;
        int k;
        for (int j = 0; j < vec[i]->size; j++)
        {
            if (j == vec[i]->size - 1)
                k = 0;
            else
                k = j + 1;
            a1 += (vec[i]->Point(j).y - vec[i]->Point(k).y) * (vec[i]->Point(j).z - vec[i]->Point(k).z);
            b1 += (vec[i]->Point(j).z - vec[i]->Point(k).z) * (vec[i]->Point(j).x - vec[i]->Point(k).x);
            c1 += (vec[i]->Point(j).x - vec[i]->Point(k).x) * (vec[i]->Point(j).y - vec[i]->Point(k).y);
        }
        //qDebug() << c1;
        if (c1 > 0)
            continue;

        for (int j = 0; j < vec[i]->size; j++)  //Отрисовка отсекателя
        {
            //painter->drawText(dx + vec[0]->Point(j).x, dy + vec[0]->Point(j).z, QString::number(j));
            if (j == vec[i]->size - 1)
            {
                drawLine3D(painter, &(vec[i]->Point(j)), &(vec[i]->Point(0)), dx, dy, -100, 10000, QColor(0,0,0));
            }
            else
            {
                drawLine3D(painter, &(vec[i]->Point(j)), &(vec[i]->Point(j+1)), dx, dy, -100, 10000, QColor(0,0,0));
            }
        }
    }*/
}

//bool intersect(Point2D A1, Point2D B1, Point2D A2, Point2D B2, Point2D& result)
int intersect(Point2D A1, Point2D B1, Point2D A2, Point2D B2, Point2D& left, Point2D& right)
{
    if (aq(A1.x, B1.x) && aq(A1.y, B1.y))
        return 0;
    if (aq(A2.x, B2.x) && aq(A2.y, B2.y))
        return 0;
    //Первичная проверка
    if (!proj_intersec(A1.x, B1.x, A2.x, B2.x) || !proj_intersec(A1.y, B1.y, A2.y, B2.y))
        return 0;
    line l1(A1, B1);
    line l2(A2, B2);
    double zn = det (l1.a, l1.b, l2.a, l2.b);
    if (fabs (zn) < 0.00001) {
        if (fabs (l1.dist (A2)) > 0.00001 || fabs (l2.dist (A1)) > 0.00001)
            return 0;
        left.x = A1.x;
        left.y = A1.y;
        right.x = B1.x;
        right.y = B1.y;
        return 1;
    }
    else {
    left.x = - det (l1.c, l1.b, l2.c, l2.b) / zn;
    left.y = - det (l1.a, l1.c, l2.a, l2.c) / zn;
    if (betw (A1.x, B1.x, left.x)
        && betw (A1.y, B1.y, left.y)
        && betw (A2.x, B2.x, left.x)
        && betw (A2.y, B2.y, left.y))
        return 1;   //Одна точка
    return 0;
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
    //0. Р СџРЎР‚Р ВµР С•Р В±РЎР‚Р В°Р В·Р С•Р Р†Р В°Р Р…Р С‘Р Вµ Р С‘Р В· Polygon3D Р Р† Р Р†Р ВµР С”РЎвЂљР С•РЎР‚Р В° РЎвЂљР С•РЎвЂЎР ВµР С”, Р С•РЎР‚Р С–Р В°Р Р…Р С‘Р В·Р С•Р Р†Р В°Р Р…Р Р…РЎвЂ№Р Вµ Р С—Р С• РЎвЂЎР В°РЎРѓР С•Р Р†Р С•Р в„– РЎРѓРЎвЂљРЎР‚Р ВµР В»Р С”Р Вµ
    //std::vector<Point2D> cutter = Polygon3Dto2D(_cutter);   //Р С›Р В±РЎР‚Р ВµР В·Р В°РЎР‹РЎвЂ°Р С‘Р в„– Р СР Р…Р С•Р С–Р С•РЎС“Р С–Р С•Р В»РЎРЉР Р…Р С‘Р С”
    //std::vector<Point2D> cutting = Polygon3Dto2D(_cutting); //Р С›Р В±РЎР‚Р ВµР В·Р В°Р ВµР СРЎвЂ№Р в„– Р СР Р…Р С•Р С–Р С•РЎС“Р С–Р С•Р В»РЎРЉР Р…Р С‘Р С”
    std::vector<int> entering;
    std::vector<int> exiting;
    int flag = 0;
    //1. РўРѕС‡РєРё РїРµСЂРµСЃРµС‡РµРЅРёСЏ Рё РёС… РєР»Р°СЃСЃРёС„РёРєР°С†РёСЏ
    for (int i = 0; i < cutting.size(); i++) //РћР±СЂРµР·Р°РµРјС‹Р№
    {
        //РќР°С‡Р°Р»СЊРЅР°СЏ Рё РєРѕРЅРµС‡РЅР°СЏ РІРµСЂС€РёРЅС‹
        Point2D A = cutting[i];
        Point2D B = (i < cutting.size()-1)?cutting[i+1]:cutting[0];
        //qDebug()<<"Cutting " << A.x << "," << A.y << " <>"<<B.x << "," << B.y;
        for (int j = 0; j < cutter.size(); j++)    //РћР±СЂРµР·Р°С‚РµР»СЊ
        {
            //РќР°С‡Р°Р»СЊРЅР°СЏ Рё РєРѕРЅРµС‡РЅР°СЏ РІРµСЂС€РёРЅС‹
            Point2D Start = cutter[j];
            Point2D Finish = (j < cutter.size()-1)?cutter[j+1]:cutter[0];
            //qDebug()<<"Cutter " << Start.x << "," << Start.y << " <>"<<Finish.x << "," << Finish.y;
            Point2D intersection;
            Point2D second_int;
            int count = intersect(A, B, Start, Finish, intersection, second_int);
            if (count != 0)
            {
                Point2D AB(B.x - A.x, B.y - A.y, 0);
                Point2D CD(Finish.x - Start.x, Finish.y - Start.y, 0);
                Point2D Normal(-AB.y, AB.x);
                if (flag == 0) //Если еще не определено
                {
                    if (count == 2)
                    {
                        if (scalar(AB,CD) > 0)  //Сначала выход потом вхож
                            flag = -1;
                        else
                            flag = 1;

                        cutting.insert(cutting.begin()+i+1, Point2D(intersection.x, intersection.y, flag));
                        if (flag == -1)
                            entering.push_back(i+1);
                        else
                            exiting.push_back(i+1);
                        cutter.insert(cutter.begin()+j+1, Point2D(intersection.x, intersection.y, flag));
                        i++;
                        j++;
                        flag = -flag;
                        cutting.insert(cutting.begin()+i+1, Point2D(second_int.x, second_int.y, flag));
                        if (flag == -1)
                            entering.push_back(i+1);
                        else
                            exiting.push_back(i+1);
                        cutter.insert(cutter.begin()+j+1, Point2D(second_int.x, second_int.y, flag));
                        i++;
                        j++;
                        flag = -flag;

                    }
                    else if (scalar(CD, Normal) < 0) //Р’С…РѕРґСЏС‰Р°СЏ РІРµСЂС€РёРЅР°
                    {
                        cutting.insert(cutting.begin()+i+1, Point2D(intersection.x, intersection.y, -1));
                        entering.push_back(i+1);
                        cutter.insert(cutter.begin()+j+1, Point2D(intersection.x, intersection.y, -1));
                        i++;
                        j++;
                        flag = 1;

                    }
                    else    //Р’С‹С…РѕРґСЏС‰Р°СЏ РІРµСЂС€РёРЅР°
                    {
                        cutting.insert(cutting.begin()+i+1, Point2D(intersection.x, intersection.y, 1));
                        exiting.push_back(i+1);
                        cutter.insert(cutter.begin()+j+1, Point2D(intersection.x, intersection.y, 1));
                        i++;
                        j++;
                        flag = -1;
                    }
                }
                else
                {
                    cutting.insert(cutting.begin()+i+1, Point2D(intersection.x, intersection.y, flag));
                    if (flag == -1)
                        entering.push_back(i+1);
                    else
                        exiting.push_back(i+1);
                    cutter.insert(cutter.begin()+j+1, Point2D(intersection.x, intersection.y, flag));
                    i++;
                    j++;
                    flag = -flag;
                    if (count == 2)
                    {
                        cutting.insert(cutting.begin()+i+1, Point2D(second_int.x, second_int.y, flag));
                        if (flag == -1)
                            entering.push_back(i+1);
                        else
                            exiting.push_back(i+1);
                        cutter.insert(cutter.begin()+j+1, Point2D(second_int.x, second_int.y, flag));
                        i++;
                        j++;
                        flag = -flag;
                    }
                }
            }
        }
    }
    if (entering.size() == 0 || exiting.size() == 0)
    {
        //return;
        //if ( (cutter[1].x - cutter[0].x) * (cutting[0].y - cutter[0].y) + (cutter[1].y - cutter[0].y) * (cutting[0].x - cutter[0].x) >= 0 )
            outer.push_back(cutting);
    }
    if (cutter.size() == 0 || cutting.size() == 0)
        return;
    qDebug() << "Enter" << entering.size();
    qDebug() << "Exit" << exiting.size();
    //2. РџРѕР»СѓС‡РµРЅРёРµ РІРЅСѓС‚СЂРµРЅРЅРµРіРѕ СЃРїРёСЃРѕРє
    for (int i = 0; i < entering.size(); i++)
    {
        if (!cutting[entering[i]].entered)    //Р•СЃР»Рё РІРµСЂС€РёРЅР° РµС‰Рµ РЅРµ РїРѕСЃРµС‰РµРЅР°
        {
            std::vector<Point2D> inside;
            int start = entering[i];
            cutting[entering[i]].entered = true; //Флаг
            inside.push_back(cutting[start]);

            bool finished = false;
            for (int enter = start; !finished ;)
            {
                int exit;
                std::vector<Point2D> outside;   //Р’РќР•РЁРќРР™ РџРћР›РР“РћРќ
                outside.push_back(cutting[enter]);

                int enter_cutter = 0;   //Аналог вершине cutting[enter]
                for (int j = 0; j < cutter.size(); j++)
                {
                    if (cutter[j].x == cutting[enter].x && cutter[j].y == cutting[enter].y)
                    {
                        enter_cutter = j;   //Searching for cutter start
                        break;
                    }
                }
                for (int j = enter_cutter; ; (j==cutter.size()-1)?j=0:j++)  //Поиск выходящей вершины
                {
                    if (j == enter_cutter)  //Looking for next exit
                        continue;
                    inside.push_back(cutter[j]);
                    outside.push_back(cutter[j]);
                    if (cutter[j].intersec == 1)
                    {
                        exit = j;
                        break;
                    }
                }
                int exit_cutting = 0;   //Аналог вершине cutter[exit]
                for (int j = 0; j < exiting.size(); j++)
                {
                    if (cutting[exiting[j]].x == cutter[exit].x && cutting[exiting[j]].y == cutter[exit].y)
                        exit_cutting = exiting[j];
                }
                for (int j = exit_cutting; ;(j==0)?j=cutting.size()-1:j--)  //"Закрытие" внешнего полигона
                {
                    if (j == exit_cutting)
                        continue;
                    if (j != enter)
                        outside.push_back(cutting[j]);
                    else
                        break;
                }
                outer.push_back(outside);
                for(int j = exit_cutting; cutting.size()!=0; (j==cutting.size()-1)?j=0:j++)  //Р”РѕР±Р°РІР»РµРЅРёРµ РІРЅРµС€РЅРёС… РІРµСЂС€РёРЅ
                {
                    //Looking for next enter
                    if (j == exit_cutting)
                        continue;
                    if (cutting[j].intersec == -1)  //Р•СЃР»Рё РЅР°Р№РґРµРЅР° РІС…РѕРґРЅР°СЏ РІРµСЂС€РёРЅР°
                    {
                        if (j == enter ) //Convex
                        {
                            finished = true;    //"Р—РђРљР Р«РўРР•" РїРѕР»РёРіРѕРЅР°
                            inner.push_back(inside);
                            break;
                        }
                        else if (!cutting[j].entered)   //new search
                        {
                            enter = j;  //new entering point
                            inside.push_back(cutting[j]);
                            cutting[j].entered = true;   //set flag
                            break;
                        }
                        else    //if found old start
                        {
                            finished = true;
                            //inside.push_back(cutting[j]);
                            break;
                        }
                    }
                    else
                        inside.push_back(cutting[j]);   //Р”РћР‘РђР’Р›Р•РќРР• Р’РќР•РЁРќР•Р™ Р’Р•Р РЁРРќР«
                }
            }
            //inner.push_back(inside); (?)
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

double OrientedArea(std::vector<Point2D>& in)   //Р’С‹С‡РёСЃР»СЏРµС‚ РѕСЂРёРµРЅС‚РёСЂРѕРІР°РЅРЅСѓСЋ РїР»РѕС‰Р°РґСЊ РїСЂРѕРµРєС†РёРё РјРЅРѕРіРѕСѓРіРѕР»СЊРЅРёРєР° РЅР° XOZ
{
    double sum = 0;
    int last = in.size() - 1;
    for (int i = 0; i < in.size(); i++)
    {
        if (i != 0)
            if (aq(in[i].x, in[i-1].x) && aq(in[i].y, in[i-1].y))
            {
                in.erase(in.begin() + i);
                i--;
                continue;
            }
        sum += in[last].x * in[i].y - in[last].y * in[i].x;
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
        {
            if (i != in.size-1)
                if (aq(in.Point(i).x, in.Point(i+1).x) && aq(in.Point(i).z, in.Point(i+1).z))
                    continue;
            result.push_back(Point2D(in.Point(i).x, in.Point(i).z, 0));
        }
    }
    else
    {
        for (int i = 0; i < in.size; i++)
        {
            if (i != 0)
                if (aq(in.Point(i).x, in.Point(i-1).x) && aq(in.Point(i).z, in.Point(i-1).z))
                    continue;
            result.push_back(Point2D(in.Point(i).x, in.Point(i).z, 0));
        }
    }

    return result;
}

double scalar(Point2D a, Point2D b)
{
    return a.x * b.x + a.y * b.y;
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

bool aq(double a, double b)
{
    return (min(a, b) + 0.00001 > max(a, b) - 0.00001);
}




