#include "linegraph.h"

#include <QDebug>

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

LineGraph::LineGraph(QPainter *painter, int width, int height,
                     int leftMargin, int rightMargin,
                     int minX, int maxX, double minY, double maxY)
    : m_painter(painter), m_width(width), m_height(height),
      m_minX(minX), m_maxX(maxX), m_minY(minY), m_maxY(maxY)
{
    m_baseX = leftMargin;
    m_baseY = m_height - 20;

    m_graphWidth = m_width - leftMargin - rightMargin;
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

void LineGraph::addXScaleAsTime(int flags, int seconds)
{
    int x = m_baseX + m_graphWidth * (seconds - m_minX) / (m_maxX - m_minX);
    int y = m_baseY + 2;
    m_painter->setPen(Qt::black);

    int min = seconds / 60;
    int sec = seconds % 60;

    if( flags & Qt::AlignLeft )
    {
        m_painter->drawText(x, y, 32, 10, Qt::AlignLeft, QString("%1:%2")
                            .arg(min, 2, 10, QChar('0'))
                            .arg(sec, 2, 10, QChar('0')));
    }
    else if( flags & Qt::AlignRight )
    {
        m_painter->drawText(x - 32, y, 32, 10, Qt::AlignRight, QString("%1:%2")
                            .arg(min, 2, 10, QChar('0'))
                            .arg(sec, 2, 10, QChar('0')));
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

void LineGraph::addYScale(int flags, double yValue, const QString &unit, int precision)
{
    int x = 0;
    int y = m_baseY - m_graphHeight * (yValue - m_minY) / (m_maxY - m_minY);
    m_painter->setPen(Qt::black);

    if( flags & Qt::AlignTop )
    {
        m_painter->drawText(x, y - 3, m_baseX - 4, 12, Qt::AlignRight,
                            QString("%1%2").arg(yValue, 0, 'f', precision).arg(unit));
    }
    else if( flags & Qt::AlignBottom )
    {
        m_painter->drawText(x, y - 10, m_baseX - 4, 12, Qt::AlignRight,
                            QString("%1%2").arg(yValue, 0, 'f', precision).arg(unit));
    }
}

void LineGraph::addYScaleText(int flags, double position, int xOffset, int yOffset, const QString &text, QColor color)
{
    int x = 0;
    int y = m_baseY - m_graphHeight * position;
    m_painter->setPen(color);

    if( flags & Qt::AlignTop )
    {
        m_painter->drawText(x + xOffset, y + yOffset - 3, m_baseX - 4, 12,
                            Qt::AlignRight, text);
    }
    else if( flags & Qt::AlignBottom )
    {
        m_painter->drawText(x + xOffset, y + yOffset - 10, m_baseX - 4, 12,
                            Qt::AlignRight, text);
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

void LineGraph::addLine(double x1, double x2, double y1, double y2, QColor color)
{
    m_painter->setPen(color);
    m_painter->drawLine(
                m_baseX + m_graphWidth * (x1 - m_minX) / (m_maxX - m_minX - 1),
                m_baseY - m_graphHeight * (y1 - m_minY) / (m_maxY - m_minY),
                m_baseX + m_graphWidth * (x2 - m_minX) / (m_maxX - m_minX - 1),
                m_baseY - m_graphHeight * (y2 - m_minY) / (m_maxY - m_minY)
                );
}

void LineGraph::addLegend(const QList<QString> &names, const QList<QColor> &colors)
{
    // (x2, y2)
    // x----------------x (x1, y1) the upper right corner
    // | RxRate    ---- |
    // | MaxRxRate ==== |
    // +----------------+
    //
    m_painter->setPen(Qt::darkGray);
    m_painter->setFont(QFont("Sans", 7.5));

    double x1 = m_baseX + m_graphWidth - 10; // Upper right corner of the legend
    double y1 = m_baseY - m_graphHeight + 10;
    double width = 100;
    double height = 6 + names.size() * 19;
    double x2 = x1 - width;
    double y2 = y1;

    // Draw border
    m_painter->setRenderHint(QPainter::Antialiasing, false);
    m_painter->setBrush(QColor(255, 255, 255, 192));
    m_painter->drawRect(x2, y2, width, height);

    for(int i = 0; i < names.size(); i++)
    {
        // Draw line & box
        m_painter->setBrush(Qt::NoBrush);
        m_painter->drawRect(x2 + 6, y2 + 5 + 19 * i + 1, 24, 13);

        m_painter->setRenderHint(QPainter::Antialiasing, true);
        m_painter->setPen(colors[i]);
        m_painter->drawLine(x2 + 12, y2 + 13 + 19 * i,
                            x2 + 25, y2 + 13 + 19 * i);
        m_painter->setPen(Qt::black);
        m_painter->setRenderHint(QPainter::Antialiasing, false);

        // Draw text
        m_painter->drawText(x2 + 38, y2 + 8 + 19 * i, 80, 14, Qt::AlignLeft, names[i]);

    }
}
