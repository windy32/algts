#include "selectscriptdialog.h"
#include "ui_selectscriptdialog.h"

SelectScriptDialog::SelectScriptDialog(ScriptData &script,
                                       QWidget *parent)
    : QDialog(parent), ui(new Ui::SelectScriptDialog),
      m_script(script)
{
    ui->setupUi(this);
    setLayout(ui->mainLayout);

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

    ui->lstParams->setModel(m_setupModel);
    ui->lstParams->setColumnWidth(0, 60);
    ui->lstParams->setColumnWidth(1, 80);
    ui->lstParams->setColumnWidth(2, 120);

    connect(ui->rdoSetupScript, SIGNAL(clicked()),
            this, SLOT(rdoSetupScript()));
    connect(ui->rdoResetScript, SIGNAL(clicked()),
            this, SLOT(rdoResetScript()));
    connect(m_setupModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(lstParamsChanged()));
    connect(m_resetModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(lstParamsChanged()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onOK()));

    m_curIndex = 0;
    rebuildScriptList();

    connect(ui->cmbScript, SIGNAL(currentIndexChanged(int)),
            this, SLOT(cmbScriptChanged(int)));

    if( ui->cmbScript->count() > 0 )
    {
        cmbScriptChanged(0);
    }

    // Make sure that m_setupText and m_resetText are initialized properly
    // Should be optimized later
    ui->rdoResetScript->setChecked(true);
    updateScript();
    ui->rdoSetupScript->setChecked(true);
    updateScript();
}

SelectScriptDialog::~SelectScriptDialog()
{
    delete ui;
}

void SelectScriptDialog::rdoSetupScript()
{
    updateScript();
    updateParamList();
}

void SelectScriptDialog::rdoResetScript()
{
    updateScript();
    updateParamList();
}

void SelectScriptDialog::cmbScriptChanged(int index)
{
    disconnect(m_setupModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(lstParamsChanged()));
    disconnect(m_resetModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(lstParamsChanged()));

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

    updateScript();
    updateParamList();

    ui->lstParams->setModel(ui->rdoSetupScript->isChecked() ?
                                  m_setupModel : m_resetModel);

    connect(m_setupModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(lstParamsChanged()));
    connect(m_resetModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(lstParamsChanged()));
}

void SelectScriptDialog::lstParamsChanged()
{
    updateScript();
}

void SelectScriptDialog::rebuildScriptList()
{
    int count = GlobalDatabase::instance()->getScriptCount();
    Script script;

    // Clear the combo box
    ui->cmbScript->clear();

    if( count == 0 )
    {
        m_curIndex = -1; // nothing selected
    }

    // Insert items
    for(int i = 0; i < count; i++)
    {
        GlobalDatabase::instance()->getScript(i, script);
        ui->cmbScript->addItem(script.name);
    }
}

void SelectScriptDialog::updateScript()
{
    QString &scriptText = ui->rdoSetupScript->isChecked() ?
                m_curScript.setupText : m_curScript.resetText;

    QStringList lines = scriptText.split('\n');
    QStandardItemModel *model = ui->rdoSetupScript->isChecked() ?
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
    ui->txtScript->setHtml(htmlBuffer);

    if( ui->rdoSetupScript->isChecked())
    {
        m_setupText = ui->txtScript->toPlainText();
    }
    else
    {
        m_resetText = ui->txtScript->toPlainText();
    }
}

void SelectScriptDialog::updateParamList()
{
    ui->lstParams->setModel(ui->rdoSetupScript->isChecked() ?
                                  m_setupModel : m_resetModel);
}

void SelectScriptDialog::onOK()
{
    m_script.name = m_curScript.name;
    m_script.setupText = m_setupText;
    m_script.resetText = m_resetText;

    // Generate the description string
    m_script.description = "(";
    for(int i = 0; i < m_setupModel->rowCount(); i++)
    {
        m_script.description += m_setupModel->item(i, 2)->text() + ", ";
    }
    for(int i = 0; i < m_resetModel->rowCount(); i++)
    {
        m_script.description += m_resetModel->item(i, 2)->text() + ", ";
    }
    if( m_script.description.endsWith(", "))
    {
        m_script.description = m_script.description.left(m_script.description.size() - 2);
    }
    m_script.description += ")";

    // Close the dialog
    accept();
}
