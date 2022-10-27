#ifndef INPUTWIDGET_H
#define INPUTWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include "commandspec.h"
#include "spinboxdefault.h"


// TODO: std::vector<std::string>


// FIXME: setLabel
class InputWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InputWidget(QGridLayout *layout, ArgSpec& as);
    void reset();
    static void resetAll();

signals:
    void valueChanged(const ArgSpec &);

protected:
    void addWidget(QWidget* w, int span = 1);
    void valueEdited(const QString &val);
    void enableChanged(bool checked);
    void finish();
    QStringList parseList(const QString& lv);

    virtual void doReset() = 0;

    QGridLayout *layout;
    ArgSpec& as;
    QCheckBox *enabledWidget;
    QLabel *label;
    int curRow;
    int curCol;
    static QVector<InputWidget *> allInputs;
};


class InputEnum : public InputWidget
{
public:
    InputEnum(QGridLayout *layout, ArgSpec &as);
    virtual void doReset() override;
private:
    QComboBox *cb;
};


class InputString: public InputWidget
{
public:
    InputString(QGridLayout *layout, ArgSpec &as);
    virtual void doReset() override;
private:
    QLineEdit *le;
};


class InputFile: public InputWidget
{
public:
    enum Type {DP, FDP, IM, CLOUD, REG3D, MPF, OTHER};
    InputFile(QGridLayout *layout, ArgSpec &as, Type type);
    virtual void doReset() override;
private:
    void fileDialog();
    QLineEdit *le;
    QPushButton *pb;
    
    QString filter,caption;
    enum Mode {OPEN_FILE, OPEN_FILES, OPEN_FILE_DIR, OPEN_DIR, CREATE_FILE};
    Mode mode;
    
    static QString curDir;
};

template<typename T, class SPIN>
class InputNumbers : public InputWidget
{
public:
    InputNumbers(QGridLayout *layout, ArgSpec &as,int n=1);
    virtual void doReset() override;
private:
    void valueEdited(const QString& val, int n);
    int minV, maxV;
    QVector<SPIN *> sbs;
};

typedef InputNumbers<double, DoubleSpinBoxDefault> InputDoubleN;
typedef InputNumbers<int, SpinBoxDefault> InputIntN;



/*********************************************************************************/


template<typename T, class SPIN>
InputNumbers<T,SPIN>::InputNumbers(QGridLayout *layout, ArgSpec &as, int n)
    : InputWidget(layout,as)
{
    if (as.vsize.size()) {
        QStringList vsize = parseList(as.vsize);
        if (vsize.size() == 2) {
            bool ok1,ok2;
            minV = vsize[0].toInt(&ok1);
            maxV = vsize[1].toInt(&ok2);
            if (ok1 && !ok2)
                maxV = minV;
            else if (!ok1 && ok2)
                minV = maxV;
            else if (!ok1 && !ok2)
                minV = maxV = 1;
        } else {
            minV = maxV = 1;
        }
    } else {
        minV = maxV = n;
    }

    for (int i=0; i<maxV; i++) {
        SPIN *sb = new SPIN();

        T max = std::numeric_limits<T>::max();
        T min = std::numeric_limits<T>::min();

        QDoubleSpinBox *dsb=dynamic_cast<QDoubleSpinBox*>(sb);
        if (dsb) {
            min = -max;
            dsb->setDecimals(3);
        }
        QStringList range = parseList(as.range);

        if (range.size() > 0 && range[0].size())
            min = qvariant_cast<T>(range[0]);
        if (range.size() > 1 && range[1].size())
            max= qvariant_cast<T>(range[1]);
        sb->setRange(min, max);
        if (as.def.size())
            sb->setDefaultValue(qvariant_cast<T>(as.def));

        connect(sb,qOverload<const QString&>(&SPIN::valueChanged),this,[this,i](const QString& val) {this->valueEdited(val,i);});
        addWidget(sb,1);
        sbs.push_back(sb);
        if (maxV == 4 && i==1) {
            curCol = 1;
            curRow ++;
        }
    }
    finish();
}


template<typename T, class SPIN>
void InputNumbers<T,SPIN>::doReset()
{
    QStringList values = parseList(as.def);
    for (int i=0; i<maxV; i++) {
        if (i<values.size() && !values[i].isEmpty())
            sbs[i]->setValue(qvariant_cast<T>(values[i]));
        else
            sbs[i]->setValue(sbs[i]->minimum());
    }
}

template<typename T, class SPIN>
void InputNumbers<T,SPIN>::valueEdited(const QString &val, int n)
{
    if (maxV == 1) {
        InputWidget::valueEdited(val);
        return;
    }
    int nbValued=0;
    for (int i=0; i<maxV; i++) {
        if (sbs[i]->text().isEmpty() || sbs[i]->text() == DEFAULT_VALUE_MARKER)
            break;
        nbValued++;
    }
    if (nbValued < minV) {
        InputWidget::valueEdited(DEFAULT_VALUE_MARKER);
        return;
    }
    for (int i=nbValued; i<maxV; i++)
        sbs[i]->setValue(sbs[i]->minimum());

    QString s = "[";
    for (int i=0; i<nbValued; i++) {
        if (i != 0)
            s += ",";
        if (i == n)
            s += val;
        else
            s += sbs[i]->text();
    }
    s += "]";
    InputWidget::valueEdited(s);
}

#endif // INPUTWIDGET_H
