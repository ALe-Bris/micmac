#include "mainwindow.h"

#include <iostream>
#include <QFile>
#include <QMessageBox>
#include <QTabWidget>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QAbstractTextDocumentLayout>


MainWindow::MainWindow(const QString &file, const QString &command, QWidget *parent)
    : QWidget(parent)
    , initCompleted(false)
{
/*
    QFile file("style.qss");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
//        Q_INIT_RESOURCE(icones);

        qApp->setStyleSheet(file.readAll());
        file.close();
    }
    else
        QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("Can't find qss file"));
*/
    QString errorMsg;
    if (! specs.fromJson(file,command,errorMsg)) {
        QMessageBox::critical(NULL, QObject::tr("Error"), errorMsg);
        std::cerr << "Error: " << qPrintable(errorMsg) << "\n";
        return;
    }
    setWindowTitle(specs.name + " - " + specs.comment);

    buildUi();
    initCompleted = true;
}

MainWindow::~MainWindow()
{
}

enum Type {T_UNKNOWN, T_BOOL, T_INT, T_DOUBLE, T_STRING, T_VEC_DOUBLE, T_VEC_INT, T_VEC_STRING, T_PTXD2_INT, T_PTXD2_DOUBLE, T_BOX2_INT, T_ENUM};

std::map<QString, Type> typeNameEnumMap = {
    {"bool",T_BOOL},
    {"int", T_INT},
    {"double",T_DOUBLE},
    {"string",T_STRING},
    {"std::vector<int>",T_VEC_INT},
    {"std::vector<double>",T_VEC_DOUBLE},
    {"std::vector<std::string>",T_VEC_STRING},
    {"cPtxd<int,2>",T_PTXD2_INT},
    {"cPtxd<double,2>",T_PTXD2_DOUBLE},
    {"cTplBox<int,2>",T_BOX2_INT}
};

static Type typeNameToEnum(const ArgSpec& as)
{
    auto it = typeNameEnumMap.find(as.type);
    if (it != typeNameEnumMap.end())
        return it->second;
    if (as.allowed.size() != 0)
        return T_ENUM;
    return T_UNKNOWN;
}


InputWidget *MainWindow::createInput(QGridLayout *layout, ArgSpec& as)
{
    Type type = typeNameToEnum(as);
    switch (type) {
    case T_ENUM:
    case T_BOOL:
        return new InputEnum(layout, as);
    case T_INT:
        return new InputIntN(layout, as);
    case T_DOUBLE:
        return new InputDoubleN(layout, as);
    case T_PTXD2_INT:
        return new InputIntN(layout, as,2);
    case T_PTXD2_DOUBLE:
        return new InputDoubleN(layout, as,2);
    case T_BOX2_INT:
        return new InputIntN(layout, as,4);
    case T_VEC_INT:
        return new InputIntN(layout, as);
    case T_VEC_DOUBLE:
        return new InputDoubleN(layout, as);
    case T_STRING:
        if (as.semantic.contains("Im"))
            return new InputFile(layout, as, InputFile::IM);
        if (as.semantic.contains("DP"))
            return new InputFile(layout, as, InputFile::DP);
        if (as.semantic.contains("FDP"))
            return new InputFile(layout, as, InputFile::FDP);
        if (as.semantic.contains("Cloud"))
            return new InputFile(layout, as, InputFile::IM);
        if (as.semantic.contains("3DReg"))
            return new InputFile(layout, as, InputFile::REG3D);
        if (as.semantic.contains("MPF"))
            return new InputFile(layout, as, InputFile::MPF);
        return new InputString(layout, as);
    default:
        // TODO: warning message
        return new InputString(layout, as);
    }
}


QWidget *MainWindow::createPage(QVector<ArgSpec>& argSpecs, const QString& level)
{
    QWidget *widget = new QWidget;
    QGridLayout *layout= new QGridLayout(widget);

    layout->setColumnStretch(0,0);
    layout->setColumnStretch(1,1);
    layout->setColumnStretch(2,1);
    layout->setColumnStretch(3,1);
    layout->setColumnStretch(4,0);

    for (auto& as : argSpecs) {
        if (level != as.level)
            continue;
        auto input = createInput(layout, as);
        connect(input, &InputWidget::valueChanged, this, &MainWindow::valueUpdated);
    }
    int nb3=0;
    int nb2=0;
    for (int i=0; i<layout->rowCount(); i++) {
        if (layout->itemAtPosition(i,3))
            nb3++;
        if (layout->itemAtPosition(i,2))
            nb2++;
    }
    layout->setColumnStretch(0,0);
    layout->setColumnStretch(1,1);
    layout->setColumnStretch(2,nb2==0 ? 0 : 1);
    layout->setColumnStretch(3,nb3==0 ? 0 : 1);
    layout->setColumnStretch(4,0);
    layout->setRowStretch(layout->rowCount(),1);
    if (layout->count() == 0) {
        delete widget;
        widget = nullptr;
    }
    return widget;
}


