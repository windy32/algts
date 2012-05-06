#include "linegraph.h"

LineGraph::LineGraph(QPainter *painter, int width, int height,
                     int minX, int maxX, double minY, double maxY)
    : m_painter(painter), m_width(width), m_height(height),
      m_minX(minX), m_maxX(maxX), m_minY(minY), m_maxY(maxY)
{
    m_baseX = 40;
    m_baseY = m_height - 20;

    m_graphWidth = m_width - 50;
    m_graphHeight = m_height - 30;

    m_painter->setFont(QFont("Sans", 8));

    m_borderColor = Qt::gray;
    m_lineColor = QColor(18, 98, 184);
}

void LineGraph::addBorder()
{
    m_painter->setPen(m_borderColor);
    m_painter->drawRect(m_baseX - 1, m_baseY - m_graphHeight - 1,
                        m_graphWidth + 2, m_graphHeight + 2);
}

void LineGraph::addXScale(int flags, int xValue)
{
    int x = m_baseX + m_graphWidth * (xValue - m_minX) / (m_maxX - m_minX);
    int y = m_baseY + 2;
    m_painter->setPen(Qt::black);

    if( flags & Qt::AlignLeft )
    {
        m_painter->drawText(x, y, 32, 10, Qt::AlignLeft,
                            QString("%1").arg(xValue));
    }
    else if( flags & Qt::AlignRight )
    {
        m_painter->drawText(x - 32, y, 32, 10, Qt::AlignRight,
                            QString("%1").arg(xValue));
    }

}

void LineGraph::addYScale(int flags, double yValue, int precision)
{
    int x = 0;
    int y = m_baseY - m_graphHeight * (yValue - m_minY) / (m_maxY - m_minY);
    m_painter->setPen(Qt::black);

    if( flags & Qt::AlignTop )
    {
        m_painter->drawText(x, y - 3, 36, 10, Qt::AlignRight,
                            QString("%1").arg(yValue, 0, 'f', precision));
    }
    else if( flags & Qt::AlignBottom )
    {
        m_painter->drawText(x, y - 10, 36, 10, Qt::AlignRight,
                            QString("%1").arg(yValue, 0, 'f', precision));
    }
}

void LineGraph::addLine(double x1, double x2, double y1, double y2)
{
    m_painter->setPen(m_lineColor);
    m_painter->drawLine(
                m_baseX + m_graphWidth * (x1 - m_minX) / (m_maxX - m_minX),
                m_baseY - m_graphHeight * (y1 - m_minY) / (m_maxY - m_minY),
                m_baseX + m_graphWidth * (x2 - m_minX) / (m_maxX - m_minX),
                m_baseY - m_graphHeight * (y2 - m_minY) / (m_maxY - m_minY)
                );
}

