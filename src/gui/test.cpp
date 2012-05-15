#include "test.h"

QDataStream &operator<<(QDataStream &out, const TestResult &result)
{
    out << result.scenario << result.script
        << result.time << result.platform << result.detail;
    return out;
}

QDataStream &operator>>(QDataStream &in, TestResult &result)
{
    in >> result.scenario >> result.script
       >> result.time >> result.platform >> result.detail;
    return in;
}
