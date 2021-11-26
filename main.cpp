
#include "mainwindow.h"
//#include "line.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setGeometry(250, 50, 800, 600);
    w.show();
    return a.exec();
}

/*
#include <QApplication>
#include "line.h"

int main(int argc, char *argv[]) {

  QApplication app(argc, argv);

  Lines window;

  //window.resize(280, 270);
  //window.setWindowTitle("Lines");
  window.show();

  //return app.exec();
}
*/
