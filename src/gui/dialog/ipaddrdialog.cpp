#include "ipaddrdialog.h"
#include "ui_ipaddrdialog.h"

IpAddrDialog::IpAddrDialog(QString &interface, QList<QHostAddress> &addrs,
                           QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IpAddrDialog), m_model(NULL),
    m_interface(interface), m_addrs(addrs)
{
    ui->setupUi(this);
    setLayout(ui->mainLayout);

    // Init combo box
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    for(int i = 0; i < interfaces.size(); i++)
    {
        if( interfaces[i].addressEntries().size() == 0 )
        {
            continue;
        }
        ui->cmbInterface->addItem(interfaces[i].name());
    }

    // Init list view
    m_model = new QStandardItemModel;
    ui->lstAddress->setModel(m_model);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onOK()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(onCancel()));
    connect(ui->cmbInterface, SIGNAL(currentIndexChanged(int)),
            this, SLOT(updateAddrs()));
    updateAddrs();
}

IpAddrDialog::~IpAddrDialog()
{
    delete ui;
}

void IpAddrDialog::updateAddrs()
{
    m_interface = ui->cmbInterface->currentText();
    m_model->clear();

    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    for(int i = 0; i < interfaces.size(); i++)
    {
        if( interfaces[i].name() != m_interface )
            continue;

        QList<QNetworkAddressEntry> addrs = interfaces[i].addressEntries();
        for(int j = 0; j < addrs.size(); j++)
        {
            if( addrs[j].ip().protocol() == QAbstractSocket::IPv4Protocol )
            {
                QStandardItem *item = new QStandardItem(QString(addrs[j].ip().toString()));
                item->setCheckable(true);
                item->setCheckState(Qt::Checked);
                m_model->setItem(j, item);
            }
        }
    }
}

void IpAddrDialog::onOK()
{
    m_addrs.clear();

    for(int i = 0; i < m_model->rowCount(); i++)
    {
        QStandardItem *item = m_model->item(i, 0);
        if( item->checkState() == Qt::Checked )
        {
            m_addrs.append(QHostAddress(item->text()));
        }
    }
}

void IpAddrDialog::onCancel()
{
    m_interface = "";
    m_addrs.clear();
}
