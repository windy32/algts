#ifndef DISTRIBUTIONDIALOG_H
#define DISTRIBUTIONDIALOG_H

#include <QDialog>

namespace Ui {
class DistributionDialog;
}

class DistributionDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit DistributionDialog(QWidget *parent = 0);
    ~DistributionDialog();
    
private:
    Ui::DistributionDialog *ui;

private slots:
    void onSliderUniformMin(int value);
    void onSliderUniformMax(int value);
    void onSliderParetoMean(int value);
    void onSliderParetoShape(int value);
    void onSliderParetoBound(int value);
    void onSliderExponentialMean(int value);
    void onSliderExponentialBound(int value);

    void updateText(int page);
};

#endif // DISTRIBUTIONDIALOG_H
