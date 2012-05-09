#include "scenarioview.h"

// User Item //////////////////////////////////////////////////////////////////
UserItem::UserItem(ScenarioView *view, const QString &name)
    : m_view(view), m_username(name)
{
}

void UserItem::paint(QPainter *painter, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/)
{
    painter->drawRect(boundingRect());
}

void UserItem::mousePressEvent(QGraphicsSceneMouseEvent * /*event*/)
{
    m_view->onUserSelected(m_username);
}

QRectF UserItem::boundingRect() const
{
    QFont font("Sans", 12, QFont::Bold);
    QFontMetricsF metrics(font);
    qreal textWidth = metrics.width(m_username);
    qreal textHeight = metrics.height();

    return QRectF(-textWidth / 2 - 20, -textHeight / 2 - 10,
                  textWidth + 40, textHeight + 20);
}

void UserItem::setName(const QString &name)
{
    m_username = name;
}

// Scenario View //////////////////////////////////////////////////////////////
ScenarioView::ScenarioView(QWidget *parent) :
    QGraphicsView(parent), m_scenario(NULL)
{
    m_scene = new QGraphicsScene(this);
    setScene(m_scene);
}

void ScenarioView::setScenario(ScenarioEx *scenario)
{
    m_scenario = scenario;
}

void ScenarioView::update()
{
    m_scene->clear();
    for(int i = 0; i < m_scenario->userCount(); i++)
    {
        m_scene->addItem(new UserItem(this, m_scenario->user(i)));
    }
}

void ScenarioView::onScenarioSelected()
{
    emit scenarioSelected();
}

void ScenarioView::onUserSelected(const QString &username)
{
    emit userSelected(username);
}

void ScenarioView::onTaskSelected(const QString &username, int index)
{
    emit taskSelected(username, index);
}
