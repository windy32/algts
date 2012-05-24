#include "emulatordialog.h"
#include "ui_emulatordialog.h"

EmulatorDialog::EmulatorDialog(int &txRate, int &rxRate, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmulatorDialog), m_txRate(txRate), m_rxRate(rxRate)
{
    ui->setupUi(this);
    setLayout(ui->mainLayout);

    connect(ui->sldTxRate, SIGNAL(valueChanged(int)),
            this, SLOT(onSliderTxRate()));
    connect(ui->sldRxRate, SIGNAL(valueChanged(int)),
            this, SLOT(onSliderRxRate()));
    connect(ui->btnClearTxRate, SIGNAL(clicked()), this, SLOT(onClearTxRate()));
    connect(ui->btnClearRxRate, SIGNAL(clicked()), this, SLOT(onClearRxRate()));

    connect(ui->btnDefault, SIGNAL(clicked()), this, SLOT(onDefault()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onOK()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(onCancel()));

    if( m_txRate >= 0 && m_txRate <= 1024 * 100 )
    {
        ui->sldTxRate->setValue(m_txRate / 32);
        m_infiniteTxRate = false;
    }
    else
    {
        ui->sldTxRate->setValue(3201);
        m_infiniteTxRate = true;
    }

    if( m_rxRate >= 0 && m_rxRate <= 1024 * 100 )
    {
        ui->sldRxRate->setValue(m_rxRate / 32);
        m_infiniteRxRate = false;
    }
    else
    {
        ui->sldRxRate->setValue(3201);
        m_infiniteRxRate = true;
    }

    updateText();
}

EmulatorDialog::~EmulatorDialog()
{
    delete ui;
}

void EmulatorDialog::onSliderTxRate()
{
    m_infiniteTxRate = false;
    updateText();
}

void EmulatorDialog::onSliderRxRate()
{
    m_infiniteRxRate = false;
    updateText();
}

void EmulatorDialog::onClearTxRate()
{
    ui->sldTxRate->setValue(320);
    m_infiniteTxRate = true;
    updateText();
}

void EmulatorDialog::onClearRxRate()
{
    ui->sldRxRate->setValue(320);
    m_infiniteRxRate = true;
    updateText();
}

void EmulatorDialog::onDefault()
{
    ui->sldTxRate->setValue(8);
    ui->sldRxRate->setValue(64);
}

void EmulatorDialog::onOK()
{
    m_txRate = m_infiniteTxRate ? -1 : ui->sldTxRate->value() * 32;
    m_rxRate = m_infiniteRxRate ? -1 : ui->sldRxRate->value() * 32;
}

void EmulatorDialog::onCancel()
{
    // nothing to do here
}

void EmulatorDialog::updateText()
{
    int txRate = ui->sldTxRate->value() * 32; // kbps
    int rxRate = ui->sldRxRate->value() * 32; // kbps

    if( m_infiniteTxRate )
    {
        ui->txtTxRate->setText("INFINITE");
    }
    else if( txRate >= 1024 )
    {
        ui->txtTxRate->setText(QString("%1 Mbps").arg((double)txRate / 1024, 0, 'f', 2));
    }
    else
    {
        ui->txtTxRate->setText(QString("%1 Kbps").arg(txRate));
    }

    if( m_infiniteRxRate )
    {
        ui->txtRxRate->setText("INFINITE");
    }
    else if( rxRate >= 1024 )
    {
        ui->txtRxRate->setText(QString("%1 Mbps").arg((double)rxRate / 1024, 0, 'f', 2));
    }
    else
    {
        ui->txtRxRate->setText(QString("%1 Kbps").arg(rxRate));
    }
}
