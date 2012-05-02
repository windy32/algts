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

    update();
}

//  +-----Widget-Area------------------------+
//  |                                        |
//  |      +----Graph-Area--------------+    |
//  | 0.02 |                            |    |
//  |      |        /| /\               |    |
//  | 0.01 |       / |/  \____          |    |
//  |      | _____/           \________ |    |
//  | 0.00 +----------------------------+    |
//  |      0    100             1000   1100  |
//  |                                        |
//  +----------------------------------------+
class LineGraph // Should be reconstructed lator
{
private:
    QPainter *m_painter;

    int m_width; // Size of the widget area
    int m_height;

    int m_graphWidth; // Size of the graph area
    int m_graphHeight;

    int m_minX;
    int m_maxX;
    int m_minY;
    int m_maxY;

    int m_baseX; // Lower left corner of the graph area
    int m_baseY;

    QColor m_borderColor;
    QColor m_lineColor;

public:
    LineGraph(QPainter *painter, int width, int height,
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

    void addBorder()
    {
        m_painter->setPen(m_borderColor);
        m_painter->drawRect(m_baseX - 1, m_baseY - m_graphHeight - 1,
                            m_graphWidth + 2, m_graphHeight + 2);
    }

    void addXScale(int flags, int xValue)
    {
        int x = m_baseX + m_graphWidth * (xValue - m_minX) / (m_maxX - m_minX);
        int y = m_baseY + 2;
        m_painter->setPen(Qt::black);

        if( flags & Qt::AlignLeft )
        {
            m_painter->drawText(x, y, 30, 10, Qt::AlignLeft,
                                QString("%1").arg(xValue));
        }
        else if( flags & Qt::AlignRight )
        {
            m_painter->drawText(x - 30, y, 30, 10, Qt::AlignRight,
                                QString("%1").arg(xValue));
        }

    }

    void addYScale(int flags, double yValue, int precision = 2)
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

    void addLine(int x1, int x2, double y1, double y2)
    {
        m_painter->setPen(m_lineColor);
        m_painter->drawLine(
                    m_baseX + m_graphWidth * (x1 - m_minX) / (m_maxX - m_minX),
                    m_baseY - m_graphHeight * (y1 - m_minY) / (m_maxY - m_minY),
                    m_baseX + m_graphWidth * (x2 - m_minX) / (m_maxX - m_minX),
                    m_baseY - m_graphHeight * (y2 - m_minY) / (m_maxY - m_minY)
                    );
    }
};

void CdfWidget::paintEvent(QPaintEvent * /* event */)
{
    int width = this->width();
    int height = this->height();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(Qt::white);

    // Draw widget bg
    painter.drawRect(0, 0, width - 1, height - 10 - 4 - 1);

    // Draw text
    painter.setFont(QFont("Sans", 10));
    painter.setPen(Qt::darkGray);
    painter.drawText(0, height - 10, width, 11, Qt::AlignCenter,
                     "Cumulative Distribution Function");

    // Draw graph
    if( m_curDistribution == UNIFORM )
    {
        int minX = m_uniformParam.min * 0.9 - m_uniformParam.max * 0.1;
        minX = qMax(0, minX);
        int maxX = m_uniformParam.max * 1.1 - m_uniformParam.min * 0.1;

        LineGraph g(&painter, width, height - 10 - 4, minX, maxX , 0.0, 1.0);
        g.addBorder();

        g.addXScale(Qt::AlignLeft, minX);
        g.addXScale(Qt::AlignRight, maxX);

        g.addYScale(Qt::AlignBottom, 0.0);
        g.addYScale(Qt::AlignTop, 1.0);

        g.addLine(minX, m_uniformParam.min, 0.0, 0.0);
        g.addLine(m_uniformParam.min, m_uniformParam.max, 0.0, 1.0);
        g.addLine(m_uniformParam.max, maxX, 1.0, 1.0);
    }
}
