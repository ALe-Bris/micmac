#include "mainwindow.h"
#include <iostream>
#include <QApplication>

// TODO: if file, is it always specified in semantic ?
// TODO: several semantics ?
// TODO: output file or dir ?
// TODO: std::vector with no/infinite size ? => warning,error ?
// TODO: get precise semantic for semantics :-)
// TODO: FFI ???

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString file, command;

    QLocale::setDefault(QLocale::C);
    switch (argc) {
    case 1:
        break;
    case 2:
        if (QString(argv[1]) != "-")
            file = argv[1];
        break;
    case 3:
        if (QString(argv[1]) != "-")
            file = argv[1];
        command = argv[2];
        break;
    default:
        std::cerr << "Error: too many arguments\n";
        return EXIT_FAILURE;
    }

    MainWindow w(file,command);
    if (! w.initOk())
        return EXIT_FAILURE;
    w.show();
    return a.exec();
}
