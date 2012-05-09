#include "scenarioex.h"

ScenarioEx::ScenarioEx(qint32 seed, qint32 length)
    : Scenario(seed, length)
{
}

int ScenarioEx::userCount()
{
    return m_tasks.count();
}

int ScenarioEx::taskCount(const QString &username)
{
    if( m_tasks.contains(username))
    {
        return m_tasks[username].size();
    }
    return 0;
}

bool ScenarioEx::insertUser(const QString &name)
{
    if( m_tasks.contains(name))
    {
        return false;
    }
    m_tasks.insert(name, QVector<Task *>());
    return true;
}

bool ScenarioEx::deleteUser(const QString &name)
{
    if( m_tasks.contains(name))
    {
        m_tasks.remove(name);
        return true;
    }
    return false;
}

bool ScenarioEx::renameUser(const QString &name, const QString newName)
{
    if( m_tasks.contains(name) && !m_tasks.contains(newName))
    {
        m_tasks[newName] = m_tasks[name];
        m_tasks.remove(name);
        return true;
    }
    return false;
}

bool ScenarioEx::insertTask(const QString &username, Task *task)
{
    if( m_tasks.contains(username))
    {
        m_tasks[username].append(task);
        return true;
    }
    return false;
}

bool ScenarioEx::deleteTask(const QString &username, int index)
{
    if( m_tasks.contains(username))
    {
        m_tasks[username].remove(index);
        return true;
    }
    return false;
}

bool ScenarioEx::replaceTask(const QString &username, int index, Task *newTask)
{
    if( m_tasks.contains(username))
    {
        Task *t = m_tasks[username][index];
        m_tasks[username][index] = newTask;
        delete t;
        return true;
    }
    return false;
}

QString ScenarioEx::user(int index)
{
    QMap<QString, QVector<Task *> >::iterator it = m_tasks.begin();
    for(int i = 0; i < index; i++)
    {
        ++it;
    }
    return it.key();
}

Task *ScenarioEx::task(const QString &username, int index)
{
    if( m_tasks.contains(username))
    {
        if( index >= 0 && index < m_tasks[username].size())
        {
            return m_tasks[username][index];
        }
    }
    return NULL;
}