void MainWindow::buildUi()
{
    QVBoxLayout *vLayout = new QVBoxLayout(this);

    teCommand = new QTextEdit(this);
    teCommand->setReadOnly(true);
    QFontMetrics fm(teCommand->font());
    QMargins margins = teCommand->contentsMargins();
    teCommand->setMinimumHeight(fm.height() * 3 + margins.top() + margins.bottom() + 8);

    QPushButton *pbReset = new QPushButton(tr("Reset"),this);
    QPushButton *pbCancel = new QPushButton(tr("Cancel"),this);
    pbRun = new QPushButton(tr("Run"),this);
    connect(pbReset,&QPushButton::clicked,this,&MainWindow::resetValues);
    connect(pbCancel,&QPushButton::clicked,this,&MainWindow::close);
    connect(pbRun,&QPushButton::clicked,this,&MainWindow::runCommand);
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(pbReset);
    hLayout->addStretch(1);
    hLayout->addWidget(pbCancel);
    hLayout->addWidget(pbRun);

    QTabWidget *tabWidget = new QTabWidget(this);
    QWidget *wMandatory = createPage(specs.mandatories, "");
    QWidget *wOptional  = createPage(specs.optionals, "normal");
    QWidget *wTuning    = createPage(specs.optionals, "tuning");
    QWidget *wGlobal    = createPage(specs.optionals, "global");

    if (wMandatory)
        tabWidget->addTab(wMandatory,tr("Mandatory"));
    if (wOptional)
        tabWidget->addTab(wOptional,tr("Optional"));
    if (wTuning)
        tabWidget->addTab(wTuning,tr("Tuning"));
    if (wGlobal)
        tabWidget->addTab(wGlobal,tr("Global"));

    vLayout->addWidget(tabWidget,0);
    vLayout->addWidget(teCommand,1);
    vLayout->addLayout(hLayout,0);
    resize(width(),minimumHeight());
    checkAllMandatories();
    updateCommand();
}

void MainWindow::resetValues()
{
    InputWidget::resetAll();
}

bool MainWindow::initOk()
{
    return initCompleted;
}

void MainWindow::checkAllMandatories()
{
    bool allNonEmpty = true;
    for (const auto& as : qAsConst(specs.mandatories)) {  // avois QVector detachment
        if (as.value.isEmpty()) {
            allNonEmpty = false;
            break;
        }
    }
    pbRun->setEnabled(allNonEmpty);
}


QString MainWindow::quoteString(const QString& s)
{
    if (s.isEmpty())
        return "\"\"";
            
    QString quote = "";
    for (const auto& c : s) {
        if (! c.isLetterOrNumber() && c != '_' && c!= '-' && c != '/' && c!= ':' && c != '.' && c!= '=') {
            quote = "\"";
            break;
        }
    }
    QString ret = quote;
    for (const auto& c : s) {
        if (c == '\\' || c == '$' || c == '"')
            ret += "\\";
        ret += c;
    }
    ret += quote;
    return ret;
}

void MainWindow::buildCommand()
{
    command = "MMVII";
    command += " " + specs.name;
    for (const auto& as : qAsConst(specs.mandatories)) 
        command += " " + quoteString(as.value);

    for (const auto& as : qAsConst(specs.optionals)) {
        if (as.isEnabled)
            command += " " + quoteString(as.name + "=" + as.value);
    }
}

void MainWindow::updateCommand()
{
    buildCommand();
    teCommand->setText(command);
}

void MainWindow::valueUpdated(const ArgSpec &as)
{
    if (as.mandatory)
        this->checkAllMandatories();
    this->updateCommand();
}

void MainWindow::runCommand()
{
    std::cout << qPrintable(command) << "\n";
    this->close();
}

