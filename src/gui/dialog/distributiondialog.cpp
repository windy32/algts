#include "distributiondialog.h"
#include "ui_distributiondialog.h"

#include "../widget/cdfwidget.h"
#include "../widget/pdfwidget.h"

#include <QDebug>

DistributionDialog::DistributionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DistributionDialog)
{
    ui->setupUi(this);
    setLayout(ui->mainLayout);
    ui->tabUniform->setLayout(ui->P1Layout);
    ui->tabPareto->setLayout(ui->P2Layout);
    ui->tabExponential->setLayout(ui->P3Layout);

    connect(ui->sldUniformMin, SIGNAL(valueChanged(int)),
            this, SLOT(onSliderUniformMin(int)));
    connect(ui->sldUniformMax, SIGNAL(valueChanged(int)),
            this, SLOT(onSliderUniformMax(int)));
    connect(ui->sldParetoMean, SIGNAL(valueChanged(int)),
            this, SLOT(onSliderParetoMean(int)));
    connect(ui->sldParetoShape, SIGNAL(valueChanged(int)),
            this, SLOT(onSliderParetoShape(int)));
    connect(ui->sldParetoBound, SIGNAL(valueChanged(int)),
            this, SLOT(onSliderParetoBound(int)));
    connect(ui->sldExponentialMean, SIGNAL(valueChanged(int)),
            this, SLOT(onSliderExponentialMean(int)));
    connect(ui->sldExponentialBound, SIGNAL(valueChanged(int)),
            this, SLOT(onSliderExponentialBound(int)));

    connect(ui->tabs, SIGNAL(currentChanged(int)), this, SLOT(updateText(int)));
    updateText(0);
}

DistributionDialog::~DistributionDialog()
{
    delete ui;
}

void DistributionDialog::onSliderUniformMin(int value)
{
    if( value > ui->sldUniformMax->value())
    {
        ui->sldUniformMin->setValue(ui->sldUniformMax->value());
    }
    ui->txtUniformMin->setText(QString("%1").arg(ui->sldUniformMin->value()));
    updateText(ui->tabs->currentIndex());
}

void DistributionDialog::onSliderUniformMax(int value)
{
    if( value < ui->sldUniformMin->value())
    {
        ui->sldUniformMax->setValue(ui->sldUniformMin->value());
    }
    ui->txtUniformMax->setText(QString("%1").arg(ui->sldUniformMax->value()));
    updateText(ui->tabs->currentIndex());
}

void DistributionDialog::onSliderParetoMean(int value)
{
    if( value > ui->sldParetoBound->value())
    {
        ui->sldParetoMean->setValue(ui->sldParetoBound->value());
    }
    ui->txtParetoMean->setText(QString("%1").arg(ui->sldParetoMean->value()));
    updateText(ui->tabs->currentIndex());
}

void DistributionDialog::onSliderParetoShape(int value)
{
    double shape = value / 1000.0;
    ui->txtParetoShape->setText(QString("%1").arg(shape, 0, 'f', 3));
    updateText(ui->tabs->currentIndex());
}

void DistributionDialog::onSliderParetoBound(int value)
{
    if( value < ui->sldParetoMean->value())
    {
        ui->sldParetoBound->setValue(ui->sldParetoMean->value());
    }
    ui->txtParetoBound->setText(QString("%1").arg(ui->sldParetoBound->value()));
    updateText(ui->tabs->currentIndex());
}

void DistributionDialog::onSliderExponentialMean(int value)
{
    if( value > ui->sldExponentialBound->value())
    {
        ui->sldExponentialMean->setValue(ui->sldExponentialBound->value());
    }
    ui->txtExponentialMean->setText(
                QString("%1").arg(ui->sldExponentialMean->value()));
    updateText(ui->tabs->currentIndex());
}

void DistributionDialog::onSliderExponentialBound(int value)
{
    if( value < ui->sldExponentialMean->value())
    {
        ui->sldExponentialBound->setValue(ui->sldExponentialMean->value());
    }
    ui->txtExponentialBound->setText(
                QString("%1").arg(ui->sldExponentialBound->value()));
    updateText(ui->tabs->currentIndex());
}

void DistributionDialog::updateText(int page)
{
    if( page == 0 )
    {
        ui->txtDistribution->setText(
                    QString("Uniform %1, %2")
                        .arg(ui->sldUniformMin->value())
                        .arg(ui->sldUniformMax->value()));
    }
    else if( page == 1 )
    {
        double shape = (double)ui->sldParetoShape->value() / 1000.0;
        ui->txtDistribution->setText(
                    QString("Pareto %1, %2, %3")
                        .arg(ui->sldParetoMean->value())
                        .arg(shape, 0, 'f', 3)
                        .arg(ui->sldParetoBound->value()));
    }
    else if( page == 2 )
    {
        ui->txtDistribution->setText(
                    QString("Exponential %1, %2")
                        .arg(ui->sldExponentialMean->value())
                        .arg(ui->sldExponentialBound->value()));
    }

    ui->cdfWidget->setDistribution(ui->txtDistribution->text());
    ui->pdfWidget->setDistribution(ui->txtDistribution->text());
}
