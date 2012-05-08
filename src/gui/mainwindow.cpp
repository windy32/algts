#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QInputDialog>
#include <QMessageBox>

#include "dialog/ipaddrdialog.h"
#include "dialog/emulatordialog.h"

#include "globaldatabase.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Layouts
    ui->centralWidget->setLayout(ui->mainLayout);
    ui->pageWelcome->setLayout(ui->welcomeLayout);

    ui->pageScenario->setLayout(ui->scenarioLayout);
    ui->scenario->setLayout(ui->P21Layout);
    ui->user->setLayout(ui->P22Layout);
    ui->task->setLayout(ui->P23Layout);

    ui->P23BdTask->setLayout(ui->P231Layout);
    ui->P23BuTask->setLayout(ui->P232Layout);
    ui->P23OdTask->setLayout(ui->P233Layout);
    ui->P23TeTask->setLayout(ui->P234Layout);
    ui->P23AueTask->setLayout(ui->P235Layout);

    ui->pageScript->setLayout(ui->scriptLayout);
    ui->pageNewTest->setLayout(ui->newTestLayout);
    ui->pageViewTests->setLayout(ui->viewTestsLayout);

    ui->tabP5Script->setLayout(ui->P52Layout);
    ui->tabP5Rating->setLayout(ui->P53Layout);
    ui->P53CustomUnary->setLayout(ui->P533Layout);
    ui->P53CustomBinary->setLayout(ui->P534Layout);

    // Menus
    connect(ui->stakcList, SIGNAL(OnPageChanged(int)),
            ui->pages, SLOT(setCurrentIndex(int)));

    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));

    connect(ui->actionWelcome, SIGNAL(triggered()),this, SLOT(MenuWelcome()));
    connect(ui->actionScenario, SIGNAL(triggered()),
            this, SLOT(MenuScenario()));
    connect(ui->actionScript, SIGNAL(triggered()), this, SLOT(MenuScript()));
    connect(ui->actionNewTest, SIGNAL(triggered()), this, SLOT(MenuNewTest()));
    connect(ui->actionViewTests, SIGNAL(triggered()),
            this, SLOT(MenuViewTests()));

    ui->cmbP23Type->addItem("Bulk Download");
    ui->cmbP23Type->addItem("Bulk Upload");
    ui->cmbP23Type->addItem("On/Off Download");
    ui->cmbP23Type->addItem("TCP Echo");
    ui->cmbP23Type->addItem("Asynchronous UDP Echo");

    // Page 1: Welcome
    connect(ui->btnAddrSettings, SIGNAL(clicked()),
            this, SLOT(btnAddrSetting()));
    connect(ui->btnEmuSettings, SIGNAL(clicked()),
            this, SLOT(btnEmulationSetting()));
    m_txRate = -1;
    m_rxRate = -1;
    updateEmulationState();
    updateStatistics();

    // Page 3: Script
    m_model = new QStandardItemModel;
    m_model->setColumnCount(3);
    m_model->setHeaderData(0, Qt::Horizontal, QString("Line"));
    m_model->setHeaderData(1, Qt::Horizontal, QString("Position"));
    m_model->setHeaderData(2, Qt::Horizontal, QString("Default Value"));

    ui->lstP3Params->setModel(m_model);
    ui->lstP3Params->setColumnWidth(0, 60);
    ui->lstP3Params->setColumnWidth(1, 80);
    ui->lstP3Params->setColumnWidth(2, 172);

    connect(ui->btnP3New, SIGNAL(clicked()), this, SLOT(btnP3New()));
    connect(ui->btnP3Save, SIGNAL(clicked()), this, SLOT(btnP3Save()));
    connect(ui->btnP3Auto, SIGNAL(clicked()), this, SLOT(btnP3Auto()));
    connect(ui->btnP3AddSelected, SIGNAL(clicked()),
            this, SLOT(btnP3AddSelected()));
    connect(ui->btnP3DeleteSelected, SIGNAL(clicked()),
            this, SLOT(btnP3DelSelected()));
    connect(ui->rdoP3SetupScript, SIGNAL(clicked()),
            this, SLOT(rdoP3ResetScript()));
    connect(ui->rdoP3ResetScript, SIGNAL(clicked()),
            this, SLOT(rdoP3ResetScript()));
    connect(ui->cmbP3Scripts, SIGNAL(currentIndexChanged(int)),
            this, SLOT(cmbP3ScriptChanged(int)));
    connect(ui->txtP3Script, SIGNAL(textChanged()),
            this, SLOT(txtP3ScriptChanged()));
    connect(m_model, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(lstP3ParamsChanged()));
    m_curIndex = 0;
    m_modified = false;
    p3UpdateList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::MenuWelcome()
{
    ui->pages->setCurrentIndex(0);
    ui->stakcList->SetPage(0);
}

void MainWindow::MenuScenario()
{
    ui->pages->setCurrentIndex(1);
    ui->stakcList->SetPage(1);
}

void MainWindow::MenuScript()
{
    ui->pages->setCurrentIndex(2);
    ui->stakcList->SetPage(2);
}

