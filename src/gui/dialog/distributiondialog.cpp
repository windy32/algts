#include "distributiondialog.h"
#include "ui_distributiondialog.h"

#include "../widget/cdfwidget.h"
#include "../widget/pdfwidget.h"

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
            this, SLOT(onSliderChanged(int)));
    connect(ui->sldUniformMax, SIGNAL(valueChanged(int)),
            this, SLOT(onSliderChanged(int)));
    connect(ui->sldParetoMean, SIGNAL(valueChanged(int)),
            this, SLOT(onSliderChanged(int)));
    connect(ui->sldParetoShape, SIGNAL(valueChanged(int)),
            this, SLOT(onSliderChanged(int)));
    connect(ui->sldParetoBound, SIGNAL(valueChanged(int)),
            this, SLOT(onSliderChanged(int)));
    connect(ui->sldExponentialMean, SIGNAL(valueChanged(int)),
            this, SLOT(onSliderChanged(int)));
    connect(ui->sldExponentialBound, SIGNAL(valueChanged(int)),
            this, SLOT(onSliderChanged(int)));

    connect(ui->tabs, SIGNAL(currentChanged(int)),
            this, SLOT(onDistributionChanged(int)));

    onDistributionChanged(0);
}

DistributionDialog::~DistributionDialog()
{
    delete ui;
}

void DistributionDialog::onSliderChanged(int /*value*/)
{
    if( ui->tabs->currentIndex() == 0 ) // Uniform
    {
        ui->txtUniformMin->setText(
                    QString("%1").arg(ui->sldUniformMin->value()));
        ui->txtUniformMax->setText(
                    QString("%1").arg(ui->sldUniformMax->value()));
    }
    else if( ui->tabs->currentIndex() == 1 ) // Pareto
    {
        double shape = (double)ui->sldParetoShape->value() / 1000.0;
        ui->txtParetoMean->setText(
                    QString("%1").arg(ui->sldParetoMean->value()));
        ui->txtParetoShape->setText(
                    QString("%1").arg(shape, 0, 'f', 3));
        ui->txtParetoBound->setText(
                    QString("%1").arg(ui->sldParetoBound->value()));
    }
    else if( ui->tabs->currentIndex() == 2 ) // Exponential
    {
        ui->txtExponentialMean->setText(
                    QString("%1").arg(ui->sldExponentialMean->value()));
        ui->txtExponentialBound->setText(
                    QString("%1").arg(ui->sldExponentialBound->value()));
    }
    onDistributionChanged(ui->tabs->currentIndex());
}

void DistributionDialog::onDistributionChanged(int page)
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

    ((CdfWidget *)ui->cdfWidget)->setDistribution(ui->txtDistribution->text());
    ((PdfWidget *)ui->pdfWidget)->setDistribution(ui->txtDistribution->text());
}
