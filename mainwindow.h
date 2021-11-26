#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include <QLabel>
#include <vector>
#include <QPainter>
#include <windows.h>
#include "graph.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

const int maxX = 820;
const int maxY = 620;
const int step = 10;
const int indent = 20;

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    struct obstacle{
        int x;
        int y;
        int w;
        int h;
    };
    Ui::MainWindow *ui;
    std::vector<obstacle> obstacles; //препятствия
    bool** obstaclesAreas;
    int startX = indent, startY = maxY - step,
        endX = maxX - step, endY = indent;

    int r = 1; //радиус робота
};
#endif // MAINWINDOW_H
