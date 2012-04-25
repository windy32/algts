#include "savetestdialog.h"
#include "ui_savetestdialog.h"

SaveTestDialog::SaveTestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveTestDialog)
{
    ui->setupUi(this);
    setLayout(ui->mainLayout);
}

SaveTestDialog::~SaveTestDialog()
{
    delete ui;
}
