#include "ipaddrdialog.h"
#include "ui_ipaddrdialog.h"

IpAddrDialog::IpAddrDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IpAddrDialog)
{
    ui->setupUi(this);
    setLayout(ui->mainLayout);
}

IpAddrDialog::~IpAddrDialog()
{
    delete ui;
}
