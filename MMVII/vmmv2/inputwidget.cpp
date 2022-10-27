#include "inputwidget.h"
#include <QFileDialog>
#include <QMessageBox>

QVector<InputWidget *> InputWidget::allInputs;

void InputWidget::resetAll()
{
    for (auto& w : allInputs)
        w->reset();
}


/*********************************************************************************/
InputWidget::InputWidget(QGridLayout *layout, ArgSpec &as)
    : layout(layout),as(as),enabledWidget(nullptr)
{
    curRow = layout->rowCount();
    curCol = 1;
    label = new QLabel();
    if (as.mandatory) {
        if (as.comment.isEmpty())
            label->setText(tr("Argument #%1 :").arg(as.number));
        else
            label->setText(as.comment + " :");
    } else {
        label->setText(as.name+ " :");
        label->setToolTip(as.comment);
    }
    layout->addWidget(label,curRow,0,Qt::AlignRight);

    if (! as.mandatory) {
        enabledWidget = new QCheckBox();
        enabledWidget->setChecked(false);
        layout->addWidget(enabledWidget,curRow,4,Qt::AlignLeft);
        layout->setColumnStretch(4,0);
        connect(enabledWidget,&QCheckBox::toggled,this,&InputWidget::enableChanged);
    }

    allInputs.push_back(this);
}

void InputWidget::addWidget(QWidget *w, int span)
{
    w->setParent(this);
    layout->addWidget(w,curRow,curCol,1,span);
    curCol += span;
    if (curCol == 4) {
        curRow++;
        curCol=1;
    }
}

void InputWidget::valueEdited(const QString &val)
{
    as.value = val;
    if (enabledWidget)
        enabledWidget->setChecked(!val.isEmpty() && val!= DEFAULT_VALUE_MARKER);
    emit valueChanged(as);
}

void InputWidget::enableChanged(bool checked)
{
    if (checked && (as.value.isEmpty() || as.value == DEFAULT_VALUE_MARKER)) {
        enabledWidget->setChecked(false);
        return;
    }
    as.isEnabled = checked;
    emit valueChanged(as);
}

void InputWidget::finish()
{
    reset();
}

QStringList InputWidget::parseList(const QString &lv)
{
    if (lv.size()>=2 && lv.front() == '['  && lv.back() == ']')
        return lv.mid(1,lv.size()-2).split(',');
    else
        return lv.split(',');
}

void InputWidget::reset()
{
    doReset();
    if (enabledWidget)
        enabledWidget->setChecked(false);
    qDebug("%s %d %d %d",qPrintable(as.name),minimumWidth(),minimumSizeHint().width(),width());
}

/*********************************************************************************/
InputEnum::InputEnum(QGridLayout *layout, ArgSpec &as) : InputWidget(layout,as)
{
    cb=new QComboBox();
    addWidget(cb,2);
    cb->show();
    if (! as.mandatory)
        cb->addItem(DEFAULT_VALUE_MARKER);
    if (as.type == "bool")
        cb->addItems({"false","true"});
    else
        cb->addItems(as.allowed);

    connect(cb,&QComboBox::currentTextChanged,this,[this](const QString& text) {this->valueEdited(text);});
    finish();
}

void InputEnum::doReset()
{
    if (as.def.isEmpty())
        cb->setCurrentIndex(0);
    else
        cb->setCurrentText(as.def);
    as.value = cb->currentText();
}

/*********************************************************************************/
InputString::InputString(QGridLayout *layout, ArgSpec &as) : InputWidget(layout,as)
{
    le = new QLineEdit();
    addWidget(le,2);
    connect(le,&QLineEdit::textChanged,this,[this](const QString& val) {this->valueEdited(val);});
    finish();
}

void InputString::doReset()
{
    le->setText(as.def);
}


/*********************************************************************************/

QString InputFile::curDir;

InputFile::InputFile(QGridLayout *layout, ArgSpec &as, Type type) : InputWidget(layout,as)
{
    le = new QLineEdit();
    addWidget(le,2);
    pb = new QPushButton(tr("Browse"));
    addWidget(pb,1);
    connect(le,&QLineEdit::textChanged,this,[this](const QString& val) {this->valueEdited(val);});
    connect(pb,&QPushButton::clicked,this,&InputFile::fileDialog);
    switch (type) {
    case DP:
        mode    = OPEN_DIR;
        caption = tr("Select a directory project");
        pb->setText(tr("Select Dir"));
        filter  = "";
        break;
    case FDP:
        mode    = OPEN_FILE_DIR;
        caption = tr("Select a file or directory project");
        pb->setText(tr("Select Dir"));
        filter  = "";
        break;
    case IM:
        mode    = OPEN_FILE;
        pb->setText(tr("Select File"));
        caption = tr("Select an Image file");
        filter  = "Images ("
                  "*.tif *.tiff *.jpg *.jpeg *.png *.cr2 *.crw *.nef "
                  "*.TIF *.TIFF *.JPG *.JPEG *.PNG *.CR2 *.CRW *.NEF "
                  ");;All(*)";
        break;
    case CLOUD:
        mode    = OPEN_FILE;
        pb->setText(tr("Select File"));
        caption = tr("Select a cloud file");
        filter  = ".ply (*.ply);;All(*)";
        break;
    case REG3D:
        mode    = OPEN_FILE;
        caption = tr("Select a 3D region file");
        pb->setText(tr("Select File"));
        filter  = "All(*)";
        break;
    case MPF:
        // FIXME: Is it correct ? (several filenames)
        mode    = OPEN_FILES;
        pb->setText(tr("Select Files"));
        caption = tr("Select files");
        filter  = "All(*)";
        break;
    default:
        mode    = OPEN_FILE;
        caption = tr("Select a file");
        filter  = "All(*)";
        break;
    }

    finish();
}

void InputFile::doReset()
{
    le->setText(as.def);
}

void InputFile::fileDialog()
{
    QString fileName,dirName;
    QStringList fileNames;
    
    switch (mode) {
    case OPEN_FILE: 
        fileName = QFileDialog::getOpenFileName(this,caption,curDir,filter);
        if (fileName.isEmpty())
            return;
        curDir = QFileInfo(fileName).absolutePath();
        fileName = QDir().relativeFilePath(fileName);
        le->setText(fileName);
        break;
    case OPEN_FILES:
        fileNames = QFileDialog::getOpenFileNames(this,caption,curDir,filter);
        if (fileNames.isEmpty())
            return;
        curDir = QFileInfo(fileNames.last()).absolutePath();
        // FIXME: Is it correct ? How to specify several files ?
        for (auto& fileName : fileNames)
            fileName = QDir().relativeFilePath(fileName);
        le->setText("[" + fileNames.join(",") + "]");
        break;
    case OPEN_DIR: 
        dirName = QFileDialog::getExistingDirectory(this,caption,curDir);
        if (dirName.isEmpty())
            return;
        dirName = QDir().relativeFilePath(dirName);
        le->setText(dirName);
        break;
    default:
        QMessageBox::warning(this,"Unimplemented","Unimplemented (yet!) for this semantic");
        break;
    }
}


