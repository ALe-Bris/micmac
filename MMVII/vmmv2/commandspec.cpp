#include "commandspec.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>


bool CommandSpec::toStringList(const QJsonObject& obj, const QString& key, QStringList& val, QString& errorMsg, const QString& context, bool needed)
{
    val.clear();
    if (!obj.contains(key)) {
        if (!needed)
            return true;
        errorMsg = tr("Missing key '%1' for %2").arg(key, context);
        return false;
    }
    if (!obj[key].isArray()) {
        errorMsg = tr("Key '%1' is not an array of string for %2").arg(key, context);
        return false;
    }
    const QJsonArray a = obj[key].toArray();
    for (const auto& v : a) {
        if (! v.isString()) {
            errorMsg = tr("Key '%1' is not an array of string for %2").arg(key, context);
            return false;
        }
        val.push_back(v.toString());
    }
    return true;
}

bool CommandSpec::toString(const QJsonObject& obj, const QString& key, QString& val, QString& errorMsg, const QString& context, bool needed)
{
    val="";
    if (!obj.contains(key)) {
        if (!needed)
            return true;
        errorMsg = tr("Missing key '%1' for %2").arg(key, context);
        return false;
    }
    if (!obj[key].isString()) {
        errorMsg = tr("Key '%1' is not a string for %2").arg(key, context);
        return false;
    }
    val = obj[key].toString();
    return true;
}


bool CommandSpec::parseArgsSpecs(const QJsonObject& argsSpecs, QVector<ArgSpec>& vSpecs, const QString& key, QString& errorMsg)
{
    vSpecs.clear();
    if (! argsSpecs.contains(key)) {
        errorMsg = tr("Missing key '%1' in command %2").arg(key, name);
        return false;
    }
    if (! argsSpecs[key].isArray()) {
        errorMsg = tr("Key '%1' must be an array in command %2").arg(key, name);
        return false;
    }
    const QJsonArray specsArray = argsSpecs[key].toArray();
    int n=0;
    for (const auto& s : specsArray) {
        n++;
        if (!s.isObject())  {
            errorMsg = tr("%1 element #%2 is not a JSON object in command %3").arg(key).arg(n).arg(name);
            return false;
        }
        QJsonObject spec = s.toObject();
        ArgSpec as(key == "mandatory");
        as.isEnabled = as.mandatory;
        as.number = n;
        QString context = tr("%1 argument #%2 in command %3").arg(key).arg(n).arg(name);
        if (!toString(spec,"name",as.name,errorMsg,context,key != "mandatory"))
            return false;
        if (!toString(spec,"level",as.level,errorMsg,context,false))
            return false;
        if (!toString(spec,"type",as.type,errorMsg,context))
            return false;
        if (!toString(spec,"default",as.def,errorMsg,context,false))
            return false;
        if (!toString(spec,"comment",as.comment,errorMsg,context,false))
            return false;
        if (!toStringList(spec,"semantic",as.semantic,errorMsg,context,false))
            return false;
        if (!toStringList(spec,"allowed",as.allowed,errorMsg,context,false))
            return false;
        if (!toString(spec,"range",as.range,errorMsg,context,false))
            return false;
        if (!toString(spec,"vsize",as.vsize,errorMsg,context,false))
            return false;
        vSpecs.push_back(as);
    }
    return true;
}


bool CommandSpec::fromJson(const QString& file, const QString& command, QString& errorMsg)
{
    QByteArray specsTxt;
    QFile in;

    if (file.isEmpty() || file == "-") {
        in.open(stdin, QIODevice::ReadOnly);
        specsTxt = in.readAll();
    } else {
        in.setFileName(file);
        if (! in.open(QIODevice::ReadOnly)) {
            errorMsg = tr("Can't read '%1' specifications file").arg(file);
            return false;
        }
        specsTxt = in.readAll();
        in.close();
    }
    if (specsTxt.isNull()) {
        errorMsg = tr("Empty JSON specifications");
        return false;
    }

    QJsonParseError error;
    QJsonDocument jDoc = QJsonDocument::fromJson(specsTxt,&error);

    if (error.error != QJsonParseError::NoError) {
        errorMsg = tr("Error parsing JSON specifications : \n%1 at %2").arg(error.errorString()).arg(error.offset);
        return false;
    }
    if (jDoc.isNull()) {
        errorMsg = tr("Empty JSON specifications");
        return false;
    }

    QJsonObject theSpec;
    if (! command.isEmpty()) {
        if (! jDoc.isArray()) {
            errorMsg = tr("Expecting a full JSON specifications (for all commands)");
            return false;
        }
        const auto cmdArray = jDoc.array();
        for (const auto& argSpec : cmdArray) {
            if (! argSpec.isObject()) {
                errorMsg = tr("Invalid JSON specification");
                return false;
            }
            auto jArgSpec = argSpec.toObject();
            if (! jArgSpec["name"].isString()) {
                errorMsg = tr("Invalid JSON specification");
                return false;
            }
            if (jArgSpec["name"].toString() == command) {
                theSpec = jArgSpec;
                break;
            }
        }
        if (theSpec.isEmpty()) {
            errorMsg = tr("No specification for command '%1'").arg(command);
            return false;
        }
    } else {
        if (! jDoc.isObject()) {
            errorMsg = tr("Expecting a unique command JSON specification");
            return false;
        }
        theSpec = jDoc.object();
    }

    QString context = tr("command specification");
    if (! toString(theSpec,"name",name,errorMsg,context))
        return false;
    context += " " + name;
    if (! toString(theSpec,"comment",comment,errorMsg,context))
        return false;
    if (! toString(theSpec,"source",source,errorMsg,context,false))
        return false;

    if (!parseArgsSpecs(theSpec, mandatories, "mandatory", errorMsg))
        return false;
    if (!parseArgsSpecs(theSpec, optionals, "optional", errorMsg))
        return false;
    return true;
}
