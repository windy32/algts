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

    // Page 2: Scenario
    ScenarioEx *s = new ScenarioEx(); // Test only
    s->name() = "My Scenario 1";
    s->addUser("Harry");
    s->addUser("Sally");
    s->addTask("Harry", new BulkDownloadTask(80));
    s->addTask("Harry", new BulkDownloadTask(80));
    s->addTask("Sally", new BulkDownloadTask(80));
    ui->scenarioWidget->setScenario(s);
    ui->scenarioWidget->update();

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
            this, SLOT(rdoP3ResetScript()));
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

    QRegExp rx("^([A-Z][A-Z0-9]*)( |)=( |)(.+)$");

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
