#ifndef COMMANDSPEC_H
#define COMMANDSPEC_H

#include <functional>
#include <QString>
#include <QObject>
#include <QVector>

struct ArgSpec
{
    ArgSpec(bool mandatory=true) : mandatory(mandatory) {}

    bool mandatory;
    bool isEnabled;
    int number;
    QString name;
    QString level;
    QString type;
    QString def;
    QString comment;
    QStringList semantic;
    QStringList allowed;
    QString range;
    QString vsize;

    QString value;
};


struct CommandSpec : public QObject
{
    Q_OBJECT
public:
    CommandSpec() {}

    bool fromJson(const QString& file, const QString& command, QString& errorMsg);

    QString name;
    QString comment;
    QString source;

    QVector<ArgSpec> mandatories;
    QVector<ArgSpec> optionals;

private:
    bool toString(const QJsonObject& obj, const QString& key, QString& val, QString& errorMsg, const QString& context, bool needed=true);
    bool toStringList(const QJsonObject &obj, const QString &key, QStringList &val, QString &errorMsg, const QString &context, bool needed);
    bool parseArgsSpecs(const QJsonObject& argsSpecs, QVector<ArgSpec> &vSpecs, const QString& key, QString& errorMsg);
};

#endif // COMMANDSPEC_H
