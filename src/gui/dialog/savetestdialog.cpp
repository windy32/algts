#include "savetestdialog.h"
#include "ui_savetestdialog.h"
#include <QPushButton>

SaveTestDialog::SaveTestDialog(TestResult &result, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveTestDialog), m_result(result)
{
    ui->setupUi(this);
    setLayout(ui->mainLayout);

    ui->txtScenario->setText(result.scenario.name());
    ui->txtScript->setText(result.script.name + result.script.description);
    ui->txtTime->setText(
                QDateTime::currentDateTime().toString("MMMM d, yyyy - hh:mm:ss"));
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(ui->txtPlatform, SIGNAL(textChanged(QString)),
            this, SLOT(onPlatformChanged()));
    connect(ui->txtDetail, SIGNAL(textChanged()),
            this, SLOT(onDetailChanged()));
}

SaveTestDialog::~SaveTestDialog()
{
    delete ui;
}

void SaveTestDialog::onPlatformChanged()
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
                ui->txtPlatform->text().trimmed() != "" &&
                ui->txtDetail->toPlainText().trimmed() != "");
}

void SaveTestDialog::onDetailChanged()
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
                ui->txtPlatform->text().trimmed() != "" &&
                ui->txtDetail->toPlainText().trimmed() != "");
}

void SaveTestDialog::accept()
{
    m_result.time = ui->txtTime->text();
    m_result.platform = ui->txtPlatform->text();
    m_result.detail = ui->txtDetail->toPlainText();

    QDialog::accept();
}
