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
    int m_width;
    int m_height;
    bool m_selected;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

public:
    UserItem(ScenarioView *view, const QString &name = "");
    QString getName();

    QRectF boundingRect() const;

    QSize getTextSize();
    void setName(const QString &name);
    void setSize(int width, int height);
    void setSelectionState(bool selected);
};

class TaskItem : public QGraphicsItem
{
private:
    ScenarioView *m_view;
    int m_width;
    int m_height;
    bool m_selected;

    QString m_username;
    int m_index;
    Task *m_task;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

public:
    TaskItem(ScenarioView *view, const QString &username, int index, Task *task);
    QRectF boundingRect() const;
    void setSize(int width, int height);
    void setSelectionState(bool selected);

public:
    static int getItemHeight();
};

class TimelineItem : public QGraphicsItem
{
private:
    int m_length;
    int m_width;
    int m_xOffset;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public:
    TimelineItem(int length, int xOffset); // in seconds
    void setWidth(int width);
    QRectF boundingRect() const;
};

class TitleItem : public QGraphicsItem
{
private:
    QString m_name;
    int m_width;
    int m_height;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public:
    TitleItem(const QString &name);
    void setSize(int width, int height);
    QRectF boundingRect() const;
};

class ScenarioView : public QGraphicsView
{
    Q_OBJECT

private:
    QGraphicsScene *m_scene;
    ScenarioEx *m_scenario;

    QList<UserItem *> m_userItems;
    QList<QList<TaskItem *> > m_taskItems;

    bool m_itemClicked;

protected:
    void resizeEvent(QResizeEvent * event);
    void mousePressEvent(QMouseEvent *event);

public:
    explicit ScenarioView(QWidget *parent = 0);
    void setScenario(ScenarioEx *scenario);
    void update();
    void setItemClicked();

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
