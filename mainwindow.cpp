#include "mainwindow.h"
#include "ui_mainwindow.h"

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
            ok = false;
            x = QInputDialog::getInt(this, tr("Ввод X"),
                                         tr("Значение"), 0, indent, (maxX - indent - step), 1, &ok);
            y = QInputDialog::getInt(this, tr("Ввод Y"),
                                         tr("Значение"), 0, indent, (maxY - indent - step), 1, &ok);
            x = (x / step) * step;
            y = (y / step) * step;

            startX = x;
            startY = y;

        } else if(ok && item == "Установить конечную позицию"){
            ok = false;
            x = QInputDialog::getInt(this, tr("Ввод X"),
                                         tr("Значение"), 0, indent, (maxX - indent - step), 1, &ok);
            y = QInputDialog::getInt(this, tr("Ввод Y"),
                                         tr("Значение"), 0, indent, (maxY - indent - step), 1, &ok);
            x = (x / step) * step;
            y = (y / step) * step;

            endX = x;
            endY = y;

        } else if(ok && item == "Установить размер робота"){
            ok = false;
            r = QInputDialog::getInt(this, tr("Ввод радиуса"),
                                     tr("Значение"), 0, 0, 50, 1, &ok);

        } else if(!ok){
            break;
        }
    }
//конец ввода координат препятствий и позиций

    int s = (maxX - indent) / step;
    int s1 = (maxY - indent) / step;
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
        s = k.w / step;
        s1 = k.h / step;
        for(int j = (k.x - indent) / step; j < s; j++){
            for(int l = (k.y - indent) / step; l < s1; l++){
                obstaclesAreas[j][l] = true;
            }
        }

        if(k.w + k.x > maxX - indent){
            k.w -= (k.w + k.x) - maxX;
        }
        if(k.h + k.y > maxY - indent){
            k.h -= (k.h + k.y) - maxY;
        }
    }

    Graph area;
    s = (maxX - indent) / step;
    s1 = (maxY - indent) / step;
    for(int i = 0; i < s; i++){
        for(int j = 0; j < s1; j++){

            if(obstaclesAreas[i][j]){
                continue;
            }

            if(area.isEmpty()){
                area.addVertex(i, j);
                continue;
            }

        }
    }
    ui->setupUi(this);
    this->setWindowTitle("qwerty");
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this); // Создаём объект отрисовщика
    // Устанавливаем кисть абриса
    painter.setPen(QPen(Qt::gray, 0, Qt::SolidLine, Qt::FlatCap));

    //Прорисовка сетки
    painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
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

    //установка точек начала и конца маршрута
    painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
    painter.drawEllipse(startX, startY, step, step);
    painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
    painter.drawEllipse(endX, endY, step, step);
}

MainWindow::~MainWindow()
{
    delete ui;
}

