#include "globaldatabase.h"

GlobalDatabase *GlobalDatabase::m_instance = NULL;

GlobalDatabase::GlobalDatabase()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("algts.db");
    if( !m_db.open())
    {
        qDebug() << "Cannot open database";
    }
}

GlobalDatabase *GlobalDatabase::instance()
{
    if( m_instance == NULL )
    {
        m_instance = new GlobalDatabase();
    }
    return m_instance;
}

int GlobalDatabase::getScenarioCount()
{
    return 0;
}

bool GlobalDatabase::getScenario(int index, Scenario &scenario)
{
    return false;
}

bool GlobalDatabase::addScenario(Scenario &scenario)
{
    return false;
}

int GlobalDatabase::getScriptCount()
{
    return 0;
}

bool GlobalDatabase::getScript(int index, Script &script)
{
    return false;
}

bool GlobalDatabase::addScript(Script &script)
{
    return false;
}
