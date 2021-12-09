#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <iostream>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    using namespace std;

    ui->setupUi(this);
    this->setWindowTitle(" РГЗ по МПиАА / Бернадский");

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

    createGraph();
}

pair<int, int> MainWindow::coords(pair<int, int> value){
    return pair(value.first * step + step / 2,
                value.second * step + step / 2);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this); // Создаём объект отрисовщика
    // Устанавливаем кисть абриса
    autoFillBackground();
    painter.setPen(QPen(Qt::gray, 0, Qt::SolidLine, Qt::FlatCap));
    painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
    this->setWindowIcon(QIcon(":/logo/images/logo.ico"));


    if(!standart){
        int s = (r / step) * step;
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

        painter.setPen(QPen(Qt::gray, 0, Qt::NoPen, Qt::FlatCap));
        //прорисовка препятствий
        for(auto s : obstacles){
            painter.drawRect(s.x, s.y, s.w, s.h);
        }

        painter.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap));

        if(!fullWay.empty()) {
            for(int i = 0; i < fullWay.size() - 1; i++){
                auto e = coords(pair(fullWay[i].x + indent / step, fullWay[i].y + indent / step));
                auto e1 = coords(pair(fullWay[i+1].x + indent / step, fullWay[i+1].y + indent / step));
                painter.drawLine(e.first, e.second, e1.first, e1.second);
            }
        }

        //установка точек начала и конца маршрута
        painter.setPen(QPen(Qt::gray, 0, Qt::SolidLine, Qt::FlatCap));
        if(startX != -1){
            painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
            painter.drawEllipse(startX, startY, step, step);
        }
        if(endX != -1){
            painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
            painter.drawEllipse(endX, endY, step, step);
        }
    } else {
        painter.drawRect(indent, indent, 2, maxY - indent);
        painter.drawRect(indent, indent+1, maxX - indent, 2);
        painter.drawRect(maxX - 1, indent, 1, maxY - indent);
        painter.drawRect(indent, maxY - 1, maxX - indent, 1);

        painter.setPen(QPen(Qt::gray, 0, Qt::NoPen, Qt::FlatCap));
        //прорисовка препятствий
        for(auto s : classicalObstacles){
            painter.drawRect(s.x, s.y, s.w, s.h);
        }

        painter.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap));

        if(!fullWay.empty()){
            for(int i = 0; i < fullWay.size() - 1; i++){
                auto e = coords(pair(fullWay[i].x + indent / step, fullWay[i].y + indent / step));
                auto e1 = coords(pair(fullWay[i+1].x + indent / step, fullWay[i+1].y + indent / step));
                painter.drawLine(e.first, e.second, e1.first, e1.second);
            }
        }

        //установка начального и конечного положения робота
        painter.setPen(QPen(Qt::gray, 0, Qt::SolidLine, Qt::FlatCap));
        if(startX != -1){
            painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
            painter.drawEllipse(startX + step / 2 - r / 2, startY + step / 2 - r / 2, r, r);
        }
        if(endX != -1){
            painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
            painter.drawEllipse(endX + step / 2 - r / 2, endY + step / 2 - r / 2, r, r);
        }
    }
}

