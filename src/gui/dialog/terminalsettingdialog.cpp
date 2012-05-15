#include "terminalsettingdialog.h"
#include "ui_terminalsettingdialog.h"

#include <QDebug>

TerminalSettingDialog::TerminalSettingDialog(QString &routerAddr, QString &username, QString &password, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TerminalSettingDialog),
    m_routerAddr(routerAddr), m_username(username), m_password(password)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(ui->btnDefault, SIGNAL(clicked()),
            this, SLOT(onDefault()));
    connect(ui->txtRouterAddr, SIGNAL(textChanged(QString)),
            this, SLOT(updateButtonState()));
    connect(ui->txtUsername, SIGNAL(textChanged(QString)),
            this, SLOT(updateButtonState()));
    connect(ui->txtPassword, SIGNAL(textChanged(QString)),
            this, SLOT(updateButtonState()));
}

TerminalSettingDialog::~TerminalSettingDialog()
{
    delete ui;
}

void TerminalSettingDialog::onDefault()
{
    ui->txtRouterAddr->setText("172.16.0.1");
    ui->txtUsername->setText("root");
    ui->txtPassword->setText("admin");
}

void TerminalSettingDialog::updateButtonState()
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
                ui->txtRouterAddr->text() != "" &&
                ui->txtUsername->text() != "" &&
                ui->txtPassword->text() != "" );
}

void TerminalSettingDialog::accept()
{
    m_routerAddr = ui->txtRouterAddr->text();
    m_username = ui->txtUsername->text();
    m_password = ui->txtPassword->text();

    QDialog::accept();
}
