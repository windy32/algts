#include "cdfwidget.h"
#include "linegraph.h"

#include <math.h>

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

        LineGraph g(&painter, width, height - 10 - 4, minX, maxX, 0.0, 1.0);
        g.addBorder();

        g.addXScale(Qt::AlignLeft, minX);
        g.addXScale(Qt::AlignRight, maxX);

        g.addYScale(Qt::AlignBottom, 0.0);
        g.addYScale(Qt::AlignTop, 1.0);

        g.addLine(minX, m_uniformParam.min, 0.0, 0.0);
        g.addLine(m_uniformParam.min, m_uniformParam.max, 0.0, 1.0);
        g.addLine(m_uniformParam.max, maxX, 1.0, 1.0);
    }
    else if( m_curDistribution == PARETO )
    {
        // CDF(x) = 1 - (xm / x) ^ alpha (x >= xm)
        // PDF(x) = alpha * xm ^ alpha / x ^ (alpha + 1)
        // mean = alpha / (alpha - 1) * xm

        // shape = alpha
        // xm = mean * (alpha - 1) / alpha
        double xm = (m_paretoParam.mean *
                       (m_paretoParam.shape - 1) / m_paretoParam.shape);
        int minX = 0;
        int maxX = xm * 3;

        LineGraph g(&painter, width, height - 10 - 4, minX, maxX, 0.0, 1.0);
        g.addBorder();

        g.addXScale(Qt::AlignLeft, minX);
        g.addXScale(Qt::AlignRight, maxX);

        g.addYScale(Qt::AlignBottom, 0.0);
        g.addYScale(Qt::AlignTop, 1.0);

        for(double x1 = xm; x1 < maxX; x1 += 1.0)
        {
            double x2 = x1 + 1;
            double y1 = 1 - pow(xm / x1, m_paretoParam.shape);
            double y2 = 1 - pow(xm / x2, m_paretoParam.shape);
            g.addLine(x1, x2, y1, y2);
        }
    }
}
