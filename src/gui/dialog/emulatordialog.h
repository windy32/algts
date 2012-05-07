#ifndef EMULATORDIALOG_H
#define EMULATORDIALOG_H

#include <QDialog>

namespace Ui {
class EmulatorDialog;
}

class EmulatorDialog : public QDialog
{
    Q_OBJECT
        
private:
    Ui::EmulatorDialog *ui;
    bool m_infiniteTxRate;
    bool m_infiniteRxRate;
    int &m_txRate;
    int &m_rxRate;

private slots:
    void onSliderTxRate();
    void onSliderRxRate();
    void onClearTxRate();
    void onClearRxRate();

    void onDefault();
    void onOK();
    void onCancel();

    void updateText();

public:
    explicit EmulatorDialog(int &txRate, int &rxRate, QWidget *parent = 0);
    ~EmulatorDialog();
};

#endif // EMULATORDIALOG_H
