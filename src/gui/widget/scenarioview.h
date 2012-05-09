#ifndef SCENARIOVIEW_H
#define SCENARIOVIEW_H

#include <QGraphicsView>
#include <QGraphicsItem>
#include "../scenarioex.h"

class ScenarioView;

class UserItem : public QGraphicsItem
{
private:
    ScenarioView *m_view;
    QString m_username;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

public:
    UserItem(ScenarioView *view, const QString &name = "");
    QRectF boundingRect() const;

    void setName(const QString &name);
};

class TaskItem : public QGraphicsItem
{
    // TODO
};

class ScenarioView : public QGraphicsView
{
    Q_OBJECT

private:
    QGraphicsScene *m_scene;
    ScenarioEx *m_scenario;

public:
    explicit ScenarioView(QWidget *parent = 0);
    void setScenario(ScenarioEx *scenario);
    void update();

signals:
    void scenarioSelected();
    void userSelected(const QString &username);
    void taskSelected(const QString &username, int index);

public slots: // for graphics items
    void onScenarioSelected();
    void onUserSelected(const QString &username);
    void onTaskSelected(const QString &username, int index);
};

#endif // SCENARIOVIEW_H
