#ifndef DISTRIBUTIONDIALOG_H
#define DISTRIBUTIONDIALOG_H

#include <QDialog>

namespace Ui {
class DistributionDialog;
}

class DistributionDialog : public QDialog
{
    Q_OBJECT
        
private:
    Ui::DistributionDialog *ui;
    QString &m_distribution;

private slots:
    void onSliderUniformMin(int value);
    void onSliderUniformMax(int value);
    void onSliderParetoMean(int value);
    void onSliderParetoShape(int value);
    void onSliderParetoBound(int value);
    void onSliderExponentialMean(int value);
    void onSliderExponentialBound(int value);

    void onCancel();

    void updateText(int page);

public:
    explicit DistributionDialog(QString &distribution, QWidget *parent = 0);
    ~DistributionDialog();
};

#endif // DISTRIBUTIONDIALOG_H
