#include "cdfwidget.h"
#include <QPainter>

CdfWidget::CdfWidget(QWidget *parent) :
    QWidget(parent)
{
    m_curDistribution = NONE;
}

void CdfWidget::setDistribution(const QString &distribution)
{
    QRegExp u2("^Uniform ([0-9]+),\\s*([0-9]+)$");
    QRegExp e2("^Exponential ([0-9]+),\\s*([0-9]+)$");
    QRegExp p3("^Pareto ([0-9]+),\\s*([0-9]+|[0-9]+\\.[0-9]+),\\s*([0-9]+)$");

    if( u2.indexIn(distribution) != -1 )
    {
        m_curDistribution = UNIFORM;

        m_uniformParam.min = u2.cap(1).toInt();
        m_uniformParam.max = u2.cap(2).toInt();
    }
    else if( e2.indexIn(distribution) != -1 )
    {
        m_curDistribution = EXPONENTIAL;

        m_exponentialParam.mean = e2.cap(1).toInt();
        m_exponentialParam.bound = e2.cap(2).toInt();
    }
    else if( p3.indexIn(distribution) != -1 )
    {
        m_curDistribution = PARETO;

        m_paretoParam.mean = p3.cap(1).toInt();
        m_paretoParam.shape = p3.cap(2).toDouble();
        m_paretoParam.bound = p3.cap(3).toInt();
    }
}

//  +-----Widget-Area------------------------+
//  |                                        |
//  |      +----Graph-Area--------------+    |
//  | 0.02 |                            |    |
//  |      |                            |    |
//  | 0.01 |                            |    |
//  |      |                            |    |
//  | 0.00 +----------------------------+    |
//  |      0    100             1000   1100  |
//  |                                        |
//  +----------------------------------------+
class LineGraph
{
private:
    QPainter *m_painter;

public:
    LineGraph(QPainter *painter, QRect *widgetRect, QRect *graphWidget);
};

void CdfWidget::paintEvent(QPaintEvent *event)
{
    int width = this->width();
    int height = this->height();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(Qt::white);

    painter.drawRect(0, 0, width - 1, height - 1);
}
