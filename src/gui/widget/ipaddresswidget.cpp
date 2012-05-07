#include "ipaddresswidget.h"

#include <QPainter>
#include <QNetworkInterface>

IpAddressWidget::IpAddressWidget(QWidget *parent) :
    QWidget(parent)
{
}

void IpAddressWidget::paintEvent(QPaintEvent */*event*/)
{
    const int leftMargin = 6;
    const int topMargin = 16;
    const int columnWidth = 120;
    const int rowHeight = 18;
    const int offsetCircleX = 4;
    const int offsetAddrX = 14;

    QFont ifFont("Sans", 9, QFont::Bold);
    QFont addrFont("Sans", 9);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Clear background
    painter.setPen(QColor::fromRgb(64, 64, 64));
    painter.setBrush(QColor::fromRgb(248, 248, 248));
    painter.drawRect(this->rect());

    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    for(int i = 0; i < interfaces.size(); i++)
    {
        // Draw interface
        painter.setPen(QColor::fromRgb(18, 98, 184));
        painter.setFont(ifFont);
        painter.drawText(leftMargin + columnWidth * i, topMargin,
                         interfaces[i].name());

        QList<QNetworkAddressEntry> addrs = interfaces[i].addressEntries();
        for(int j = 0; j < addrs.size(); j++)
        {
            if( addrs[j].ip().protocol() == QAbstractSocket::IPv4Protocol )
            {
                int baseX = leftMargin + columnWidth * i;
                int baseY = topMargin + rowHeight * (j + 1);

                // Draw circle (select state)
                if( m_interface == interfaces[i].name() &&
                    m_addrs.contains(addrs[j].ip()))
                {
                    painter.setBrush(Qt::black);
                }
                else
                {
                    painter.setBrush(Qt::lightGray);
                }
                painter.setPen(Qt::NoPen);
                painter.drawEllipse(baseX + offsetCircleX, baseY - 6, 4, 4);

                // Draw IPv4 address
                painter.setPen(Qt::black);
                painter.setFont(addrFont);
                painter.drawText(baseX + offsetAddrX, baseY,
                                 addrs[j].ip().toString());
            }
        }
    }
}

void IpAddressWidget::setAddress(const QString &interface, const QList<QHostAddress> &addrs)
{
    m_interface = interface;
    m_addrs = addrs;
    update();
}
