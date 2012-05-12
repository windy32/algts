#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QInputDialog>
#include <QMessageBox>

#include "dialog/ipaddrdialog.h"
#include "dialog/emulatordialog.h"
#include "dialog/distributiondialog.h"
#include "dialog/selectscriptdialog.h"
#include "dialog/selectscenariodialog.h"
#include "globaldatabase.h"
#include "testthread.h"
#include "progressthread.h"

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

    // Page 2: Scenario
    m_scenarioIndex = 0;
    p2RebuildScenarioList();

    if( m_scenarioIndex == -1 ) // No scenario available
    {
        ui->scenarioWidget->setScenario(NULL);
        ui->scenarioWidget->update();
        ui->P2TargetProperty->setEnabled(false);
    }
    else
    {
        GlobalDatabase::instance()->getScenario(0, m_scenario);
        ui->cmbP2Scenarios->setCurrentIndex(0);
        ui->scenarioWidget->setScenario(&m_scenario);
        ui->scenarioWidget->update();
        p2ScenarioSelected();
    }

    connect(ui->cmbP2Scenarios, SIGNAL(currentIndexChanged(int)),
            this, SLOT(cmbP2ScenarioChanged(int)));
    connect(ui->btnP2New, SIGNAL(clicked()), this, SLOT(btnP2NewScenario()));
    connect(ui->btnP2Save, SIGNAL(clicked()), this, SLOT(btnP2SaveScenario()));

    connect(ui->txtP21Name, SIGNAL(textChanged(QString)),
            this, SLOT(txtP21NameChanged()));
    connect(ui->sldP21Length, SIGNAL(valueChanged(int)),
            this, SLOT(sldP21LengthChange()));
    connect(ui->sldP21Seed, SIGNAL(valueChanged(int)),
            this, SLOT(sldP21SeedChanged()));
    connect(ui->btnP21Random, SIGNAL(clicked()), this, SLOT(btnP21RandomSeed()));

    connect(ui->scenarioWidget, SIGNAL(scenarioSelected()),
            this, SLOT(p2ScenarioSelected()));
    connect(ui->scenarioWidget, SIGNAL(userSelected(QString)),
            this, SLOT(p2UserSelected(QString)));
    connect(ui->scenarioWidget, SIGNAL(taskSelected(QString,int)),
            this, SLOT(p2TaskSelected(QString,int)));

    connect(ui->scenarioWidget, SIGNAL(userDeleteClicked(QString)),
            this, SLOT(p2UserDeleted(QString)));
    connect(ui->scenarioWidget, SIGNAL(taskDeleteClicked(QString,int)),
            this, SLOT(p2TaskDeleted(QString,int)));

    connect(ui->scenarioWidget, SIGNAL(newUserClicked()),
            this, SLOT(p2NewUser()));
    connect(ui->scenarioWidget, SIGNAL(newTaskClicked(QString)),
            this, SLOT(p2NewTask(QString)));

    connect(ui->txtP22Name, SIGNAL(textChanged(QString)),
            this, SLOT(txtP22NameChanged(QString)));
    connect(ui->cmbP23Type, SIGNAL(currentIndexChanged(int)),
            this, SLOT(cmbP23TaskTypeChanged(int)));

    connect(ui->sldP231MaxBytes, SIGNAL(valueChanged(int)),
            this, SLOT(sldP231MaxBytesChanged(int)));
    connect(ui->sldP231MaxRate, SIGNAL(valueChanged(int)),
            this, SLOT(sldP231MaxRateChanged(int)));

    connect(ui->sldP232MaxBytes, SIGNAL(valueChanged(int)),
            this, SLOT(sldP232MaxBytesChanged(int)));
    connect(ui->sldP232MaxRate, SIGNAL(valueChanged(int)),
            this, SLOT(sldP232MaxRateChanged(int)));

    connect(ui->sldP233MaxRate, SIGNAL(valueChanged(int)),
            this, SLOT(sldP233MaxRateChanged(int)));
    connect(ui->sldP233PacketSize, SIGNAL(valueChanged(int)),
            this, SLOT(sldP233PacketSizeChanged(int)));
    connect(ui->sldP233RequestSize, SIGNAL(valueChanged(int)),
            this, SLOT(sldP233RequestSizeChanged(int)));
    connect(ui->btnP233OnTime, SIGNAL(clicked()), this, SLOT(btnP233OnTime()));
    connect(ui->btnP233OffTime, SIGNAL(clicked()), this, SLOT(btnP233OffTime()));

    connect(ui->btnP234InputSize, SIGNAL(clicked()), this, SLOT(btnP234InputSize()));
    connect(ui->btnP234EchoSize, SIGNAL(clicked()), this, SLOT(btnP234EchoSize()));
    connect(ui->btnP234Interval, SIGNAL(clicked()), this, SLOT(btnP234Interval()));

    connect(ui->btnP235InputSize, SIGNAL(clicked()), this, SLOT(btnP235InputSize()));
    connect(ui->btnP235EchoSize, SIGNAL(clicked()), this, SLOT(btnP235EchoSize()));
    connect(ui->btnP235Interval, SIGNAL(clicked()), this, SLOT(btnP235Interval()));

    // Page 3: Script
    m_setupModel = new QStandardItemModel;
    m_setupModel->setColumnCount(3);
    m_setupModel->setHeaderData(0, Qt::Horizontal, QString("Line"));
    m_setupModel->setHeaderData(1, Qt::Horizontal, QString("Position"));
    m_setupModel->setHeaderData(2, Qt::Horizontal, QString("Default Value"));

    m_resetModel = new QStandardItemModel;
    m_resetModel->setColumnCount(3);
    m_resetModel->setHeaderData(0, Qt::Horizontal, QString("Line"));
    m_resetModel->setHeaderData(1, Qt::Horizontal, QString("Position"));
    m_resetModel->setHeaderData(2, Qt::Horizontal, QString("Default Value"));

    ui->lstP3Params->setModel(m_setupModel);
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
            this, SLOT(rdoP3SetupScript()));
    connect(ui->rdoP3ResetScript, SIGNAL(clicked()),
            this, SLOT(rdoP3ResetScript()));
    connect(ui->cmbP3Scripts, SIGNAL(currentIndexChanged(int)),
            this, SLOT(cmbP3ScriptChanged(int)));
    connect(ui->txtP3Script, SIGNAL(textChanged()),
            this, SLOT(txtP3ScriptChanged()));
    connect(m_setupModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(lstP3ParamsChanged()));
    connect(m_resetModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(lstP3ParamsChanged()));

    m_curIndex = 0;
    m_modified = false;
    p3RebuildScriptList();

    // Page 4: New Test
    connect(ui->btnP4Scenario, SIGNAL(clicked()), this, SLOT(btnP4SelectScenario()));
    connect(ui->btnP4Script, SIGNAL(clicked()), this, SLOT(btnP4SelectScript()));
    connect(ui->btnP4Run, SIGNAL(clicked()), this, SLOT(btnP4Run()));
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

