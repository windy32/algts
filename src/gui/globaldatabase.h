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
    GlobalDatabase *instance();

    int getScenarioCount();
    bool getScenario(int index, Scenario &scenario);
    bool addScenario(Scenario &scenario);

    int getScriptCount();
    bool getScript(int index, Script &script);
    bool addScript(Script &script);
};

#endif // GLOBALDATABASE_H
