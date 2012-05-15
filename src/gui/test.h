#ifndef TEST_H
#define TEST_H

#include "scenarioex.h"
#include "script.h"

struct Test
{
    ScenarioEx scenario;
    ScriptData script;
};

struct TestResult
{
    ScenarioEx scenario;
    ScriptData script;
    QString time;
    QString platform;
    QString detail;
};

QDataStream &operator<<(QDataStream &out, const TestResult &result);
QDataStream &operator>>(QDataStream &in, TestResult &result);

#endif // TEST_H
