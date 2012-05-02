#ifndef PDFWIDGET_H
#define PDFWIDGET_H

#include <QWidget>

class PdfWidget : public QWidget
{
    Q_OBJECT
private:
    enum Distribution
    {
        NONE,
        UNIFORM,
        PARETO,
        EXPONENTIAL
    } m_curDistribution;

    struct UniformDistributionParameter
    {
        int min;
        int max;
    } m_uniformParam;

    struct ParetoDistributionParameter
    {
        int mean;
        double shape;
        int bound;
    } m_paretoParam;

    struct ExponentialDistributionParameter
    {
        int mean;
        int bound;
    } m_exponentialParam;

protected:
    void paintEvent(QPaintEvent *event);

public:
    explicit PdfWidget(QWidget *parent = 0);
    void setDistribution(const QString &distribution);

signals:
    
public slots:
    
};

#endif // PDFWIDGET_H
