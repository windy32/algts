#ifndef PDFWIDGET_H
#define PDFWIDGET_H

#include <QWidget>

class PdfWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PdfWidget(QWidget *parent = 0);
    void setDistribution(QString distribution);

signals:
    
public slots:
    
};

#endif // PDFWIDGET_H