void MainWindow::MenuNewTest()
{
    ui->pages->setCurrentIndex(3);
    ui->stakcList->SetPage(3);
}

void MainWindow::MenuViewTests()
{
    ui->pages->setCurrentIndex(4);
    ui->stakcList->SetPage(4);
}

void MainWindow::btnAddrSetting()
{
    IpAddrDialog dialog(m_interface, m_addrs);
    dialog.exec();

    if( m_interface != "" )
    {
        ui->ipAddrWidget->setAddress(m_interface, m_addrs);
    }
}

void MainWindow::btnEmulationSetting()
{
    EmulatorDialog dialog(m_txRate, m_rxRate);
    dialog.exec();
    updateEmulationState();
}

void MainWindow::updateEmulationState()
{
    if( m_txRate == -1 )
    {
        ui->lblTxRate->setText("TxRate: INFINITE");
    }
    else if( m_txRate >= 1024 )
    {
        ui->lblTxRate->setText(QString("TxRate: %1 Mbps").arg((double)m_txRate / 1024, 0, 'f', 2));
    }
    else
    {
        ui->lblTxRate->setText(QString("TxRate: %1 Kbps").arg(m_txRate));
    }

    if( m_rxRate == -1 )
    {
        ui->lblRxRate->setText("RxRate: INFINITE");
    }
    else if( m_rxRate >= 1024 )
    {
        ui->lblRxRate->setText(QString("RxRate: %1 Mbps").arg((double)m_rxRate / 1024, 0, 'f', 2));
    }
    else
    {
        ui->lblRxRate->setText(QString("RxRate: %1 Kbps").arg(m_rxRate));
    }
}

void MainWindow::updateStatistics()
{
    ui->lblScenarios->setText(
        QString("Scenarios: %1").arg(GlobalDatabase::instance()->getScenarioCount()));
    ui->lblScripts->setText(
        QString("Scripts: %1").arg(GlobalDatabase::instance()->getScriptCount()));
}

void MainWindow::btnP3New()
{
    bool ok;
    QString name = QInputDialog::getText(
                this, "Script", "Enter the name of the script",
                QLineEdit::Normal, "", &ok);
    if( ok && !name.isEmpty())
    {
        if( GlobalDatabase::instance()->existScript(name))
        {
            QMessageBox msgBox;
            msgBox.setText("The name exists.");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }
        else
        {
            Script newScript;
            newScript.name = name; // Other fields are left empty
            GlobalDatabase::instance()->addScript(newScript);

            p3UpdateList();
            ui->cmbP3Scripts->setCurrentIndex(ui->cmbP3Scripts->count() - 1);
            m_curIndex = ui->cmbP3Scripts->count() - 1;
        }
    }
    else if( ok && name.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText("The name cannot be empty.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }
}

void MainWindow::btnP3Save()
{
    // TODO
}

void MainWindow::btnP3Auto()
{

}

void MainWindow::btnP3AddSelected()
{

}

void MainWindow::btnP3DelSelected()
{

}

void MainWindow::rdoP3SetupScript()
{

}

void MainWindow::rdoP3ResetScript()
{

}

void MainWindow::cmbP3ScriptChanged(int index)
{
    m_curIndex = index;

    // Load script from database
    Script script;
    GlobalDatabase::instance()->getScript(m_curIndex, script);
    if( ui->rdoP3SetupScript->isChecked())
    {
        ui->txtP3Script->setText(script.setupText);
    }
    else
    {
        ui->txtP3Script->setText(script.resetText);
    }

    m_modified = false; // Reset modified after setting script text
    p3UpdateModifiedState();
}

void MainWindow::txtP3ScriptChanged()
{
    m_modified = true;
    p3UpdateModifiedState();
}

void MainWindow::lstP3ParamsChanged()
{
    m_modified = true;
    p3UpdateModifiedState();
}

void MainWindow::p3UpdateList()
{
    int count = GlobalDatabase::instance()->getScriptCount();
    Script script;

    // Clear the combo box
    ui->cmbP3Scripts->clear();

    if( count == 0 )
    {
        m_curIndex = -1; // nothing selected
    }

    // Insert items
    for(int i = 0; i < count; i++)
    {
        GlobalDatabase::instance()->getScript(i, script);
        ui->cmbP3Scripts->addItem(script.name);
    }
}

void MainWindow::p3UpdateModifiedState()
{
    for(int i = 0; i < ui->cmbP3Scripts->count(); i++)
    {
        if( m_curIndex == i && m_modified ) // Add modified indicator
        {
            if( !ui->cmbP3Scripts->itemText(i).endsWith(" *"))
            {
                ui->cmbP3Scripts->setItemText(
                            i, ui->cmbP3Scripts->itemText(i) + " *");
            }
        }
        else // Remove modified indicator
        {
            if( ui->cmbP3Scripts->itemText(i).endsWith(" *"))
            {
                ui->cmbP3Scripts->setItemText(i,
                            ui->cmbP3Scripts->itemText(i).left(
                                ui->cmbP3Scripts->itemText(i).size() - 2));
            }
        }
    }
}
