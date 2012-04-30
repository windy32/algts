#include "distributiondialog.h"
#include "ui_distributiondialog.h"

DistributionDialog::DistributionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DistributionDialog)
{
    ui->setupUi(this);
    setLayout(ui->mainLayout);
    ui->tabUniform->setLayout(ui->P1Layout);
    ui->tabPareto->setLayout(ui->P2Layout);
    ui->tabExponential->setLayout(ui->P3Layout);
}

DistributionDialog::~DistributionDialog()
{
    delete ui;
}
