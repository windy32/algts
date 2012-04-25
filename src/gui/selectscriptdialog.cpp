#include "selectscriptdialog.h"
#include "ui_selectscriptdialog.h"

SelectScriptDialog::SelectScriptDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectScriptDialog)
{
    ui->setupUi(this);
    setLayout(ui->mainLayout);
}

SelectScriptDialog::~SelectScriptDialog()
{
    delete ui;
}
