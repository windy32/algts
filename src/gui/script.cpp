#include "script.h"

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
    out << script.text << script.params;
    return out;
}

QDataStream &operator>>(QDataStream &in, Script &script)
{
    in >> script.text >> script.params;
    return in;
}

