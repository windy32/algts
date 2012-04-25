#ifndef IPADDRDIALOG_H
#define IPADDRDIALOG_H

#include <QDialog>

namespace Ui {
class IpAddrDialog;
}

class IpAddrDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit IpAddrDialog(QWidget *parent = 0);
    ~IpAddrDialog();
    
private:
    Ui::IpAddrDialog *ui;
};

#endif // IPADDRDIALOG_H
