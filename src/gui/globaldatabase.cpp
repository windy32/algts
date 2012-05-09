#include "globaldatabase.h"

// algts.db consists of following tables:
//
// - Scenario
//    - UID          [Key] : Integer
//    - Data               : Blob
//
// - Script
//    - UID          [Key] : Integer
//    - Data               : Blob

GlobalDatabase *GlobalDatabase::m_instance = NULL;

GlobalDatabase::GlobalDatabase()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("algts.db");
    if( !m_db.open())
    {
        qDebug() << "Cannot open database";
    }
    else
    {
        QSqlQuery query;
        query.exec(
            "Create Table If Not Exists Scenario("
            "    UID         Integer,"
            "    Data        Blob,"
            "    "
            "    Primary Key (UID)"
            ");");

        query.exec(
            "Create Table If Not Exists Script("
            "    UID         Integer,"
            "    Data        Blob,"
            "    "
            "    Primary Key (UID)"
            ");");
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

void GlobalDatabase::destroy()
{
    if( m_instance != NULL )
    {
        m_instance->m_db.close();
        delete m_instance;
        m_instance = NULL;
    }
}

int GlobalDatabase::getScenarioCount()
{
    QSqlQuery query;
    query.exec("Select Count(*) From Scenario");
    query.next();
    return query.value(0).toInt();
}

void GlobalDatabase::getScenario(int index, Scenario &scenario)
{
    QSqlQuery query(QString("Select Data From Scenario where UID = %1").arg(index + 1));

    if( query.next())
    {
        QByteArray data;
        QDataStream in(&data, QIODevice::ReadOnly);

        data = query.value(0).toByteArray();
        in >> scenario;
    }
    else
    {
        qDebug() << QString("No scenario with index %1 found").arg(index);
    }
}

void GlobalDatabase::addScenario(Scenario &scenario)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << scenario;

    QSqlQuery query;
    query.prepare("Insert Into Scenario(Data) values (:data)");
    query.bindValue(":data", data);
    query.exec();
}

int GlobalDatabase::getScriptCount()
{
    // qDebug() << "Entering GlobalDatabase::getScriptCount";

    QSqlQuery query;
    query.exec("Select Count(*) From Script");
    query.next();
    return query.value(0).toInt();
}

bool GlobalDatabase::existScript(const QString &name)
{
    // qDebug() << "Entering GlobalDatabase::existScript";

    // Should be optimized later
    Script script;
    int count = getScriptCount();

    for(int i = 0; i < count; i++)
    {
        getScript(i, script);
        if( name == script.name)
        {
            return true;
        }
    }
    return false;
}

void GlobalDatabase::getScript(int index, Script &script)
{
    // qDebug() << "Entering GlobalDatabase::getScript";

    QSqlQuery query(QString("Select Data From Script where UID = %1").arg(index + 1));
    if( query.next())
    {
        QByteArray data;
        QDataStream in(&data, QIODevice::ReadOnly);

        data = query.value(0).toByteArray();
        in >> script;
    }
    else
    {
        qDebug() << QString("No script with index %1 found").arg(index);
    }
}

void GlobalDatabase::setScript(int index, Script &script)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << script;

    QSqlQuery query;
    query.prepare(QString("Update Script set Data = :data Where UID = %1").arg(index + 1));
    query.bindValue(":data", data);
    query.exec();
}

void GlobalDatabase::addScript(Script &script)
{
    // qDebug() << "Entering GlobalDatabase::addScript";

    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << script;

    QSqlQuery query;
    query.prepare("Insert Into Script(Data) values (:data)");
    query.bindValue(":data", data);
    query.exec();
}
