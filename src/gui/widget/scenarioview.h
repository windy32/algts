#ifndef SCENARIOVIEW_H
#define SCENARIOVIEW_H

#include <QGraphicsView>
#include <QGraphicsItem>
#include "../scenarioex.h"

class ScenarioView;

class DeleteIcon : public QGraphicsItem
{
protected:
    bool m_hover;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

public:
    DeleteIcon();
    QRectF boundingRect() const;
};

class DeleteUserIcon : public DeleteIcon
{
protected:
    ScenarioView *m_view;
    QString m_name;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

public:
    DeleteUserIcon(ScenarioView *view, const QString &name);
};

class DeleteTaskIcon : public DeleteIcon
{
protected:
    ScenarioView *m_view;
    QString m_name;
    int m_index;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

public:
    DeleteTaskIcon(ScenarioView *view, const QString &name, int index);
};

class NewIcon : public QGraphicsItem
{
protected:
    bool m_hover;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

public:
    NewIcon();
    QRectF boundingRect() const;
};

class NewUserIcon : public NewIcon
{
protected:
    ScenarioView *m_view;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

public:
    NewUserIcon(ScenarioView *view);
};

class NewTaskIcon : public NewIcon
{
protected:
    ScenarioView *m_view;
    QString m_username;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    QRectF boundingRect() const;

public:
    NewTaskIcon(ScenarioView *view, const QString &username);
};


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

    void userDeleteClicked(const QString &username);
    void taskDeleteClicked(const QString &username, int index);

    void newUserClicked();
    void newTaskClicked(const QString &username);

public slots: // for graphics items
    void onScenarioSelected();
    void onUserSelected(const QString &username);
    void onTaskSelected(const QString &username, int index);

    void onUserDeleteClicked(const QString &username);
    void onTaskDeleteClicked(const QString &username, int index);

    void onNewUserClicked();
    void onNewTaskClicked(const QString &username);
};

#endif // SCENARIOVIEW_H
