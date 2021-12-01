#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <iostream>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    using namespace std;

//начало ввода координат препятствий и позиций
    QStringList items;
    items << tr("Добавить новое препятствие")
          << tr("Установить начальную позицию")
          << tr("Установить конечную позицию")
          << tr("Установить размер робота")
          << tr("Завершить ввод");
    bool ok;
    QString item;
    while(item != "Завершить ввод"){
        ok = false;
        item = QInputDialog::getItem(this, "Меню ввода",tr("Следующее действие:"),
                     items, 0, false, &ok);
        obstacle gamma;
        int x, y;
        if(ok && item == "Добавить новое препятствие"){
            ok = false;
            gamma.x = QInputDialog::getInt(this, tr("Ввод X"),
                                         tr("Значение"), 0, 0, maxX, 1, &ok);
            gamma.y = QInputDialog::getInt(this, tr("Ввод Y"),
                                         tr("Значение"), 0, 0, maxY, 1, &ok);
            gamma.x += indent;
            gamma.y += indent;
            gamma.w = QInputDialog::getInt(this, tr("Ввод ширины фигуры"),
                                         tr("Значение"), 0, 0, maxX - indent, 1, &ok);
            gamma.h = QInputDialog::getInt(this, tr("Ввод высоты фигуры"),
                                         tr("Значение"), 0, 0, maxY - indent, 1, &ok);
            obstacles.emplace_back(gamma);

        } else if(ok && item == "Установить начальную позицию"){
            h1:
            ok = false;
            x = QInputDialog::getInt(this, tr("Ввод X"),
                                         tr("Значение"), 0, indent, (maxX - indent - step), 1, &ok);
            y = QInputDialog::getInt(this, tr("Ввод Y"),
                                         tr("Значение"), 0, indent, (maxY - indent - step), 1, &ok);
            x = (x / step) * step;
            y = (y / step) * step;
            int f = (r / step) * step + indent;
            if(r % step != 0){
                f += step;
            }
            if(x < f || x > (maxX - f + step) || y < f || y > (maxY - f + step)){
                QMessageBox messageBox;
                messageBox.critical(0,"Ошибка","Выбранная точка находится за границей доступного поля");
                messageBox.setFixedSize(500,200);
                goto h1;
            }
            startX = x;
            startY = y;

        } else if(ok && item == "Установить конечную позицию"){
            h2:
            ok = false;
            x = QInputDialog::getInt(this, tr("Ввод X"),
                                         tr("Значение"), 0, indent, (maxX - indent - step), 1, &ok);
            y = QInputDialog::getInt(this, tr("Ввод Y"),
                                         tr("Значение"), 0, indent, (maxY - indent - step), 1, &ok);
            x = (x / step) * step;
            y = (y / step) * step;
            int f = (r / step) * step + indent;
            if(r % step != 0){
                f += step;
            }
            if(x < f || x > (maxX - f + step) || y < f || y > (maxY - f + step)){
                QMessageBox messageBox;
                messageBox.critical(0,"Ошибка","Выбранная точка находится за границей доступного поля");
                messageBox.setFixedSize(500,200);
                goto h2;
            }
            endX = x;
            endY = y;

        } else if(ok && item == "Установить размер робота"){
            ok = false;
            r = QInputDialog::getInt(this, tr("Ввод радиуса"),
                                     tr("Значение"), 0, 0, 50, 1, &ok);

        } else if(ok && item == "Завершить ввод"){
            break;
        } else if(!ok){
            exit(0);
        }
    }
    if(!startX || !startY){
        QMessageBox messageBox;
        messageBox.critical(0,"Ошибка","Не введена точка старта");
        messageBox.setFixedSize(500,200);
        goto h1;
    }
    if(!endX || !endY){
        QMessageBox messageBox;
        messageBox.critical(0,"Ошибка","Не введена точка финиша");
        messageBox.setFixedSize(500,200);
        goto h2;
    }
