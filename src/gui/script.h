#ifndef SCRIPT_H
#define SCRIPT_H

#include "../core/core.h"

struct ScriptParam
{
    qint32 line;
    qint32 pos;
    QString defValue;
};

QDataStream &operator<<(QDataStream &out, const ScriptParam &param);
QDataStream &operator>>(QDataStream &in, ScriptParam &param);

struct Script
{
    QString text;
    QList<ScriptParam> params;
};

QDataStream &operator<<(QDataStream &out, const Script &script);
QDataStream &operator>>(QDataStream &in, Script &script);

#endif // SCRIPT_H

