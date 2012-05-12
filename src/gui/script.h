#ifndef SCRIPT_H
#define SCRIPT_H

#include "../core/core.h"

struct ScriptParam
{
    qint32 line;
    qint32 pos;
    QString defValue;

    ScriptParam();
    ScriptParam(qint32 line, qint32 pos, const QString &defValue);
};

QDataStream &operator<<(QDataStream &out, const ScriptParam &param);
QDataStream &operator>>(QDataStream &in, ScriptParam &param);

struct Script // Template
{
    QString name;

    QString setupText;
    QList<ScriptParam> setupParams;

    QString resetText;
    QList<ScriptParam> resetParams;
};

QDataStream &operator<<(QDataStream &out, const Script &script);
QDataStream &operator>>(QDataStream &in, Script &script);

struct ScriptData
{
    QString name;
    QString description;
    QString setupText;
    QString resetText;
};

QDataStream &operator<<(QDataStream &out, const ScriptData &script);
QDataStream &operator>>(QDataStream &in, ScriptData &script);

#endif // SCRIPT_H

