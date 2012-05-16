#ifndef SCENARIOEX_H
#define SCENARIOEX_H

#include "../core/scenario.h"
#include "../core/trace.h"

class ScenarioEx : public Scenario
{
protected:
    QString m_name; // name of the scenario
    QMap<QString, QVector<RegularTraceItem> > m_traces;

public:
    ScenarioEx(qint32 seed = 12345, qint32 length = 60);

public: // interface for gui
    QString &name() { return m_name; }
    qint32 &seed() { return m_seed; }
    qint32 &length() { return m_length; }

    int userCount();
    int taskCount(const QString &username);

    bool insertUser(const QString &name);
    bool deleteUser(const QString &name);
    bool existUser(const QString &name);
    bool renameUser(const QString &name, const QString newName);

    bool insertTask(const QString &username, Task *task);
    bool deleteTask(const QString &username, int index);
    bool replaceTask(const QString &username, int index, Task *newTask);

    QString user(int index);
    Task *task(const QString &username, int index);

    QMap<QString, QVector<RegularTraceItem> >& getTraces() { return m_traces; }

public:
    friend QDataStream &operator<<(QDataStream &out, const ScenarioEx &scenario);
    friend QDataStream &operator>>(QDataStream &in, ScenarioEx &scenario);
};

QDataStream &operator<<(QDataStream &out, const ScenarioEx &scenario);
QDataStream &operator>>(QDataStream &in, ScenarioEx &scenario);

#endif // SCENARIOEX_H
