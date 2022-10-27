#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include "inputwidget.h"
#include "commandspec.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(const QString& file, const QString& command, QWidget *parent = nullptr);
    ~MainWindow();

    bool initOk();

private:
    enum Level {Mandatory, Normal, Global, Tuning};
    bool initCompleted;
    QString command;
    CommandSpec specs;
    QPushButton *pbRun;
    QTextEdit *teCommand;

    void buildUi();
    QWidget *createPage(QVector<ArgSpec> &argSpecs, const QString& level);
    InputWidget *createInput(QGridLayout *layout, ArgSpec& as);
    QWidget *inputEnum(ArgSpec& as);
    QWidget *inputInt(ArgSpec &as);
    QWidget *inputDouble(ArgSpec &as);
    QWidget *inputString(ArgSpec &as);


    QString quoteString(const QString &s);
    void resetValues();
    void checkAllMandatories();
    void buildCommand();
    void runCommand();
    void updateCommand();
    void valueUpdated(const ArgSpec& as);
};
#endif // MAINWINDOW_H
