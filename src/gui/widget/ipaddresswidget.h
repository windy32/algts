#ifndef IPADDRESSWIDGET_H
#define IPADDRESSWIDGET_H

#include <QWidget>
#include <QHostAddress>

class IpAddressWidget : public QWidget
{
    Q_OBJECT
private:
    QString m_interface;
    QList<QHostAddress> m_addrs;

protected:
    void paintEvent(QPaintEvent */*event*/);

public:
    explicit IpAddressWidget(QWidget *parent = 0);
    void setAddress(const QString &interface, const QList<QHostAddress> &addrs);

signals:
    
public slots:
    
};

#endif // IPADDRESSWIDGET_H