void MainWindow::createGraph(){
    area.clear();
    fullWay.clear();
    int s1 = (maxX - indent) / step;
    int s = (maxY - indent) / step;
    for(int i = 0; i < s; i++){
        for(int j = 0; j < s1; j++){

            if(obstaclesAreas[i][j]){
                continue;
            }

            if(area.isEmpty()){
                area.addVertex(j, i);
                continue;
            }

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
        }
    }
    if(startX != -1 && endX != -1 &&
            !(obstaclesAreas[(startY - indent) / step][(startX - indent) / step] ||
             obstaclesAreas[(endY - indent) / step][(endX - indent) / step])){
        pair<int, int> startCell = pair((startX - indent) / step, (startY - indent) / step);
        pair<int, int> endCell = pair((endX - indent) / step, (endY - indent) / step);
        fullWay = area.deikstrasAlgo(startCell.first, startCell.second, endCell.first, endCell.second);
    }
    if((startX != -1 && endX != -1) && (fullWay.empty() ||
        obstaclesAreas[(startY - indent) / step][(startX - indent) / step] ||
        obstaclesAreas[(endY - indent) / step][(endX - indent) / step])){
        QMessageBox messageBox;
        messageBox.warning(0,"Ошибка","Невозможно построить путь");
        messageBox.setFixedSize(500,200);
        fullWay.clear();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

//смена точки старта
void MainWindow::on_action_2_triggered()
{
    fullWay.clear();
    bool ok = false;
    int x = QInputDialog::getInt(this, tr("Ввод X"),
                                 tr("Значение"), 0, 0, (maxX - indent - step), 1, &ok);
    int y = QInputDialog::getInt(this, tr("Ввод Y"),
                                 tr("Значение"), 0, 0, (maxY - indent - step), 1, &ok);
    classicalStartX = x;
    classicalStartY = y;
    x = (x / step) * step + indent;
    y = (y / step) * step + indent;

    /*if(x < f || x > (maxX - f + step) || y < f || y > (maxY - f + step)){
        QMessageBox messageBox;
        messageBox.warning(0,"Ошибка","Выбранная точка находится за границей доступного поля");
        messageBox.setFixedSize(500,200);
        goto h1;
    }*/
    startX = x;
    startY = y;
    if (startX == endX && startY == endY){
        QMessageBox messageBox;
        messageBox.warning(0,"Ошибка","Выбранная точка совпадает с точкой финиша");
        messageBox.setFixedSize(500,200);
    }
    if(startX != -1 && endX != -1 &&
            !(obstaclesAreas[(startY - indent) / step][(startX - indent) / step] ||
             obstaclesAreas[(endY - indent) / step][(endX - indent) / step])){
        pair<int, int> startCell = pair((startX - indent) / step, (startY - indent) / step);
        pair<int, int> endCell = pair((endX - indent) / step, (endY - indent) / step);
        fullWay = area.deikstrasAlgo(startCell.first, startCell.second, endCell.first, endCell.second);
    }
    if((startX != -1 && endX != -1) && (fullWay.empty() ||
        obstaclesAreas[(startY - indent) / step][(startX - indent) / step] ||
        obstaclesAreas[(endY - indent) / step][(endX - indent) / step])){
        QMessageBox messageBox;
        messageBox.warning(0,"Ошибка","Невозможно построить путь");
        messageBox.setFixedSize(500,200);
        fullWay.clear();
    }

}

//смена точки финиша
void MainWindow::on_action_3_triggered()
{
    fullWay.clear();
    bool ok = false;
    int x = QInputDialog::getInt(this, tr("Ввод X"),
                                 tr("Значение"), 0, 0, (maxX - indent - step), 1, &ok);
    int y = QInputDialog::getInt(this, tr("Ввод Y"),
                                 tr("Значение"), 0, 0, (maxY - indent - step), 1, &ok);
    classicalEndX = x;
    classicalEndY = y;
    x = (x / step) * step + indent;
    y = (y / step) * step + indent;
    int f = (r / step) * step + indent;
    if(r % step != 0){
        f += step;
    }
    /*if(x < f || x > (maxX - f + step) || y < f || y > (maxY - f + step)
            || (startX == endX && startY == endY)){
        QMessageBox messageBox;
        messageBox.warning(0,"Ошибка","Выбранная точка находится за границей доступного поля");
        messageBox.setFixedSize(500,200);
        goto h2;
    }*/
    endX = x;
    endY = y;
    if (startX == endX && startY == endY){
        QMessageBox messageBox;
        messageBox.warning(0,"Ошибка","Выбранная точка совпадает с точкой старта");
        messageBox.setFixedSize(500,200);
    }
    if(startX != -1 && endX != -1 &&
            !(obstaclesAreas[(startY - indent) / step][(startX - indent) / step] ||
             obstaclesAreas[(endY - indent) / step][(endX - indent) / step])){
        pair<int, int> startCell = pair((startX - indent) / step, (startY - indent) / step);
        pair<int, int> endCell = pair((endX - indent) / step, (endY - indent) / step);
        fullWay = area.deikstrasAlgo(startCell.first, startCell.second, endCell.first, endCell.second);
    }
    if((startX != -1 && endX != -1) && (fullWay.empty() ||
        obstaclesAreas[(startY - indent) / step][(startX - indent) / step] ||
        obstaclesAreas[(endY - indent) / step][(endX - indent) / step])){
        QMessageBox messageBox;
        messageBox.warning(0,"Ошибка","Невозможно построить путь");
        messageBox.setFixedSize(500,200);
        fullWay.clear();
    }
}

//новое препятствие
void MainWindow::on_action_triggered()
{
    obstacle gamma;
    int s, s1;
    bool ok = false;
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
    if(gamma.w == 0 || gamma.h == 0) return;
    classicalObstacles.emplace_back(gamma);
    gamma.x -= r;
    gamma.y -= r;
    if(gamma.x % step != 0){
        gamma.x = (gamma.x / step) * step;
    }
    if(gamma.y % step != 0){
        gamma.y = (gamma.y / step) * step;
    }

    if(r % step != 0){
        gamma.h += 2 * (r / step) + 2 * step;
        if(gamma.h % step != 0){
            gamma.h = (gamma.h / step) * step;

        }

        gamma.w += 2 * (r / step) + 2 * step;
        if(gamma.w % step != 0){
            gamma.w = (gamma.w / step) * step;

        }
    } else {
        gamma.h += 2 * r;
        gamma.w += 2 * r;
    }
    if(gamma.x < indent){
        gamma.w -= abs(gamma.x);
        gamma.x = indent;
    } else if(gamma.x > maxX){
        gamma.x = maxX;
    }
    if(gamma.y < indent){
        gamma.h -= abs(gamma.y);
        gamma.y = indent;
    } else if(gamma.y > maxY){
        gamma.y = maxY;
    }
    if(gamma.w + gamma.x > maxX - indent){
        gamma.w -= (gamma.w + gamma.x) - maxX;
    }
    if(gamma.h + gamma.y > maxY - indent){
        gamma.h -= (gamma.h + gamma.y) - maxY;
    }

    s = (gamma.x - indent + gamma.w) / step;
    s1 = (gamma.y - indent + gamma.h) / step;
    for(int j = (gamma.y - indent) / step; j < s1; j++){
        for(int l = (gamma.x - indent) / step; l < s; l++){
            obstaclesAreas[j][l] = true;
        }
    }
    obstacles.emplace_back(gamma);
    //пересоздание графа

    createGraph();
}

//стандартный вид
void MainWindow::on_action_8_triggered()
{
    standart = true;
    repaint();
}

//клеточный вид
void MainWindow::on_action_9_triggered()
{
    standart = false;
    repaint();
}

void MainWindow::updateObstacles(){

    obstacles.clear();
    int s, s1;
    s1 = (maxX - indent) / step;
    s = (maxY - indent) / step;
    delete obstaclesAreas;
    obstaclesAreas = new bool*[s];
    for(int i = 0; i < s; i++){
        obstaclesAreas[i] = new bool[s1];
        for(int j = 0; j < s1; j++){
            obstaclesAreas[i][j] = false;
        }
    }
    for(auto k : classicalObstacles){
        k.x -= r;
        k.y -= r;
        if(k.x % step != 0){
            k.x = (k.x / step) * step;
        }
        if(k.y % step != 0){
            k.y = (k.y / step) * step;
        }

        if(k.h % step != 0){
            k.h = (k.h / step) * step;
            k.h += step;
        }
        if(k.w % step != 0){
            k.w = (k.w / step) * step;
            k.w += step;
        }

        if(r % step != 0){
            k.h += 2 * (r / step) * step;
            if(r % step != 0){
                k.h += 2 * step;

            }

            k.w += 2 * (r / step) * step;
            if(r % step != 0){
                k.w += 2 * step;

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

        obstacles.push_back(k);

        s = (k.x - indent + k.w) / step;
        s1 = (k.y - indent + k.h) / step;
        for(int j = (k.y - indent) / step; j < s1; j++){
            for(int l = (k.x - indent) / step; l < s; l++){
                obstaclesAreas[j][l] = true;
            }
        }

    }

    s1 = (maxX - indent) / step;
    s = (maxY - indent) / step;

    int p = (r / step) * step;
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
}

//изменить размер робота
void MainWindow::on_action_4_triggered()
{
    bool ok = false;
    r = QInputDialog::getInt(this, tr("Ввод радиуса"),
                             tr("Значение"), 0, 1, 50, 1, &ok);

    if(!ok){
        return;
    }

    updateObstacles();

    createGraph();
}

//изменить размер клетки
void MainWindow::on_action_5_triggered()
{
    if(step == 10){
        step = 20;
    } else {
        step = 10;
    }
    startX = (classicalStartX / step) * step + indent;
    startY = (classicalStartY / step) * step + indent;
    endX = (classicalEndX / step) * step + indent;
    endY = (classicalEndY / step) * step + indent;
    updateObstacles();

    createGraph();
    repaint();
}

//удалить все препятствия
void MainWindow::on_action_10_triggered()
{
    obstacles.clear();
    classicalObstacles.clear();
    int s1 = (maxX - indent) / step;
    int s = (maxY - indent) / step;
    delete obstaclesAreas;
    obstaclesAreas = new bool*[s];
    for(int i = 0; i < s; i++){
        obstaclesAreas[i] = new bool[s1];
        for(int j = 0; j < s1; j++){
            obstaclesAreas[i][j] = false;
        }
    }

    int p = (r / step) * step;
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
    createGraph();

    /*
    pair<int, int> startCell = pair((startX - indent) / step, (startY - indent) / step);
    pair<int, int> endCell = pair((endX - indent) / step, (endY - indent) / step);
    fullWay = area.deikstrasAlgo(startCell.first, startCell.second, endCell.first, endCell.second);
    */
    repaint();
}

//удалить последнее добавленное препятствие
void MainWindow::on_action_11_triggered()
{
    if(obstacles.empty()) return;
    obstacles.pop_back();
    classicalObstacles.pop_back();
    updateObstacles();
    createGraph();
    repaint();
}

//Сохранить как...
void MainWindow::on_action_6_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Открыть файл"),
                                                    "",
                                                    tr("Ini-files (*.ini)"));
    if(fileName.isEmpty()) return;
    QSettings sett(fileName, QSettings::IniFormat);
    sett.setValue("AREA/STEP", step);
    sett.setValue("ROBOT/RADIUS", r);
    sett.setValue("ROBOT/STARTX", classicalStartX);
    sett.setValue("ROBOT/STARTY", classicalStartY);
    sett.setValue("ROBOT/ENDX", classicalEndX);
    sett.setValue("ROBOT/ENDY", classicalEndY);
    sett.remove("OBSTACLES");
    QString value;
    sett.setValue("OBSTACLES/AMOUNT", classicalObstacles.size());
    for(int i = 0; i < classicalObstacles.size(); i++){
        value = "OBSTACLES/OBSX" + QString::number(i);
        sett.setValue(value, classicalObstacles[i].x);
        value = "OBSTACLES/OBSY" + QString::number(i);
        sett.setValue(value, classicalObstacles[i].y);
        value = "OBSTACLES/OBSW" + QString::number(i);
        sett.setValue(value, classicalObstacles[i].w);
        value = "OBSTACLES/OBSH" + QString::number(i);
        sett.setValue(value, classicalObstacles[i].h);
    }
}


void MainWindow::on_action_7_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"),
                                                    "",
                                                    tr("Ini-files (*.ini)"));

    if(fileName.isEmpty()) return;
    QSettings sett(fileName, QSettings::IniFormat);
    step = sett.value("AREA/STEP", "").toInt();
    r = sett.value("ROBOT/RADIUS", "").toInt();
    classicalStartX = sett.value("ROBOT/STARTX", "").toInt();
    classicalStartY = sett.value("ROBOT/STARTY", "").toInt();
    classicalEndX = sett.value("ROBOT/ENDX", "").toInt();
    classicalEndY = sett.value("ROBOT/ENDY", "").toInt();

    startX = (classicalStartX / step) * step + indent;
    startY = (classicalStartY / step) * step + indent;
    endX = (classicalEndX / step) * step + indent;
    endY = (classicalEndY / step) * step + indent;

    classicalObstacles.clear();
    int n = sett.value("OBSTACLES/AMOUNT", "").toInt();
    obstacle buff;
    QString value;
    for(int i = 0; i < n; i++){
        value = "OBSTACLES/OBSX" + QString::number(i);
        buff.x = sett.value(value, "").toInt();
        value = "OBSTACLES/OBSY" + QString::number(i);
        buff.y = sett.value(value, "").toInt();
        value = "OBSTACLES/OBSW" + QString::number(i);
        buff.w = sett.value(value, "").toInt();
        value = "OBSTACLES/OBSH" + QString::number(i);
        buff.h = sett.value(value, "").toInt();
        classicalObstacles.push_back(buff);
    }
    updateObstacles();
    createGraph();
    repaint();
}
