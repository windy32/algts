#include "selectscenariodialog.h"
#include "ui_selectscenariodialog.h"

#include <QDialogButtonBox>
#include <QPushButton>

#include "globaldatabase.h"

SelectScenarioDialog::SelectScenarioDialog(ScenarioEx &scenario, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectScenarioDialog), m_scenario(scenario)
{
    ui->setupUi(this);
    setLayout(ui->mainLayout);

    m_scenarioIndex = 0;
    rebuildScenarioList();

    if( m_scenarioIndex == -1 ) // No scenario available
    {
        ui->scenarioWidget->setScenario(NULL);
        ui->scenarioWidget->update();
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
    else
    {
        GlobalDatabase::instance()->getScenario(0, m_scenario);
        ui->cmbScenarios->setCurrentIndex(0);
        ui->scenarioWidget->setScenario(&m_scenario);
        ui->scenarioWidget->update();
    }

    connect(ui->cmbScenarios, SIGNAL(currentIndexChanged(int)),
            this, SLOT(cmbScenarioChanged(int)));
    connect(ui->buttonBox, SIGNAL(accepted()),
            this, SLOT(onOK()));
}

SelectScenarioDialog::~SelectScenarioDialog()
{
    delete ui;
}

void SelectScenarioDialog::cmbScenarioChanged(int index)
{
    m_scenarioIndex = index;
    GlobalDatabase::instance()->getScenario(index, m_scenario);
    ui->scenarioWidget->setScenario(&m_scenario);
    ui->scenarioWidget->update();
}

void SelectScenarioDialog::rebuildScenarioList()
{
    int count = GlobalDatabase::instance()->getScenarioCount();
    ScenarioEx scenario;

    // Clear the combo box
    ui->cmbScenarios->clear();

    if( count == 0 )
    {
        m_scenarioIndex = -1; // nothing selected
    }

    // Insert items
    for(int i = 0; i < count; i++)
    {
        GlobalDatabase::instance()->getScenario(i, scenario);
        ui->cmbScenarios->addItem(scenario.name());
    }
}

void SelectScenarioDialog::onOK()
{
    accept();
}