// Page 1: Welcome ////////////////////////////////////////////////////////////

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

// Page 2: Scenario ///////////////////////////////////////////////////////////
void MainWindow::cmbP2ScenarioChanged(int index)
{
    m_scenarioIndex = index;
    GlobalDatabase::instance()->getScenario(index, m_scenario);
    ui->scenarioWidget->setScenario(&m_scenario);
    ui->scenarioWidget->update();
    ui->P2TargetProperty->setEnabled(true);
    p2ScenarioSelected();
}

void MainWindow::btnP2NewScenario()
{
    bool ok;
    QString name = QInputDialog::getText(
                this, "algtsgui", "Enter the name of the scenario",
                QLineEdit::Normal, "", &ok);
    if( ok && !name.isEmpty())
    {
        if( GlobalDatabase::instance()->existScenario(name))
        {
            QMessageBox msgBox;
            msgBox.setText("The name exists.");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }
        else // Add new scenario
        {
            m_scenario = ScenarioEx();
            m_scenario.name() = name; // Other fields are left empty
            GlobalDatabase::instance()->addScenario(m_scenario);
            m_scenarioIndex = GlobalDatabase::instance()->getScenarioCount() - 1;

            // Update ui
            p2RebuildScenarioList();
            ui->scenarioWidget->setScenario(&m_scenario);
            ui->cmbP2Scenarios->setCurrentIndex(ui->cmbP2Scenarios->count() - 1);
            ui->P2TargetProperty->setEnabled(true);
            p2ScenarioSelected();
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

void MainWindow::btnP2SaveScenario()
{
    bool ok = true;

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);

    for(int i = 0; i < m_scenario.userCount(); i++)
    {
        QString username = m_scenario.user(i);
        for(int j = 0; j < m_scenario.taskCount(username); j++)
        {
            // Check tasks
            Task *task = m_scenario.task(username, j);
            if( task->getType() == Task::ONOFF_DOWNLOAD )
            {
                if( ((OnoffDownloadTask *)task)->getOnTime() == NullVariable::getInstance() ||
                    ((OnoffDownloadTask *)task)->getOffTime() == NullVariable::getInstance())
                {
                    msgBox.setText(QString("User %1's task %2 isn't completed.").arg(username).arg(j));
                    msgBox.exec();

                    // Quit checking
                    ok = false;
                    break;
                }
            }
            else if( task->getType() == Task::TCP_ECHO )
            {
                if( ((TcpEchoTask *)task)->getInputSize() == NullVariable::getInstance() ||
                    ((TcpEchoTask *)task)->getEchoSize() == NullVariable::getInstance() ||
                    ((TcpEchoTask *)task)->getInterval() == NullVariable::getInstance())
                {
                    msgBox.setText(QString("User %1's task %2 isn't completed.").arg(username).arg(j));
                    msgBox.exec();

                    // Quit checking
                    ok = false;
                    break;
                }
            }
            else if( task->getType() == Task::ASYNC_UDP_ECHO )
            {
                if( ((AsyncUdpEchoTask *)task)->getInputSize() == NullVariable::getInstance() ||
                    ((AsyncUdpEchoTask *)task)->getEchoSize() == NullVariable::getInstance() ||
                    ((AsyncUdpEchoTask *)task)->getInterval() == NullVariable::getInstance())
                {
                    msgBox.setText(QString("User %1's task %2 isn't completed.").arg(username).arg(j));
                    msgBox.exec();

                    // Quit checking
                    ok = false;
                    break;
                }
            }
        }
    }

    if( ok )
    {
        GlobalDatabase::instance()->setScenario(m_scenarioIndex, m_scenario);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(QString("\"%1\" is successfully saved.")
                       .arg(m_scenario.name()));
        msgBox.exec();
    }
}

void MainWindow::p2RebuildScenarioList()
{
    int count = GlobalDatabase::instance()->getScenarioCount();
    ScenarioEx scenario;
    // Clear the combo box
    ui->cmbP2Scenarios->clear();

    if( count == 0 )
    {
        m_scenarioIndex = -1; // nothing selected
    }

    // Insert items
    for(int i = 0; i < count; i++)
    {
        GlobalDatabase::instance()->getScenario(i, scenario);
        ui->cmbP2Scenarios->addItem(scenario.name());
    }
}

void MainWindow::txtP21NameChanged()
{
    m_scenario.name() = ui->txtP21Name->text();
    ui->scenarioWidget->update();
}

void MainWindow::sldP21LengthChange()
{
    m_scenario.length() = ui->sldP21Length->value();
    ui->scenarioWidget->update();
    ui->txtP21Length->setText(QString("%1").arg(ui->sldP21Length->value()));
}

void MainWindow::sldP21SeedChanged()
{
    m_scenario.seed() = ui->sldP21Length->value();
    ui->scenarioWidget->update();
    ui->txtP21Seed->setText(QString("%1").arg(ui->sldP21Seed->value()));
}

void MainWindow::btnP21RandomSeed()
{
    qsrand((uint)QTime::currentTime().msec());
    double randomValue = (double)qrand() / RAND_MAX;
    int seed = (int)(randomValue * 2000000000);
    ui->sldP21Seed->setValue(seed);
}

void MainWindow::p2ScenarioSelected()
{
    ui->P2TargetProperty->setCurrentIndex(0);

    ui->txtP21Name->setText(m_scenario.name());
    ui->sldP21Length->setValue(m_scenario.length());
    ui->sldP21Seed->setValue(m_scenario.seed());

    ui->txtP21Length->setText(QString("%1").arg(ui->sldP21Length->value()));
    ui->txtP21Seed->setText(QString("%1").arg(ui->sldP21Seed->value()));
}

void MainWindow::p2UserSelected(const QString &username)
{
    disconnect(ui->txtP22Name, SIGNAL(textChanged(QString)),
            this, SLOT(txtP22NameChanged(QString)));

    ui->P2TargetProperty->setCurrentIndex(1);
    ui->txtP22Name->setText(username);
    m_p2user = username;

    connect(ui->txtP22Name, SIGNAL(textChanged(QString)),
            this, SLOT(txtP22NameChanged(QString)));
}

void MainWindow::p2TaskSelected(const QString &username, int index)
{
    disconnect(ui->cmbP23Type, SIGNAL(currentIndexChanged(int)),
            this, SLOT(cmbP23TaskTypeChanged(int)));

    ui->P2TargetProperty->setCurrentIndex(2);

    m_p2user = username;
    m_p2index = index;
    Task *task = m_scenario.task(username, index);

    if( task->getType() == Task::BULK_DOWNLOAD )
    {
        int maxBytes = ((BulkDownloadTask *)task)->getMaxBytes();
        int maxRate = ((BulkDownloadTask *)task)->getMaxRate();

        ui->sldP231MaxBytes->setValue(
                    (maxBytes == -1) ? 32 * 1024 + 1 : maxBytes / (32 * 1024));
        ui->sldP231MaxRate->setValue(
                    (maxRate == -1) ? 32 * 100 + 1 : maxRate * 8 / (32 * 1024));

        ui->cmbP23Type->setCurrentIndex(0);
        ui->tasks->setCurrentIndex(0);
    }
    else if( task->getType() == Task::BULK_UPLOAD )
    {
        int maxBytes = ((BulkDownloadTask *)task)->getMaxBytes();
        int maxRate = ((BulkDownloadTask *)task)->getMaxRate();

        ui->sldP232MaxBytes->setValue(
                    (maxBytes == -1) ? 32 * 1024 + 1 : maxBytes / (32 * 1024));
        ui->sldP232MaxRate->setValue(
                    (maxRate == -1) ? 32 * 100 + 1 : maxRate * 8 / (32 * 1024));

        ui->cmbP23Type->setCurrentIndex(1);
        ui->tasks->setCurrentIndex(1);
    }
    else if( task->getType() == Task::ONOFF_DOWNLOAD )
    {
        int maxRate = ((OnoffDownloadTask *)task)->getMaxRate();
        int packetSize = ((OnoffDownloadTask *)task)->getPacketSize();
        int requestSize = ((OnoffDownloadTask *)task)->getRequestSize();
        RandomVariable *onTime = ((OnoffDownloadTask *)task)->getOnTime();
        RandomVariable *offTime = ((OnoffDownloadTask *)task)->getOffTime();

        ui->sldP233MaxRate->setValue(
                    (maxRate == -1) ? 32 * 100 + 1 : maxRate * 8 / (32 * 1024));
        ui->sldP233PacketSize->setValue(packetSize);
        ui->sldP233RequestSize->setValue(requestSize);
        ui->txtP233OnTime->setText(
                    onTime == NullVariable::getInstance() ? "" : onTime->toString());
        ui->txtP233OffTime->setText(
                    offTime == NullVariable::getInstance() ? "" : offTime->toString());

        sldP233PacketSizeChanged(packetSize);
        sldP233RequestSizeChanged(requestSize);

        ui->cmbP23Type->setCurrentIndex(2);
        ui->tasks->setCurrentIndex(2);
    }
    else if( task->getType() == Task::TCP_ECHO )
    {
        RandomVariable *inputSize = ((TcpEchoTask *)task)->getInputSize();
        RandomVariable *echoSize = ((TcpEchoTask *)task)->getEchoSize();
        RandomVariable *interval = ((TcpEchoTask *)task)->getInterval();

        ui->txtP234InputSize->setText(
                    inputSize == NullVariable::getInstance() ? "" : inputSize->toString());
        ui->txtP234EchoSize->setText(
                    echoSize == NullVariable::getInstance() ? "" : echoSize->toString());
        ui->txtP234Interval->setText(
                    interval == NullVariable::getInstance() ? "" : interval->toString());

        ui->cmbP23Type->setCurrentIndex(3);
        ui->tasks->setCurrentIndex(3);
    }
    else if( task->getType() == Task::ASYNC_UDP_ECHO )
    {
        RandomVariable *inputSize = ((AsyncUdpEchoTask *)task)->getInputSize();
        RandomVariable *echoSize = ((AsyncUdpEchoTask *)task)->getEchoSize();
        RandomVariable *interval = ((AsyncUdpEchoTask *)task)->getInterval();

        ui->txtP235InputSize->setText(
                    inputSize == NullVariable::getInstance() ? "" : inputSize->toString());
        ui->txtP235EchoSize->setText(
                    echoSize == NullVariable::getInstance() ? "" : echoSize->toString());
        ui->txtP235Interval->setText(
                    interval == NullVariable::getInstance() ? "" : interval->toString());

        ui->cmbP23Type->setCurrentIndex(4);
        ui->tasks->setCurrentIndex(4);
    }

    connect(ui->cmbP23Type, SIGNAL(currentIndexChanged(int)),
            this, SLOT(cmbP23TaskTypeChanged(int)));
}

void MainWindow::p2UserDeleted(const QString &username)
{
    QMessageBox msgBox;
    msgBox.setText(QString("Do you want to delete user %1?").arg(username));
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int ret = msgBox.exec();

    if( ret == QMessageBox::Yes )
    {
        m_scenario.deleteUser(username);
        ui->scenarioWidget->update();
    }
}

void MainWindow::p2TaskDeleted(const QString &username, int index)
{
    QMessageBox msgBox;
    msgBox.setText(QString("Do you want to delete task %1 of %2?").arg(index).arg(username));
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int ret = msgBox.exec();

    if( ret == QMessageBox::Yes )
    {
        m_scenario.deleteTask(username, index);
        ui->scenarioWidget->update();
    }
}

void MainWindow::p2NewUser()
{
    bool ok;
    QString name = QInputDialog::getText(
                this, "algtsgui", "Enter the name of the user",
                QLineEdit::Normal, "", &ok);
    if( ok && m_scenario.existUser(name))
    {
        QMessageBox msgBox;
        msgBox.setText(QString("User %1 exists.").arg(name));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }
    else if( ok )
    {
        m_scenario.insertUser(name);
        ui->scenarioWidget->update();
    }
}

void MainWindow::p2NewTask(const QString &username)
{
    m_scenario.addTask(username, new BulkDownloadTask(80));
    ui->scenarioWidget->update();
}


void MainWindow::txtP22NameChanged(const QString &newUsername)
{
    m_scenario.renameUser(m_p2user, newUsername);
    ui->scenarioWidget->update();
    ui->scenarioWidget->onUserSelected(newUsername);
    m_p2user = newUsername;
}

void MainWindow::cmbP23TaskTypeChanged(int index)
{
    Task *task =
            (index == 0) ? (Task *)new BulkDownloadTask(80) :
            (index == 1) ? (Task *)new BulkUploadTask(80) :
            (index == 2) ? (Task *)new OnoffDownloadTask(80) :
            (index == 3) ? (Task *)new TcpEchoTask(80) :
            /*(index == 4) ?*/ (Task *)new AsyncUdpEchoTask(80);

    m_scenario.replaceTask(m_p2user, m_p2index, task);

    p2TaskSelected(m_p2user, m_p2index);
}

void MainWindow::sldP231MaxBytesChanged(int value)
{
    int maxBytes = value * 32; // KB
    if( maxBytes < 1024 )
    {
        ui->txtP231MaxBytes->setText(QString("%1 KB").arg(maxBytes));
    }
    else if( maxBytes < 1024 * 1024 )
    {
        ui->txtP231MaxBytes->setText(QString("%1 MB").arg(maxBytes / 1024));
    }
    else if( maxBytes == 1024 * 1024 )
    {
        ui->txtP231MaxBytes->setText("1 GB");
    }
    else
    {
        ui->txtP231MaxBytes->setText("INFINITE");
    }

    BulkDownloadTask *task = (BulkDownloadTask *)m_scenario.task(m_p2user, m_p2index);
    task->setMaxBytes(maxBytes * 1024);
}

void MainWindow::sldP231MaxRateChanged(int value)
{
    int maxRate = value * 32; // Kbps
    if( maxRate < 1024 )
    {
        ui->txtP231MaxRate->setText(QString("%1 Kbps").arg(maxRate));
    }
    else if( maxRate < 100 * 1024)
    {
        ui->txtP231MaxRate->setText(QString("%1 Mbps")
                                    .arg(maxRate / 1024.0, 0, 'f', 2));
    }
    else if( maxRate == 100 * 1024 )
    {
        ui->txtP231MaxRate->setText("100 Mbps");
    }
    else
    {
        ui->txtP231MaxRate->setText("INFINITE");
    }

    BulkDownloadTask *task = (BulkDownloadTask *)m_scenario.task(m_p2user, m_p2index);
    task->setMaxRate(maxRate * 1024 / 8);
}

void MainWindow::sldP232MaxBytesChanged(int value)
{
    int maxBytes = value * 32; // KB
    if( maxBytes < 1024 )
    {
        ui->txtP232MaxBytes->setText(QString("%1 KB").arg(maxBytes));
    }
    else if( maxBytes < 1024 * 1024 )
    {
        ui->txtP232MaxBytes->setText(QString("%1 MB").arg(maxBytes / 1024));
    }
    else if( maxBytes == 1024 * 1024 )
    {
        ui->txtP232MaxBytes->setText("1 GB");
    }
    else
    {
        ui->txtP232MaxBytes->setText("INFINITE");
    }

    BulkUploadTask *task = (BulkUploadTask *)m_scenario.task(m_p2user, m_p2index);
    task->setMaxBytes(maxBytes * 1024);
}

void MainWindow::sldP232MaxRateChanged(int value)
{
    int maxRate = value * 32; // Kbps
    if( maxRate < 1024 )
    {
        ui->txtP232MaxRate->setText(QString("%1 Kbps").arg(maxRate));
    }
    else if( maxRate < 100 * 1024)
    {
        ui->txtP232MaxRate->setText(QString("%1 Mbps")
                                    .arg(maxRate / 1024.0, 0, 'f', 2));
    }
    else if( maxRate == 100 * 1024 )
    {
        ui->txtP232MaxRate->setText("100 Mbps");
    }
    else
    {
        ui->txtP232MaxRate->setText("INFINITE");
    }

    BulkUploadTask *task = (BulkUploadTask *)m_scenario.task(m_p2user, m_p2index);
    task->setMaxRate(maxRate * 1024 / 8);
}

void MainWindow::sldP233MaxRateChanged(int value)
{
    int maxRate = value * 32; // Kbps
    if( maxRate < 1024 )
    {
        ui->txtP233MaxRate->setText(QString("%1 Kbps").arg(maxRate));
    }
    else if( maxRate < 100 * 1024)
    {
        ui->txtP233MaxRate->setText(QString("%1 Mbps")
                                    .arg(maxRate / 1024.0, 0, 'f', 2));
    }
    else if( maxRate == 100 * 1024 )
    {
        ui->txtP233MaxRate->setText("100 Mbps");
    }
    else
    {
        ui->txtP233MaxRate->setText("INFINITE");
    }

    OnoffDownloadTask *task = (OnoffDownloadTask *)m_scenario.task(m_p2user, m_p2index);
    task->setMaxRate(maxRate * 1024 / 8);
}

void MainWindow::sldP233PacketSizeChanged(int value)
{
    ui->txtP233PacketSize->setText(QString("%1 Bytes").arg(value));

    OnoffDownloadTask *task = (OnoffDownloadTask *)m_scenario.task(m_p2user, m_p2index);
    task->setPacketSize(value);
}

void MainWindow::sldP233RequestSizeChanged(int value)
{
    ui->txtP233RequestSize->setText(QString("%1 Bytes").arg(value));

    OnoffDownloadTask *task = (OnoffDownloadTask *)m_scenario.task(m_p2user, m_p2index);
    task->setRequestSize(value);
}

void MainWindow::btnP233OnTime()
{
    QString onTime;
    DistributionDialog dialog(onTime);
    dialog.exec();

    if( onTime != "" ) // User didn't click Cancel
    {
        OnoffDownloadTask *task = (OnoffDownloadTask *)m_scenario.task(m_p2user, m_p2index);
        task->setAttribute("OnTime", onTime);
        ui->txtP233OnTime->setText(onTime);
    }
}

void MainWindow::btnP233OffTime()
{
    QString offTime;
    DistributionDialog dialog(offTime);
    dialog.exec();

    if( offTime != "" ) // User didn't click Cancel
    {
        OnoffDownloadTask *task = (OnoffDownloadTask *)m_scenario.task(m_p2user, m_p2index);
        task->setAttribute("OffTime", offTime);
        ui->txtP233OffTime->setText(offTime);
    }
}

void MainWindow::btnP234InputSize()
{
    QString inputSize;
    DistributionDialog dialog(inputSize);
    dialog.exec();

    if( inputSize != "" ) // User didn't click Cancel
    {
        TcpEchoTask *task = (TcpEchoTask *)m_scenario.task(m_p2user, m_p2index);
        task->setAttribute("InputSize", inputSize);
        ui->txtP234InputSize->setText(inputSize);
    }
}

void MainWindow::btnP234EchoSize()
{
    QString echoSize;
    DistributionDialog dialog(echoSize);
    dialog.exec();

    if( echoSize != "" ) // User didn't click Cancel
    {
        TcpEchoTask *task = (TcpEchoTask *)m_scenario.task(m_p2user, m_p2index);
        task->setAttribute("EchoSize", echoSize);
        ui->txtP234EchoSize->setText(echoSize);
    }
}

void MainWindow::btnP234Interval()
{
    QString interval;
    DistributionDialog dialog(interval);
    dialog.exec();

    if( interval != "" ) // User didn't click Cancel
    {
        TcpEchoTask *task = (TcpEchoTask *)m_scenario.task(m_p2user, m_p2index);
        task->setAttribute("Interval", interval);
        ui->txtP234Interval->setText(interval);
    }
}

void MainWindow::btnP235InputSize()
{
    QString inputSize;
    DistributionDialog dialog(inputSize);
    dialog.exec();

    if( inputSize != "" ) // User didn't click Cancel
    {
        TcpEchoTask *task = (TcpEchoTask *)m_scenario.task(m_p2user, m_p2index);
        task->setAttribute("InputSize", inputSize);
        ui->txtP235InputSize->setText(inputSize);
    }
}

void MainWindow::btnP235EchoSize()
{
    QString echoSize;
    DistributionDialog dialog(echoSize);
    dialog.exec();

    if( echoSize != "" ) // User didn't click Cancel
    {
        TcpEchoTask *task = (TcpEchoTask *)m_scenario.task(m_p2user, m_p2index);
        task->setAttribute("EchoSize", echoSize);
        ui->txtP235EchoSize->setText(echoSize);
    }
}

void MainWindow::btnP235Interval()
{
    QString interval;
    DistributionDialog dialog(interval);
    dialog.exec();

    if( interval != "" ) // User didn't click Cancel
    {
        TcpEchoTask *task = (TcpEchoTask *)m_scenario.task(m_p2user, m_p2index);
        task->setAttribute("Interval", interval);
        ui->txtP235Interval->setText(interval);
    }
}

// Page 3: Script /////////////////////////////////////////////////////////////

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
        else // Add new script
        {
            Script newScript;
            newScript.name = name; // Other fields are left empty
            GlobalDatabase::instance()->addScript(newScript);

            p3RebuildScriptList();
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
    m_curScript.setupParams.clear();
    m_curScript.resetParams.clear();

    if( m_textState == HtmlText )
    {
        for(int i = 0; i < m_setupModel->rowCount(); i++)
        {
            qDebug() << "Setup " << i << " / " << m_setupModel->rowCount();
            m_curScript.setupParams.append(
                        ScriptParam(m_setupModel->item(i, 0)->text().toInt(),
                                    m_setupModel->item(i, 1)->text().toInt(),
                                    m_setupModel->item(i, 2)->text()));
        }
        for(int i = 0; i < m_resetModel->rowCount(); i++)
        {
            qDebug() << "Setup " << i << " / " << m_resetModel->rowCount();
            m_curScript.resetParams.append(
                        ScriptParam(m_resetModel->item(i, 0)->text().toInt(),
                                    m_resetModel->item(i, 1)->text().toInt(),
                                    m_resetModel->item(i, 2)->text()));
        }
    }

    GlobalDatabase::instance()->setScript(
                ui->cmbP3Scripts->currentIndex(), m_curScript);

    m_modified = false;
    p3UpdateModifiedState();
}

void MainWindow::btnP3Auto()
{
    disconnect(ui->txtP3Script, SIGNAL(textChanged()),
            this, SLOT(txtP3ScriptChanged()));
    disconnect(m_setupModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(lstP3ParamsChanged()));
    disconnect(m_resetModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(lstP3ParamsChanged()));

    QString &script = ui->rdoP3SetupScript->isChecked() ?
                m_curScript.setupText : m_curScript.resetText;
    QStringList lines = script.split('\n');
    QStandardItemModel *model = ui->rdoP3SetupScript->isChecked() ?
                m_setupModel : m_resetModel;
    model->removeRows(0, model->rowCount());

    QRegExp rx("^([A-Z][A-Za-z0-9]*)( |)=( |)(.+)$");

    int count = 0;
    for(int i = 0; i < lines.size(); i++)
    {
        int pos = rx.indexIn(lines[i]);
        if( pos != -1 )
        {
            QString value = rx.cap(4);
            QStandardItem *itemLine = new QStandardItem(QString("%1").arg(i));
            QStandardItem *itemPos = new QStandardItem(
                        QString("%1").arg(
                            pos + rx.cap(1).size() + rx.cap(2).size() +
                            1 + rx.cap(3).size()));
            itemLine->setEditable(false);
            itemPos->setEditable(false);

            model->setItem(count, 0, itemLine);
            model->setItem(count, 1, itemPos);
            model->setItem(count++, 2, new QStandardItem(QString("%1").arg(value)));

            lines[i] = lines[i].left(lines[i].size() - value.size());
        }
    }
    script = lines.join("\n");

    m_textState = HtmlText;
    p3UpdateScript();

    connect(ui->txtP3Script, SIGNAL(textChanged()),
            this, SLOT(txtP3ScriptChanged()));
    connect(m_setupModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(lstP3ParamsChanged()));
    connect(m_resetModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(lstP3ParamsChanged()));
}

void MainWindow::btnP3AddSelected()
{
    // This button is temporarily unavailable
}

void MainWindow::btnP3DelSelected()
{
    // This button is temporarily unavailable
}

void MainWindow::rdoP3SetupScript()
{
    disconnect(ui->txtP3Script, SIGNAL(textChanged()),
            this, SLOT(txtP3ScriptChanged()));
    disconnect(m_setupModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(lstP3ParamsChanged()));
    disconnect(m_resetModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(lstP3ParamsChanged()));

    p3UpdateScript();
    p3UpdateParamList();

    connect(ui->txtP3Script, SIGNAL(textChanged()),
            this, SLOT(txtP3ScriptChanged()));
    connect(m_setupModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(lstP3ParamsChanged()));
    connect(m_resetModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(lstP3ParamsChanged()));
}

void MainWindow::rdoP3ResetScript()
{
    disconnect(ui->txtP3Script, SIGNAL(textChanged()),
            this, SLOT(txtP3ScriptChanged()));
    disconnect(m_setupModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(lstP3ParamsChanged()));
    disconnect(m_resetModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(lstP3ParamsChanged()));

    p3UpdateScript();
    p3UpdateParamList();

    connect(ui->txtP3Script, SIGNAL(textChanged()),
            this, SLOT(txtP3ScriptChanged()));
    connect(m_setupModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(lstP3ParamsChanged()));
    connect(m_resetModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(lstP3ParamsChanged()));
}

void MainWindow::cmbP3ScriptChanged(int index)
{
    disconnect(m_setupModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(lstP3ParamsChanged()));
    disconnect(m_resetModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(lstP3ParamsChanged()));
    disconnect(ui->txtP3Script, SIGNAL(textChanged()),
            this, SLOT(txtP3ScriptChanged()));

    // Load script from database
    m_curIndex = index;
    GlobalDatabase::instance()->getScript(m_curIndex, m_curScript);

    // Update models
    m_setupModel->removeRows(0, m_setupModel->rowCount());
    m_resetModel->removeRows(0, m_resetModel->rowCount());

    for(int i = 0; i < m_curScript.setupParams.size(); i++)
    {
        QStandardItem *itemLine = new QStandardItem(
                    QString("%1").arg(m_curScript.setupParams[i].line));
        QStandardItem *itemPos = new QStandardItem(
                    QString("%1").arg(m_curScript.setupParams[i].pos));
        itemLine->setEditable(false);
        itemPos->setEditable(false);

        m_setupModel->setItem(i, 0, itemLine);
        m_setupModel->setItem(i, 1, itemPos);
        m_setupModel->setItem(i, 2, new QStandardItem(
                                  m_curScript.setupParams[i].defValue));
    }

    for(int i = 0; i < m_curScript.resetParams.size(); i++)
    {
        QStandardItem *itemLine = new QStandardItem(
                    QString("%1").arg(m_curScript.resetParams[i].line));
        QStandardItem *itemPos = new QStandardItem(
                    QString("%1").arg(m_curScript.resetParams[i].pos));
        itemLine->setEditable(false);
        itemPos->setEditable(false);

        m_resetModel->setItem(i, 0, itemLine);
        m_resetModel->setItem(i, 1, itemPos);
        m_resetModel->setItem(i, 2, new QStandardItem(
                                  m_curScript.resetParams[i].defValue));
    }

    m_textState = HtmlText;

    p3UpdateScript();
    p3UpdateParamList();

    ui->lstP3Params->setModel(ui->rdoP3SetupScript->isChecked() ?
                                  m_setupModel : m_resetModel);

    // Set unmodified
    m_modified = false;
    p3UpdateModifiedState();

    connect(m_setupModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(lstP3ParamsChanged()));
    connect(m_resetModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(lstP3ParamsChanged()));
    connect(ui->txtP3Script, SIGNAL(textChanged()),
            this, SLOT(txtP3ScriptChanged()));
}

void MainWindow::txtP3ScriptChanged()
{
    m_modified = true;
    p3UpdateModifiedState();

    // [State 1: Plain Text] ---------------------------> [State 1: Plain Text]
    if( m_textState == PlainText ) // Update script object
    {
        if( ui->rdoP3SetupScript->isChecked())
        {
            m_curScript.setupText = ui->txtP3Script->toPlainText();
        }
        else
        {
            m_curScript.resetText = ui->txtP3Script->toPlainText();
        }
    }
    // [State 2: HTML Text] ----------------------------> [State 1: Plain Text]
    else if( m_textState == HtmlText )
    {
        disconnect(m_setupModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
                this, SLOT(lstP3ParamsChanged()));
        disconnect(m_resetModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
                this, SLOT(lstP3ParamsChanged()));
        disconnect(ui->txtP3Script, SIGNAL(textChanged()),
                this, SLOT(txtP3ScriptChanged()));

        if( ui->rdoP3SetupScript->isChecked())
        {
            m_curScript.setupText = ui->txtP3Script->toPlainText();
            m_setupModel->removeRows(0, m_setupModel->rowCount());
        }
        else
        {
            m_curScript.resetText = ui->txtP3Script->toPlainText();
            m_resetModel->removeRows(0, m_resetModel->rowCount());
        }
        m_textState = PlainText;
        p3UpdateScript();

        connect(m_setupModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
                this, SLOT(lstP3ParamsChanged()));
        connect(m_resetModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
                this, SLOT(lstP3ParamsChanged()));
        connect(ui->txtP3Script, SIGNAL(textChanged()),
                this, SLOT(txtP3ScriptChanged()));
    }
    ui->btnP3Auto->setEnabled(!ui->txtP3Script->toPlainText().isEmpty());
}

void MainWindow::lstP3ParamsChanged()
{
    m_modified = true;
    p3UpdateModifiedState();

    disconnect(ui->txtP3Script, SIGNAL(textChanged()),
            this, SLOT(txtP3ScriptChanged()));
    p3UpdateScript();
    connect(ui->txtP3Script, SIGNAL(textChanged()),
            this, SLOT(txtP3ScriptChanged()));
}

void MainWindow::p3RebuildScriptList()
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

void MainWindow::p3UpdateScript()
{
    QString &scriptText = ui->rdoP3SetupScript->isChecked() ?
                m_curScript.setupText : m_curScript.resetText;
    if( m_textState == PlainText )
    {
        ui->txtP3Script->setPlainText(scriptText);
    }
    else if( m_textState == HtmlText ) // Insert parameters with bgcolor
    {
        QStringList lines = scriptText.split('\n');
        QStandardItemModel *model = ui->rdoP3SetupScript->isChecked() ?
                    m_setupModel : m_resetModel;
        QString htmlBuffer;

        int paramIndex = 0;
        for(int i = 0; i < lines.size(); i++) // It's assumed that each line contains not more than one parameter
        {
            if( paramIndex < model->rowCount())
            {
                int paramLine = model->item(paramIndex, 0)->text().toInt();
                int paramPos = model->item(paramIndex, 1)->text().toInt();
                QString paramValue = model->item(paramIndex, 2)->text();

                if( i == paramLine )
                {
                    htmlBuffer += lines[i].left(paramPos);
                    htmlBuffer +=
                            QString("<span style='background-color: #EEEEEE'>%1</span>").arg(paramValue);
                    htmlBuffer += lines[i].right(lines[i].size() - paramPos);
                    htmlBuffer += "<br>\n";

                    paramIndex += 1;
                }
                else
                {
                    htmlBuffer += lines[i] + "<br>\n";
                }
            }
            else
            {
                htmlBuffer += lines[i] + "<br>\n";
            }
        }

        // Remove the extra newline and display
        htmlBuffer =  htmlBuffer.left(htmlBuffer.size() - strlen("<br>\n"));
        ui->txtP3Script->setHtml(htmlBuffer);
    }
}

void MainWindow::p3UpdateParamList()
{
    ui->lstP3Params->setModel(ui->rdoP3SetupScript->isChecked() ?
                                  m_setupModel : m_resetModel);
}

// Page 4: New Test ///////////////////////////////////////////////////////////

void MainWindow::btnP4SelectScenario()
{
    SelectScenarioDialog dialog(m_p4test.scenario);
    if( dialog.exec() == QDialog::Accepted )
    {
        ui->txtP4Scenario->setText(m_p4test.scenario.name());
    }

    if( ui->txtP4Scenario->text() != "" && ui->txtP4Script->text() != "")
    {
        ui->btnP4Run->setEnabled(true);
    }
}

void MainWindow::btnP4SelectScript()
{
    SelectScriptDialog dialog(m_p4test.script);
    if( dialog.exec() == QDialog::Accepted )
    {
        ui->txtP4Script->setText(m_p4test.script.name + m_p4test.script.description);
    }

    if( ui->txtP4Scenario->text() != "" && ui->txtP4Script->text() != "")
    {
        ui->btnP4Run->setEnabled(true);
    }
}

void MainWindow::btnP4Run()
{
    QString serverAddr = ui->txtP4ServerAddress->text();
    QString serverPort = ui->txtP4ServerPort->text();
    QHostAddress addr;
    quint16 port;

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);

    // Check ip address
    QRegExp rx("^(0|[1-9][0-9]*)\\.(0|[1-9][0-9]*)\\."
                "(0|[1-9][0-9]*)\\.(0|[1-9][0-9]*)$");
    if( rx.indexIn(serverAddr) == -1 )
    {
        msgBox.setText("Invalid server address");
        msgBox.exec();
        return;
    }
    if( rx.cap(1).toUInt() > 255 ||
        rx.cap(2).toUInt() > 255 ||
        rx.cap(3).toUInt() > 255 ||
        rx.cap(4).toUInt() > 255 )
    {
        msgBox.setText("Invalid server address");
        msgBox.exec();
        return;
    }
    addr = QHostAddress(serverAddr);

    // Check port
    bool ok;
    port = serverPort.toUShort(&ok);
    if( !ok )
    {
        msgBox.setText("Invalid server port");
        msgBox.exec();
        return;
    }
    else if( port < 1024 )
    {
        msgBox.setText("Invalid server port");
        msgBox.exec();
        return;
    }

    TestThread *testThread = new TestThread(
                m_addrs, addr, port, &m_p4test.scenario, this);
    testThread->start();

    // Should be fixed later
    ProgressThread progressThread(ui->pgsP4, m_p4test.scenario.length(), this);
    progressThread.start();

    // TODO: redirect the log messages to the log textbox
}
