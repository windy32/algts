#include "scenarioview.h"

// +------------------------------------------------------------------+
// | Scenario Name                                                    |
// +--------------------+---------------------------------------------+
// |               [Del]| Task 1-1                              [Del] |
// |                    +---------------------------------------------+
// |       User 1       | Task 1-2                              [Del] |
// |                    +---------------------------------------------+
// |                    |                                       [New] |
// |                    |                                             |
// +--------------------+---------------------------------------------+
// |               [Del]| Task 2-1                              [Del] |
// |       User 2       +---------------------------------------------+
// |                    |                                       [New] |
// +--------------------+                                             |
// |     [New User]     |                                             |
// |                    |                                             |
// +--------------------+---------------------------------------------+
// |                    | 00:00              00:10              00:20 |
// +--------------------+---------------------------------------------+

// Delete Icon Item ///////////////////////////////////////////////////////////
DeleteIcon::DeleteIcon()
{
    m_hover = false;
}

void DeleteIcon::paint(QPainter *painter, const QStyleOptionGraphicsItem * /*option*/, QWidget */*widget*/)
{
    QPen pen(QBrush(m_hover ? Qt::darkGray : Qt::lightGray), 1.5);
    painter->setPen(pen);/*
    painter->setBrush(QBrush(m_hover ?
                                 QColor::fromRgb(0xE0, 0xE0, 0xE0) :
                                 QColor::fromRgb(0xF0, 0xF0, 0xF0)));*/
    painter->setRenderHint(QPainter::Antialiasing);
    //painter->drawEllipse(0, 0, 11, 11);
    painter->drawLine(3, 3, 7, 7);
    painter->drawLine(3, 7, 7, 3);
}

void DeleteIcon::hoverEnterEvent(QGraphicsSceneHoverEvent * /*event*/)
{
    m_hover = true;
    update();
}

void DeleteIcon::hoverLeaveEvent(QGraphicsSceneHoverEvent * /*event*/)
{
    m_hover = false;
    update();
}

QRectF DeleteIcon::boundingRect() const
{
    return QRectF(1, 2, 9, 9);
}

// Delete User Icon Item //////////////////////////////////////////////////////
void DeleteUserIcon::mousePressEvent(QGraphicsSceneMouseEvent * /*event*/)
{
    m_view->onUserDeleteClicked(m_name);
}

DeleteUserIcon::DeleteUserIcon(ScenarioView *view, const QString &name = "")
    : m_view(view), m_name(name)
{
}

// Delete Task Icon Item //////////////////////////////////////////////////////
DeleteTaskIcon::DeleteTaskIcon(ScenarioView *view, const QString &name, int index)
    : m_view(view), m_name(name), m_index(index)
{
}

void DeleteTaskIcon::mousePressEvent(QGraphicsSceneMouseEvent * /*event*/)
{
    m_view->onTaskDeleteClicked(m_name, m_index);
}

// New Icon Item //////////////////////////////////////////////////////////////
NewIcon::NewIcon()
{
    m_hover = false;
}

void NewIcon::paint(QPainter *painter, const QStyleOptionGraphicsItem * /*option*/, QWidget */*widget*/)
{
    QLinearGradient gradient(0.5, 0, 0.5, 1);
    gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    gradient.setColorAt(0.0, QColor(0xF8, 0xF8, 0xF8));
    gradient.setColorAt(1.0, m_hover ?
                            QColor(0xC0, 0xC0, 0xC0) :
                            QColor(0xDD, 0xDD, 0xDD));

    QBrush bgBrush(gradient);
    QPen pen(Qt::black);
    painter->setPen(pen);
    painter->setBrush(bgBrush);
    painter->drawRoundedRect(0, 0, 15, 15, 3, 3);

    QPen thickPen(QBrush(Qt::darkGray), 2.0);
    painter->setPen(thickPen);
    painter->drawLine(8, 5, 8, 11);
    painter->drawLine(5, 8, 11, 8);
}

void NewIcon::hoverEnterEvent(QGraphicsSceneHoverEvent * /*event*/)
{
    m_hover = true;
    update();
}

void NewIcon::hoverLeaveEvent(QGraphicsSceneHoverEvent * /*event*/)
{
    m_hover = false;
    update();
}

QRectF NewIcon::boundingRect() const
{
    return QRectF(0, 0, 15, 15);
}

// New User Item //////////////////////////////////////////////////////////////
NewUserIcon::NewUserIcon(ScenarioView *view)
    : m_view(view)
{
}

