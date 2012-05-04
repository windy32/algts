#ifndef CDFWIDGET_H
#define CDFWIDGET_H

#include <QWidget>

class CdfWidget : public QWidget
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
    void paintEvent(QPaintEvent * event);

public:
    explicit CdfWidget(QWidget *parent = 0);
    void setDistribution(const QString &distribution);
    
signals:
    
public slots:
    
};

#endif // CDFWIDGET_H