//конец ввода координат препятствий и позиций

    int s, s1;
    s1 = (maxX - indent) / step;
    s = (maxY - indent) / step;
    obstaclesAreas = new bool*[s];
    for(int i = 0; i < s; i++){
        obstaclesAreas[i] = new bool[s1];
        for(int j = 0; j < s1; j++){
            obstaclesAreas[i][j] = false;
        }
    }

    //Увеличение препятствий на радиус робота
    //и заполнение частично затронутых клеток
    for(auto &k : obstacles){
        k.x -= r;
        k.y -= r;
        if(k.x % step != 0){
            k.x = (k.x / step) * step;
        }
        if(k.y % step != 0){
            k.y = (k.y / step) * step;
        }

        if(r % step != 0){
            k.h += 2 * (r / step) + 2 * step;
            if(k.h % step != 0){
                k.h = (k.h / step) * step;

            }

            k.w += 2 * (r / step) + 2 * step;
            if(k.w % step != 0){
                k.w = (k.w / step) * step;

            }
        } else {
            k.h += 2 * r;
            k.w += 2 * r;
        }
        if(k.x < indent){
            k.w -= abs(k.x);
            k.x = indent;
        } else if(k.x > maxX){
            k.x = maxX;
        }
        if(k.y < indent){
            k.h -= abs(k.y);
            k.y = indent;
        } else if(k.y > maxY){
            k.y = maxY;
        }
        if(k.w + k.x > maxX - indent){
            k.w -= (k.w + k.x) - maxX;
        }
        if(k.h + k.y > maxY - indent){
            k.h -= (k.h + k.y) - maxY;
        }

        s = (k.x - indent + k.w) / step;
        s1 = (k.y - indent + k.h) / step;
        for(int j = (k.y - indent) / step; j < s1; j++){
            for(int l = (k.x - indent) / step; l < s; l++){
                obstaclesAreas[j][l] = true;
            }
        }

    }

    Graph area;
    s1 = (maxX - indent) / step;
    s = (maxY - indent) / step;

    int p = (r / 10) * 10;
    if(r % step != 0){
        p += step;
    }

    //верхняя грань
    for(int i = 0; i < (p / step); i++){
        for(int j = 0; j < s1; j++){
            obstaclesAreas[i][j] = true;
        }
    }
    //левая грань
    for(int i = 0; i < s; i++){
        for(int j = 0; j < (p / step); j++){
            obstaclesAreas[i][j] = true;
        }
    }
    //нижняя грань
    for(int i = s-1; i >= s - (p / step); i--){
        for(int j = 0; j < s1; j++){
            obstaclesAreas[i][j] = true;
        }
    }
    //правая грань
    for(int i = 0; i < s; i++){
        for(int j = s1 - (p / step); j < s1; j++){
            obstaclesAreas[i][j] = true;
        }
    }

    /*cout << "\n------------------------------------\n";
    for(int i = 0; i < s; i++){
        for(int j = 0; j < s1; j++){
            if(obstaclesAreas[i][j]){
                cout << "1 ";
            } else {
               cout << "0 ";
            }
        }
        cout << endl;
    }
    cout << "\n------------------------------------\n";*/


    for(int i = 0; i < s; i++){
        for(int j = 0; j < s1; j++){

            if(obstaclesAreas[i][j]){
                continue;
            }

            if(area.isEmpty()){
                area.addVertex(j, i);
                continue;
            }

            if(!obstaclesAreas[i][j]) {
                if(!obstaclesAreas[i][j-1]){
                    //Новая вершина в направлении 3 от родителя
                    area.addVertexByParent(j - 1, i, j, i, 3);
                } else {
                    area.addVertex(j, i);
                }
                //Связь с вершиной в направлении 0
                if(i != 0 && j != 0 && !obstaclesAreas[i-1][j-1]) {
                    area.addEdge(j, i, j-1, i-1, 0);
                }

                //Связь с вершиной в направлении 1
                if(i != 0 && !obstaclesAreas[i-1][j]) {
                    area.addEdge(j, i, j, i - 1, 1);
                }

                //Связь с вершиной в направлении 2
                if(i != 0 && j != (s1 - 1) && !obstaclesAreas[i-1][j+1]) {
                    area.addEdge(j, i, j + 1, i-1, 2);
                }
            } else {
                continue;
            }
        }
    }

    pair<int, int> startCell = pair((startX - indent) / step, (startY - indent) / step);
    pair<int, int> endCell = pair((endX - indent) / step, (endY - indent) / step);

    fullWay = area.deikstrasAlgo(startCell.first, startCell.second, endCell.first, endCell.second);


    ui->setupUi(this);
    this->setWindowTitle(" РГЗ по МПиАА / Бернадский");

}

pair<int, int> coords(pair<int, int> value){
    return pair(value.first * step + step / 2,
                value.second * step + step / 2);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this); // Создаём объект отрисовщика
    // Устанавливаем кисть абриса
    painter.setPen(QPen(Qt::gray, 0, Qt::SolidLine, Qt::FlatCap));
    painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
    int s = (r / 10) * 10;
    if(r % step != 0){
        s += step;
    }
    painter.drawRect(indent, indent, s, maxY - indent);
    painter.drawRect(indent, indent, maxX - indent, s);
    painter.drawRect(maxX - s, indent, s, maxY - indent);
    painter.drawRect(indent, maxY - s, maxX - indent, s);

    //Прорисовка сетки

    for(int i = indent; i <= maxX; i+=step){
        painter.drawLine(i, indent, i, maxY);
    }
    for(int i = indent; i <= maxY; i+=step){
        painter.drawLine(indent, i, maxX, i);
    }

    //прорисовка препятствий
    for(auto s : obstacles){
        painter.drawRect(s.x, s.y, s.w, s.h);
    }



    painter.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::FlatCap));

    for(int i = 0; i < fullWay.size() - 1; i++){
        auto e = coords(pair(fullWay[i].x + indent / step, fullWay[i].y + indent / step));
        auto e1 = coords(pair(fullWay[i+1].x + indent / step, fullWay[i+1].y + indent / step));
        painter.drawLine(e.first, e.second, e1.first, e1.second);
    }

    //установка точек начала и конца маршрута
    painter.setPen(QPen(Qt::gray, 0, Qt::SolidLine, Qt::FlatCap));
    painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
    painter.drawEllipse(startX, startY, step, step);
    painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
    painter.drawEllipse(endX, endY, step, step);

}

MainWindow::~MainWindow()
{
    delete ui;
}

