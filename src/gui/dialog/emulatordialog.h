#ifndef EMULATORDIALOG_H
#define EMULATORDIALOG_H

#include <QDialog>

namespace Ui {
class EmulatorDialog;
}

class EmulatorDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EmulatorDialog(QWidget *parent = 0);
    ~EmulatorDialog();
    
private:
    Ui::EmulatorDialog *ui;
    bool m_infiniteTxRate;
    bool m_infiniteRxRate;

private slots:
    void onSliderTxRate();
    void onSliderRxRate();
    void onClearTxRate();
    void onClearRxRate();

    void onDefault();
    void onOK();
    void onCancel();

    void updateText();
};

#endif // EMULATORDIALOG_H
