#include "pdfwidget.h"
#include <QPainter>

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
}

void PdfWidget::paintEvent(QPaintEvent *event)
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
                     "Probability Density Function");}
