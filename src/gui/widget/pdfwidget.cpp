#include "pdfwidget.h"
#include "linegraph.h"

#include <math.h>
#include <QDebug>

PdfWidget::PdfWidget(QWidget *parent) :
    QWidget(parent)
{
    m_curDistribution = NONE;
}

void PdfWidget::setDistribution(const QString &distribution)
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

void PdfWidget::paintEvent(QPaintEvent * /*event*/)
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
                     "Probability Density Function");

    // Draw graph
    if( m_curDistribution == UNIFORM )
    {
        int minX = m_uniformParam.min * 0.9 - m_uniformParam.max * 0.1;
        minX = qMax(0, minX);
        int maxX = m_uniformParam.max * 1.1 - m_uniformParam.min * 0.1;
        maxX = qMax(1, maxX);
        maxX = qMax(m_uniformParam.max + 1, maxX);

        LineGraph g(&painter, width, height - 10 - 4,
                    minX, maxX, 0.0,
                    (m_uniformParam.min == m_uniformParam.max) ?
                        1.0 : 1.4 / (m_uniformParam.max - m_uniformParam.min));
        g.addBorder();

        g.addXScale(Qt::AlignLeft, minX);
        g.addXScale(Qt::AlignRight, maxX);

        g.addYScale(Qt::AlignBottom, 0.0, 3);
        g.addYScale(Qt::AlignTop, (m_uniformParam.min == m_uniformParam.max) ?
                        1.0 : 1.4 / (m_uniformParam.max - m_uniformParam.min), 3);

        if(m_uniformParam.min == m_uniformParam.max)
        {
            g.addLine(m_uniformParam.min, m_uniformParam.max, 0.0, 1.0);
        }
        else
        {
            g.addLine(minX, m_uniformParam.min, 0.0, 0.0);
            g.addLine(m_uniformParam.min, m_uniformParam.min,
                      0, 1.0 / (m_uniformParam.max - m_uniformParam.min));
            g.addLine(m_uniformParam.min, m_uniformParam.max,
                      1.0 / (m_uniformParam.max - m_uniformParam.min),
                      1.0 / (m_uniformParam.max - m_uniformParam.min));
            g.addLine(m_uniformParam.max, m_uniformParam.max,
                      1.0 / (m_uniformParam.max - m_uniformParam.min), 0.0);
            g.addLine(m_uniformParam.max, maxX, 0.0, 0.0);
        }
    }
    else if( m_curDistribution == PARETO )
    {
        // CDF(x) = 1 - (xm / x) ^ shape (x >= xm)
        // PDF(x) = shape * xm ^ shape / x ^ (shape + 1)
        // mean = shape / (shape - 1) * xm

        // xm = mean * (alpha - 1) / shape
        double xm = (m_paretoParam.mean *
                       (m_paretoParam.shape - 1) / m_paretoParam.shape);
        int minX = 0;
        int maxX = qMax(1, (int)(xm * 3.0));

        LineGraph g(&painter, width, height - 10 - 4, minX, maxX, 0.0,
                    (m_paretoParam.bound == 0) ?
                        m_paretoParam.shape / xm :
                        m_paretoParam.shape / xm / (1 - pow(xm / m_paretoParam.mean, m_paretoParam.shape)));
        g.addBorder();

        g.addXScale(Qt::AlignLeft, minX);
        g.addXScale(Qt::AlignRight, maxX);

        g.addYScale(Qt::AlignBottom, 0.0, 3);
        g.addYScale(Qt::AlignTop, (m_paretoParam.bound == 0) ?
                        m_paretoParam.shape / xm :
                        m_paretoParam.shape / xm / (1 - pow(xm / m_paretoParam.mean, m_paretoParam.shape)), 3);

        double step = (maxX - xm) / 200.0;

        g.addLine(0, xm, 0, 0);
        g.addLine(xm, xm, 0, (m_paretoParam.bound == 0) ?
                      m_paretoParam.shape / xm :
                      m_paretoParam.shape / xm / (1 - pow(xm / m_paretoParam.bound, m_paretoParam.shape)));

        for(double x1 = xm; x1 < maxX; x1 += step)
        {
            double x2 = x1 + step;
            double y1 = m_paretoParam.shape * pow(xm, m_paretoParam.shape) /
                    pow(x1, (m_paretoParam.shape + 1));
            double y2 = m_paretoParam.shape * pow(xm, m_paretoParam.shape) /
                    pow(x2, (m_paretoParam.shape + 1));

            if( m_paretoParam.bound > 0 && x2 < m_paretoParam.bound )
            {
                double lamda = (x1 - xm) / (m_paretoParam.bound - xm);
                y1 /= 1 - pow(xm / m_paretoParam.bound, m_paretoParam.shape);
                y2 /= 1 - pow(xm / m_paretoParam.bound, m_paretoParam.shape);
                y1 = y1 * (1 - lamda);
                y2 = y2 * (1 - lamda);
            }
            else if( m_paretoParam.bound > 0 && x2 > m_paretoParam.bound )
            {
                y1 = 0.0;
                y2 = 0.0;
            }
            g.addLine(x1, x2, y1, y2);
        }
    }
    else if( m_curDistribution == EXPONENTIAL )
    {
        // CDF(x) = 1 - e ^ (- lamda * x) (x >= 0)
        // PDF(x) = lamda * e ^  (- lamda * x) (x >= 0)
        // mean = 1 / lumda
        int minX = 0;
        int maxX = qMax(1, (m_exponentialParam.mean * 3));

        LineGraph g(&painter, width, height - 10 - 4,
                    minX, maxX, 0.0, (m_exponentialParam.bound == 0) ?
                        1.0 / m_exponentialParam.mean :
                        1.0 / m_exponentialParam.mean / (1 - exp(-1.0 / m_exponentialParam.mean * m_exponentialParam.mean)));
        g.addBorder();

        g.addXScale(Qt::AlignLeft, minX);
        g.addXScale(Qt::AlignRight, maxX);

        g.addYScale(Qt::AlignBottom, 0.0, 3);
        g.addYScale(Qt::AlignTop, (m_exponentialParam.bound == 0) ?
                        1.0 / m_exponentialParam.mean :
                        1.0 / m_exponentialParam.mean / (1 - exp(-1.0 / m_exponentialParam.mean * m_exponentialParam.mean)), 3);

        double step = (maxX - minX) / 200.0;

        for(double x1 = 0; x1 < maxX; x1 += step)
        {
            double x2 = x1 + step;
            double y1 = 1.0 / m_exponentialParam.mean * exp(-1.0 / m_exponentialParam.mean * x1);
            double y2 = 1.0 / m_exponentialParam.mean * exp(-1.0 / m_exponentialParam.mean * x2);

            if( m_exponentialParam.bound > 0 && x2 < m_exponentialParam.bound )
            {
                double lamda = x1 / m_exponentialParam.bound;
                y1 /= 1 - exp(-1.0 / m_exponentialParam.mean * m_exponentialParam.bound);
                y2 /= 1 - exp(-1.0 / m_exponentialParam.mean * m_exponentialParam.bound);
                y1 = y1 * (1 - lamda);
                y2 = y2 * (1 - lamda);
            }
            else if( m_exponentialParam.bound > 0 && x2 > m_exponentialParam.bound )
            {
                y1 = 0.0;
                y2 = 0.0;
            }
            g.addLine(x1, x2, y1, y2);
        }
    }
}
