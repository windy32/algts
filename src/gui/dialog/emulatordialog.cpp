#include "emulatordialog.h"
#include "ui_emulatordialog.h"

EmulatorDialog::EmulatorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmulatorDialog)
{
    ui->setupUi(this);
    setLayout(ui->mainLayout);
}

EmulatorDialog::~EmulatorDialog()
{
    delete ui;
}
