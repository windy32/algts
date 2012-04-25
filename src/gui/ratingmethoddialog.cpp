#include "ratingmethoddialog.h"
#include "ui_ratingmethoddialog.h"

RatingMethodDialog::RatingMethodDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RatingMethodDialog)
{
    ui->setupUi(this);
    setLayout(ui->mainLayout);
    ui->customUnary->setLayout(ui->P3Layout);
    ui->customBinary->setLayout(ui->P4Layout);
}

RatingMethodDialog::~RatingMethodDialog()
{
    delete ui;
}