void NewUserIcon::mousePressEvent(QGraphicsSceneMouseEvent * /*event*/)
{
    m_view->onNewUserClicked();
}

// New Task Item //////////////////////////////////////////////////////////////
NewTaskIcon::NewTaskIcon(ScenarioView *view, const QString &username)
    : m_view(view), m_username(username)
{
}

void NewTaskIcon::paint(QPainter *painter, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/)
{
    QPen pen(QBrush(m_hover ? Qt::darkGray : Qt::lightGray), 1.5);
    painter->setPen(pen);
    painter->setRenderHint(QPainter::Antialiasing);

    painter->drawLine(QPointF(5, 2.5), QPointF(5, 7.5));
    painter->drawLine(QPointF(2.5, 5), QPointF(7.5, 5));
}

void NewTaskIcon::mousePressEvent(QGraphicsSceneMouseEvent * /*event*/)
{
    m_view->onNewTaskClicked(m_username);
}

QRectF NewTaskIcon::boundingRect() const
{
    return QRectF(1, 2, 9, 9);
}

// User Item //////////////////////////////////////////////////////////////////
UserItem::UserItem(ScenarioView *view, const QString &name)
    : m_view(view), m_username(name), m_selected(false)
{
}

QString UserItem::getName()
{
    return m_username;
}

void UserItem::paint(QPainter *painter, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/)
{
    QBrush brush(m_selected ?
                     QColor::fromRgb(0xE8, 0xE8, 0xE8) :
                     QColor::fromRgb(0xF8, 0xF8, 0xF8));
    painter->setBrush(brush);
    painter->drawRect(boundingRect());

    QFont font("Sans", 16, QFont::Bold);
    painter->setFont(font);
    painter->drawText(boundingRect(), Qt::AlignCenter, m_username);
}

void UserItem::mousePressEvent(QGraphicsSceneMouseEvent * /*event*/)
{
    m_view->setItemClicked();
    m_selected = true;
    m_view->onUserSelected(m_username);

    update();
}

QRectF UserItem::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

QSize UserItem::getTextSize()
{
    QFont font("Sans", 16, QFont::Bold);
    QFontMetricsF metrics(font);
    qreal textWidth = metrics.width(m_username);
    qreal textHeight = metrics.height();

    return QSize(textWidth, textHeight);
}

void UserItem::setName(const QString &name)
{
    m_username = name;
}

void UserItem::setSize(int width, int height)
{
    m_width = width;
    m_height = height;
}

void UserItem::setSelectionState(bool selected)
{
    m_selected = selected;
    update();
}

// Task Item //////////////////////////////////////////////////////////////////
TaskItem::TaskItem(ScenarioView *view, const QString &username, int index, Task *task)
    : m_view(view), m_selected(false), m_username(username), m_index(index), m_task(task)
{
}

void TaskItem::paint(QPainter *painter, const QStyleOptionGraphicsItem * /*option*/, QWidget */*widget*/)
{
    QBrush brush(m_selected ?
                     QColor::fromRgb(0xE8, 0xE8, 0xE8) :
                     QColor::fromRgb(0xF8, 0xF8, 0xF8));
    painter->setBrush(brush);
    painter->drawRect(boundingRect());
}

void TaskItem::mousePressEvent(QGraphicsSceneMouseEvent * /*event*/)
{
    m_view->setItemClicked();
    m_selected = true;
    m_view->onTaskSelected(m_username, m_index);

    update();
}

QRectF TaskItem::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

void TaskItem::setSize(int width, int height)
{
    m_width = width;
    m_height = height;
}

void TaskItem::setSelectionState(bool selected)
{
    m_selected = selected;
    update();
}

int TaskItem::getItemHeight()
{
    return 16;
}

// Timeline Item //////////////////////////////////////////////////////////////
TimelineItem::TimelineItem(int length, int xOffset)
    : m_length(length), m_xOffset(xOffset)
{
}

void TimelineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/)
{
    QRectF rect = boundingRect();

    // Background
    QBrush brush(QColor::fromRgb(0xF0, 0xF0, 0xF0));
    painter->setBrush(brush);
    painter->drawRect(rect);

    // Vertical Line
    QPen pen(QColor::fromRgb(0x40, 0x40, 0x40));
    painter->setPen(pen);
    painter->drawLine(m_xOffset, 0, m_xOffset, rect.height());

    // Scales
    QFont font("Sans", 8);
    painter->setFont(font);

    for(int i = 0; i < 5; i++)
    {
        int minutes = (m_length * i / 5) / 60;
        int seconds = (m_length * i / 5) % 60;

        int x = m_xOffset + 2 + (m_width - m_xOffset - 2) * i / 5;
        int y = rect.height() - 2;
        painter->drawText(x, y, QString("%1:%2")
                          .arg(minutes, 2, 10, QChar('0'))
                          .arg(seconds, 2, 10, QChar('0')));
        painter->drawLine(x - 2, 0, x - 2, 2);
    }

    int minutes = (m_length) / 60;
    int seconds = (m_length) % 60;
    int x = m_width - 34;
    int y = rect.height() - 2;
    pen.setColor(QColor::fromRgb(12, 34, 177));
    painter->setPen(pen);
    painter->drawText(x, y, QString("%1:%2")
                      .arg(minutes, 2, 10, QChar('0'))
                      .arg(seconds, 2, 10, QChar('0')));
}

void TimelineItem::setWidth(int width)
{
    m_width = width;
}

QRectF TimelineItem::boundingRect() const
{
    return QRectF(0, 0, m_width, 14);
}

// Title Item /////////////////////////////////////////////////////////////////
TitleItem::TitleItem(const QString &name)
{
    m_name = name;
}

void TitleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/)
{
    // Title rect
    QBrush brush(QColor::fromRgb(0xF0, 0xF0, 0xF0));
    painter->setBrush(brush);
    painter->drawRect(boundingRect());

    // Title text
    QPen titlePen(QColor::fromRgb(0x40, 0x40, 0x40));
    painter->setPen(titlePen);

    QFont titleFont("Sans", 9, QFont::Bold);
    painter->setFont(titleFont);
    painter->drawText(4, 13, m_name);
}

void TitleItem::setSize(int width, int height)
{
    m_width = width;
    m_height = height;
}

QRectF TitleItem::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}


// Scenario View //////////////////////////////////////////////////////////////
ScenarioView::ScenarioView(QWidget *parent) :
    QGraphicsView(parent), m_scenario(NULL)
{
    m_scene = new QGraphicsScene(this);
    setScene(m_scene);
}

void ScenarioView::resizeEvent(QResizeEvent * /*event*/)
{
    update();
}

void ScenarioView::mousePressEvent(QMouseEvent *event)
{
    if( m_scenario == NULL )
    {
        return;
    }

    m_itemClicked = false;
    QGraphicsView::mousePressEvent(event);

    if( !m_itemClicked ) // No item is selected
    {
        onScenarioSelected();
    }
}

void ScenarioView::setScenario(ScenarioEx *scenario)
{
    m_scenario = scenario;
}

