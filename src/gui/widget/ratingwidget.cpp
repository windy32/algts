#include "ratingwidget.h"
#include "linegraph.h"

RatingWidget::RatingWidget(QWidget *parent) :
    QWidget(parent)
{
}

void RatingWidget::paintEvent(QPaintEvent */*event*/)
{
    int width = this->width();
    int height = this->height();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(Qt::white);

    // Draw widget bg
    painter.drawRect(0, 0, width - 1, height - 1);

    // Draw graph
    if( m_trace.contains("TxRate") &&
        m_trace.contains("MaxTxRate") &&
        m_trace.contains("Active"))
    {
        // Draw TxRate and MaxTxRate
        Q_ASSERT(m_trace["TxRate"].size() == m_trace["MaxTxRate"].size());
        Q_ASSERT(m_trace["MaxTxRate"].size() == m_trace["Active"].size());

        // Get max rate
        int maxRate = 0;
        for(int i = 0; i < m_trace["TxRate"].size(); i++)
        {
            maxRate = qMax(maxRate, m_trace["TxRate"][i]);
            maxRate = qMax(maxRate, m_trace["MaxTxRate"][i]);
        }

        // Init graph
        int minX = 0;
        int maxX = m_trace["Active"].size();
        double minY = 0;
        double maxY = (double)maxRate / (128.0 * 1000); // byte/s -> Mbps

        LineGraph g(&painter, width, height, 70, 10, minX, maxX, minY, maxY);
        g.addBorder();

        g.addXScaleAsTime(Qt::AlignLeft, minX);
        g.addXScaleAsTime(Qt::AlignRight, maxX);

        g.addYScale(Qt::AlignBottom, minY, " Mbps");
        g.addYScale(Qt::AlignTop, maxY, " Mbps");

        // Draw TxRate & MaxTxRate
        for(int i = 0; i < maxX - 1; i++)
        {
            g.addLine(i, i + 1,
                      m_trace["TxRate"][i] / (128.0 * 1000),
                      m_trace["TxRate"][i + 1] / (128.0 * 1000),
                      QColor::fromRgb(0x31, 0x77, 0xC1));
            g.addLine(i, i + 1,
                      m_trace["MaxTxRate"][i] / (128.0 * 1000),
                      m_trace["MaxTxRate"][i + 1] / (128.0 * 1000),
                      QColor::fromRgb(0xDF, 0x00, 0x24));
        }

        // Draw legend
        QList<QString> names;
        names.append("TxRate");
        names.append("MaxTxRate");

        QList<QColor> colors;
        colors.append(QColor::fromRgb(0x31, 0x77, 0xC1));
        colors.append(QColor::fromRgb(0xDF, 0x00, 0x24));

        g.addLegend(names, colors);
    }
    else if( m_trace.contains("RxRate") &&
             m_trace.contains("MaxRxRate") &&
             m_trace.contains("Active"))
    {
        // Draw TxRate and MaxTxRate
        Q_ASSERT(m_trace["RxRate"].size() == m_trace["MaxRxRate"].size());
        Q_ASSERT(m_trace["MaxRxRate"].size() == m_trace["Active"].size());

        // Get max rate
        int maxRate = 0;
        for(int i = 0; i < m_trace["RxRate"].size(); i++)
        {
            maxRate = qMax(maxRate, m_trace["RxRate"][i]);
            maxRate = qMax(maxRate, m_trace["MaxRxRate"][i]);
        }

        // Init graph
        int minX = 0;
        int maxX = m_trace["Active"].size();
        double minY = 0;
        double maxY = (double)maxRate / (128.0 * 1000); // byte/s -> Mbps

        LineGraph g(&painter, width, height, 70, 10, minX, maxX, minY, maxY);
        g.addBorder();

        g.addXScaleAsTime(Qt::AlignLeft, minX);
        g.addXScaleAsTime(Qt::AlignRight, maxX);

        g.addYScale(Qt::AlignBottom, minY, " Mbps");
        g.addYScale(Qt::AlignTop, maxY, " Mbps");

        // Draw TxRate & MaxTxRate
        for(int i = 0; i < maxX - 1; i++)
        {
            g.addLine(i, i + 1,
                      m_trace["RxRate"][i] / (128.0 * 1000),
                      m_trace["RxRate"][i + 1] / (128.0 * 1000),
                      QColor::fromRgb(0x31, 0x77, 0xC1));
            g.addLine(i, i + 1,
                      m_trace["MaxRxRate"][i] / (128.0 * 1000),
                      m_trace["MaxRxRate"][i + 1] / (128.0 * 1000),
                      QColor::fromRgb(0xDF, 0x00, 0x24));
        }

        // Draw legend
        QList<QString> names;
        names.append("RxRate");
        names.append("MaxRxRate");

        QList<QColor> colors;
        colors.append(QColor::fromRgb(0x31, 0x77, 0xC1));
        colors.append(QColor::fromRgb(0xDF, 0x00, 0x24));

        g.addLegend(names, colors);
    }
    else if( m_trace.contains("Delay") &&
            !m_trace.contains("Lost") &&
             m_trace.contains("Active"))
    {
        // Draw TxRate and MaxTxRate
        Q_ASSERT(m_trace["Delay"].size() == m_trace["Active"].size());

        // Get max delay
        int maxDelay = 0;
        for(int i = 0; i < m_trace["Delay"].size(); i++)
        {
            maxDelay = qMax(maxDelay, m_trace["Delay"][i]);
        }

        // Init graph
        int minX = 0;
        int maxX = m_trace["Active"].size();
        double minY = 0;
        double maxY = (double)maxDelay;

        LineGraph g(&painter, width, height, 54, 10, minX, maxX, minY, maxY);
        g.addBorder();

        g.addXScaleAsTime(Qt::AlignLeft, minX);
        g.addXScaleAsTime(Qt::AlignRight, maxX);

        g.addYScale(Qt::AlignBottom, minY, " ms", 0);
        g.addYScale(Qt::AlignTop, maxY, " ms", 0);

        // Draw Delay
        for(int i = 0; i < maxX - 1; i++)
        {
            g.addLine(i, i + 1, m_trace["Delay"][i], m_trace["Delay"][i + 1],
                      QColor::fromRgb(0x31, 0x77, 0xC1));
        }

        // Draw legend
        QList<QString> names;
        names.append("Delay");

        QList<QColor> colors;
        colors.append(QColor::fromRgb(0x31, 0x77, 0xC1));

        g.addLegend(names, colors);
    }
    else if( m_trace.contains("Delay") &&
             m_trace.contains("Lost") &&
             m_trace.contains("Active"))
    {
        // Draw TxRate and MaxTxRate
        Q_ASSERT(m_trace["Delay"].size() == m_trace["Lost"].size());
        Q_ASSERT(m_trace["Lost"].size() == m_trace["Active"].size());

        // Get max delay
        int maxDelay = 0;
        for(int i = 0; i < m_trace["Delay"].size(); i++)
        {
            maxDelay = qMax(maxDelay, m_trace["Delay"][i]);
        }

        // Init graph
        int minX = 0;
        int maxX = m_trace["Active"].size();
        double minY = 0;
        double maxY = (double)maxDelay;

        LineGraph g(&painter, width, height, 90, 10, minX, maxX, minY, maxY);
        g.addBorder();

        g.addXScaleAsTime(Qt::AlignLeft, minX);
        g.addXScaleAsTime(Qt::AlignRight, maxX);

        g.addYScaleText(Qt::AlignBottom, 0.0, 0, 0, QString("0 ms / 0%"));
        g.addYScaleText(Qt::AlignTop, 1.0, 0, 0, QString("%1 ms / 100%").arg(maxDelay));

        // Draw Delay
        for(int i = 0; i < maxX - 1; i++)
        {
            g.addLine(i, i + 1, m_trace["Delay"][i], m_trace["Delay"][i + 1],
                      QColor::fromRgb(0x31, 0x77, 0xC1));
            g.addLine(i, i + 1,
                      m_trace["Lost"][i] * maxY / 100.0,
                      m_trace["Lost"][i + 1] * maxY / 100.0,
                      QColor::fromRgb(0xDF, 0x00, 0x24));
        }

        // Draw legend
        QList<QString> names;
        names.append("Delay");
        names.append("Lost");

        QList<QColor> colors;
        colors.append(QColor::fromRgb(0x31, 0x77, 0xC1));
        colors.append(QColor::fromRgb(0xDF, 0x00, 0x24));

        g.addLegend(names, colors);
    }
}

void RatingWidget::setTrace(const RegularTraceItem &trace)
{
    m_trace = trace;
}

void RatingWidget::setRating(const QList<double> &rating)
{
    m_rating = rating;
}

void RatingWidget::hideRating()
{

}

void RatingWidget::showRating()
{

}
