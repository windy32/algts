#ifndef GLOBALDATABASE_H
#define GLOBALDATABASE_H

#include <QtSql>
#include "script.h"

class GlobalDatabase
{
private:
    static GlobalDatabase *m_instance;
    QSqlDatabase m_db;

private:
    GlobalDatabase();

public:
    static GlobalDatabase *instance();
    static void destroy();

public:
    int getScenarioCount();
    void getScenario(int index, Scenario &scenario); // index starts with 0
    void addScenario(Scenario &scenario);

    int getScriptCount();
    bool existScript(const QString &name);
    void getScript(int index, Script &script); // index starts with 0
    void setScript(int index, Script &script);
    void addScript(Script &script);
};

#endif // GLOBALDATABASE_H
