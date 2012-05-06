#include "script.h"

QDataStream &operator<<(QDataStream &out, const Script::Param &param)
{
    out << param.line << param.pos << param.defValue;
    return out;
}

QDataStream &operator>>(QDataStream &in, Script::Param &param)
{
    in >> param.line >> param.pos >> param.defValue;
    return in;
}

Script::Script()
{
}

void Script::serialize(QDataStream &stream)
{
    if( stream.device()->openMode() == QIODevice::ReadOnly )
    {
        stream >> m_script >> m_params;
    }
    else if( stream.device()->openMode() == QIODevice::WriteOnly )
    {
        stream << m_script << m_params;
    }
}
