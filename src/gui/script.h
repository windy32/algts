#ifndef SCRIPT_H
#define SCRIPT_H

#include "../core/core.h"

class Script : public Serializable
{
public:
    struct Param
    {
        qint32 line;
        qint32 pos;
        QString defValue;
    };

private:
    QString m_script;
    QList<Param> m_params;

public:
    Script();

    virtual void serialize(QDataStream &stream);
};

#endif // SCRIPT_H
