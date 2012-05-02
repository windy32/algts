#ifndef CDFWIDGET_H
#define CDFWIDGET_H

#include <QWidget>

class CdfWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CdfWidget(QWidget *parent = 0);
    void setDistribution(QString distribution);
    
signals:
    
public slots:
    
};

#endif // CDFWIDGET_H
