#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include <QLabel>
#include <QSettings>
#include <QVariant>
#include <vector>
#include <QPainter>
#include <windows.h>
#include "graph.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


const int indent = 20;
const int maxX = 820;
const int maxY = 620;

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    int step = 20;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_2_triggered();

    void on_action_3_triggered();

    void on_action_triggered();

    void createGraph();

    void on_action_8_triggered();

    void on_action_9_triggered();

    void on_action_4_triggered();

    pair<int, int> coords(pair<int, int> value);

    void updateObstacles();

    void on_action_5_triggered();

    void on_action_10_triggered();

    void on_action_11_triggered();

    void on_action_6_triggered();

    void on_action_7_triggered();

private:

    struct obstacle{
        int x;
        int y;
        int w;
        int h;
    };
    struct cord{
        int x;
        int y;
    };
    Ui::MainWindow *ui;
    vector<obstacle> obstacles; //препятствия
    vector<obstacle> classicalObstacles;

    bool** obstaclesAreas;
    int startX = -1, startY = -1,
        endX = -1, endY = -1;
    int classicalStartX = -1, classicalStartY = -1,
        classicalEndX = -1, classicalEndY = -1;
    vector<cord> way;
    vector<Graph::vertex> fullWay;
    Graph area;
    bool standart = false;
    int r = 5; //радиус робота
};
#endif // MAINWINDOW_H