void ScenarioView::update()
{
    if( m_scenario == NULL )
    {
        return;
    }

    m_scene->clear();
    m_userItems.clear();
    m_taskItems.clear();

    // Draw border
    QRectF borderRect = this->rect();
    borderRect.setWidth(borderRect.width() - 3);
    borderRect.setHeight(borderRect.height() - 2);
    m_scene->addRect(borderRect);

    // Scan the scenario and get basic sizes/positions
    QList<int> userItemHeights;
    QList<int> userItemOffsets; // y axis offset

    int maxWidth = 0;
    int offset = 0;
    for(int i = 0; i < m_scenario->userCount(); i++)
    {
        QString username = m_scenario->user(i);
        int taskCount = m_scenario->taskCount(username);

        userItemHeights.append(qMax(40, taskCount * TaskItem::getItemHeight() + 5));
        userItemOffsets.append(offset);
        offset += userItemHeights.last();
        offset += 4;

        UserItem *item = new UserItem(this, username);
        int width = item->getTextSize().width();
        maxWidth = qMax(maxWidth, width);
        m_userItems.append(item);
        m_taskItems.append(QList<TaskItem *>());
    }

    // Draw baseline
    m_scene->addRect(10, 0, maxWidth + 40, rect().height() - 2,
                     QPen(QColor::fromRgb(0xD0, 0xD0, 0xD0)),
                     QBrush(QColor::fromRgb(0xF8, 0xF8, 0xF8)));

    // Add user items
    for(int i = 0; i < m_scenario->userCount(); i++)
    {
        m_userItems[i]->setSize(maxWidth + 40, userItemHeights[i]);
        m_userItems[i]->setPos(10, 16 + userItemOffsets[i]);
        m_scene->addItem(m_userItems[i]);

        // plus a delete user icon
        DeleteUserIcon *icon = new DeleteUserIcon(this, m_scenario->user(i));
        icon->setPos(10 + maxWidth + 40 - 14, 16 + userItemOffsets[i] + 4);
        icon->setAcceptHoverEvents(true);
        m_scene->addItem(icon);

        // and a new task icon
        NewTaskIcon *newTaskIcon = new NewTaskIcon(this, m_scenario->user(i));
        newTaskIcon->setPos(10 + maxWidth + 40 - 24, 16 + userItemOffsets[i] + 4);
        newTaskIcon->setAcceptHoverEvents(true);
        m_scene->addItem(newTaskIcon);
    }

    // Add task items
    for(int i = 0; i < m_scenario->userCount(); i++)
    {
        QString username = m_scenario->user(i);
        for(int j = 0; j < m_scenario->taskCount(username); j++)
        {
            TaskItem *item = new TaskItem(this, username, j,
                                          m_scenario->task(username, j));
            item->setSize(this->rect().width() - 10 - maxWidth - 40 - 3, TaskItem::getItemHeight());
            item->setPos(10 + maxWidth + 40, 16 + userItemOffsets[i] + j * TaskItem::getItemHeight());
            m_taskItems[i].append(item);
            m_scene->addItem(item);

            // plus a delete task icon
            DeleteTaskIcon *icon = new DeleteTaskIcon(this, m_scenario->user(i), j);
            icon->setPos(this->rect().width() - 16, 16 + userItemOffsets[i] + j * TaskItem::getItemHeight() + 3);
            icon->setAcceptHoverEvents(true);
            m_scene->addItem(icon);
        }
    }

    // Add title item
    TitleItem *titleItem = new TitleItem(m_scenario->name());
    titleItem->setSize(this->rect().width() - 3, 16);
    titleItem->setPos(0, 0);
    m_scene->addItem(titleItem);

    // Add a new user icon
    NewUserIcon *newUserItem = new NewUserIcon(this);
    newUserItem->setPos(10 + (maxWidth + 40) / 2 - 7, offset + 20);
    newUserItem->setAcceptsHoverEvents(true);
    m_scene->addItem(newUserItem);

    // Add timeline item
    TimelineItem *timelineItem = new TimelineItem(m_scenario->length(), 10 + maxWidth + 40);
    timelineItem->setWidth(this->rect().width() - 3);
    timelineItem->setPos(0, this->rect().height() - 17);
    m_scene->addItem(timelineItem);
}

void ScenarioView::setItemClicked()
{
    m_itemClicked = true;
}

void ScenarioView::onScenarioSelected()
{
    emit scenarioSelected();

    // Set useritems and taskitems to unselected state
    for(int i = 0; i < m_userItems.size(); i++)
    {
        m_userItems[i]->setSelectionState(false);
        for(int j = 0; j < m_taskItems[i].size(); j++)
        {
            m_taskItems[i][j]->setSelectionState(false);
        }
    }
}

void ScenarioView::onUserSelected(const QString &username)
{
    emit userSelected(username);

    // Set other users unselected
    for(int i = 0; i < m_userItems.size(); i++)
    {
        if( m_userItems[i]->getName() != username )
        {
            m_userItems[i]->setSelectionState(false);
        }
        else
        {
            m_userItems[i]->setSelectionState(true);
        }
    }

    // Set tasks unselected
    for(int i = 0; i < m_userItems.size(); i++)
    {
        for(int j = 0; j < m_taskItems[i].size(); j++)
        {
            m_taskItems[i][j]->setSelectionState(false);
        }
    }

}

void ScenarioView::onTaskSelected(const QString &username, int index)
{
    emit taskSelected(username, index);

    // Set users unselected
    for(int i = 0; i < m_userItems.size(); i++)
    {
        m_userItems[i]->setSelectionState(false);
    }

    // Set other tasks unselected
    for(int i = 0; i < m_userItems.size(); i++)
    {
        for(int j = 0; j < m_taskItems[i].size(); j++)
        {
            if( m_userItems[i]->getName() != username || j != index )
            {
                m_taskItems[i][j]->setSelectionState(false);
            }
        }
    }
}

void ScenarioView::onUserDeleteClicked(const QString &username)
{
    emit userDeleteClicked(username);
}

void ScenarioView::onTaskDeleteClicked(const QString &username, int index)
{
    emit taskDeleteClicked(username, index);
}

void ScenarioView::onNewUserClicked()
{
    emit newUserClicked();
}

void ScenarioView::onNewTaskClicked(const QString &username)
{
    emit newTaskClicked(username);
}
