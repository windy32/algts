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
};

#endif // DISTRIBUTIONDIALOG_H
