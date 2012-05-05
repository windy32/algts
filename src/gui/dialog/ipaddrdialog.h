#ifndef IPADDRDIALOG_H
#define IPADDRDIALOG_H

#include <QDialog>
#include <QNetworkInterface>
#include <QStandardItemModel>

namespace Ui {
class IpAddrDialog;
}

class IpAddrDialog : public QDialog
{
    Q_OBJECT
    
private:
    Ui::IpAddrDialog *ui;

    QStandardItemModel *m_model;
    QString &m_interface;
    QList<QHostAddress> &m_addrs;

public:
    explicit IpAddrDialog(QString &interface, QList<QHostAddress> &addrs,
                          QWidget *parent = 0);
    ~IpAddrDialog();
    
public slots:
    void updateAddrs();
    void onOK();
    void onCancel();
};

#endif // IPADDRDIALOG_H
