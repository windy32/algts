#include "script.h"

ScriptParam::ScriptParam()
{
    this->line = 0;
    this->pos = 0;
    this->defValue = "";
}

ScriptParam::ScriptParam(qint32 line, qint32 pos, const QString &defValue)
{
    this->line = line;
    this->pos = pos;
    this->defValue = defValue;
}

QDataStream &operator<<(QDataStream &out, const ScriptParam &param)
{
    out << param.line << param.pos << param.defValue;
    return out;
}

QDataStream &operator>>(QDataStream &in, ScriptParam &param)
{
    in >> param.line >> param.pos >> param.defValue;
    return in;
}

QDataStream &operator<<(QDataStream &out, const Script &script)
{
    out << script.setupText << script.setupParams
        << script.resetText << script.resetParams << script.name;
    return out;
}

QDataStream &operator>>(QDataStream &in, Script &script)
{
    in >> script.setupText >> script.setupParams
       >> script.resetText >> script.resetParams >> script.name;
    return in;
}
