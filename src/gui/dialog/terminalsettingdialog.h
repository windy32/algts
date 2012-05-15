#ifndef TERMINALSETTINGDIALOG_H
#define TERMINALSETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class TerminalSettingDialog;
}

class TerminalSettingDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::TerminalSettingDialog *ui;
    QString &m_routerAddr;
    QString &m_username;
    QString &m_password;

private slots:
    void onDefault();
    void updateButtonState();

protected:
    void accept();

public:
    explicit TerminalSettingDialog(
            QString &routerAddr, QString &username, QString &password,
            QWidget *parent = 0);
    ~TerminalSettingDialog();
    
};

#endif // TERMINALSETTINGDIALOG_H
