#include "selectscenariodialog.h"
#include "ui_selectscenariodialog.h"

SelectScenarioDialog::SelectScenarioDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectScenarioDialog)
{
    ui->setupUi(this);
    setLayout(ui->mainLayout);
}

SelectScenarioDialog::~SelectScenarioDialog()
{
    delete ui;
}
