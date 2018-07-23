#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    for (int i = 1; i < argc; ++i)
    {
        QString current(argv[i]);
        QFile file(current);
        if (file.exists())
            w.loadFile(current);
    }

    w.show();

    return a.exec();
}
