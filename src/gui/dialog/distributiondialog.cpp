#include "distributiondialog.h"
#include "ui_distributiondialog.h"

#include "../widget/cdfwidget.h"
#include "../widget/pdfwidget.h"

#include <math.h>

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

    ui->sldUniformMin->setValue(log(10 / 1250.0 + 1) / log(1.000219747) + 1);
    ui->sldUniformMax->setValue(log(500 / 1250.0 + 1) / log(1.000219747) + 1);
    ui->sldParetoBound->setValue(log(1450 / 1250.0 + 1) / log(1.000219747) + 1);
    ui->sldParetoMean->setValue(log(500 / 1250.0 + 1) / log(1.000219747) + 1);
    ui->sldExponentialBound->setValue(log(1450 / 1250.0 + 1) / log(1.000219747) + 1);
    ui->sldExponentialMean->setValue(log(500 / 1250.0 + 1) / log(1.000219747) + 1);

    updateText(0);
}

DistributionDialog::~DistributionDialog()
{
    delete ui;
}

// An exponential sliderbar is used here
//
// Linear sliderbar:
//
// x-----------------x-----------------x-----------------x-----------------x
// 0                2500              5000              7500             10000
//
// Exponential sliderbar:
//
// x-----------------x-----------------x-----------------x-----------------x
// 0                1250              2500              5000             10000
//
// For current version of ui, the minimum value is alwasy 0, and the max 10000,
// the trasform function is:
//
// f(x) = alpha * (beta ^ x - 1)
//
// Thus, alpha * (beta ^ 10000 - 1) = 10000
//
// When alpha = 1250, and beta = (10000 / 1250 + 1) ^ 0.0001 = 1.000219747, the model
// is similar to the exponential sliderbar presented above.

void DistributionDialog::onSliderUniformMin(int value)
{
    if( value > ui->sldUniformMax->value())
    {
        ui->sldUniformMin->setValue(ui->sldUniformMax->value());
        return;
    }
    int transformedValue =
            (int)(1250.0 * (pow(1.000219747, ui->sldUniformMin->value()) - 1.0));
    ui->txtUniformMin->setText(QString("%1").arg(transformedValue));
    updateText(ui->tabs->currentIndex());
}

void DistributionDialog::onSliderUniformMax(int value)
{
    if( value < ui->sldUniformMin->value())
    {
        ui->sldUniformMax->setValue(ui->sldUniformMin->value());
        return;
    }
    int transformedValue =
            (int)(1250.0 * (pow(1.000219747, ui->sldUniformMax->value()) - 1.0));
    ui->txtUniformMax->setText(QString("%1").arg(transformedValue));
    updateText(ui->tabs->currentIndex());
}

void DistributionDialog::onSliderParetoMean(int value)
{
    if( value < 39 )
    {
        ui->sldParetoMean->setValue(39);
        return;
    }
    if( value > ui->sldParetoBound->value())
    {
        ui->sldParetoMean->setValue(ui->sldParetoBound->value());
        return;
    }
    int transformedValue =
            (int)(1250.0 * (pow(1.000219747, ui->sldParetoMean->value()) - 1.0));
    ui->txtParetoMean->setText(QString("%1").arg(transformedValue));
    updateText(ui->tabs->currentIndex());
}

void DistributionDialog::onSliderParetoShape(int value)
{
    if( value < 1001 )
    {
        ui->sldParetoShape->setValue(1001);
        return;
    }
    double shape = value / 1000.0;
    ui->txtParetoShape->setText(QString("%1").arg(shape, 0, 'f', 3));
    updateText(ui->tabs->currentIndex());
}

void DistributionDialog::onSliderParetoBound(int value)
{
    if( value < ui->sldParetoMean->value())
    {
        ui->sldParetoBound->setValue(ui->sldParetoMean->value());
        return;
    }
    int transformedValue =
            (int)(1250.0 * (pow(1.000219747, ui->sldParetoBound->value()) - 1.0));
    ui->txtParetoBound->setText(QString("%1").arg(transformedValue));
    updateText(ui->tabs->currentIndex());
}

void DistributionDialog::onSliderExponentialMean(int value)
{
    if( value > ui->sldExponentialBound->value())
    {
        ui->sldExponentialMean->setValue(ui->sldExponentialBound->value());
        return;
    }
    int transformedValue =
            (int)(1250.0 * (pow(1.000219747, ui->sldExponentialMean->value()) - 1.0));
    ui->txtExponentialMean->setText(QString("%1").arg(transformedValue));
    updateText(ui->tabs->currentIndex());
}

void DistributionDialog::onSliderExponentialBound(int value)
{
    if( value < ui->sldExponentialMean->value())
    {
        ui->sldExponentialBound->setValue(ui->sldExponentialMean->value());
        return;
    }
    int transformedValue =
            (int)(1250.0 * (pow(1.000219747, ui->sldExponentialBound->value()) - 1.0));
    ui->txtExponentialBound->setText(QString("%1").arg(transformedValue));
    updateText(ui->tabs->currentIndex());
}

void DistributionDialog::updateText(int page)
{
    if( page == 0 )
    {
        int transformedMin =
                (int)(1250.0 * (pow(1.000219747, ui->sldUniformMin->value()) - 1.0));
        int transformedMax =
                (int)(1250.0 * (pow(1.000219747, ui->sldUniformMax->value()) - 1.0));

        ui->txtDistribution->setText(
                    QString("Uniform %1, %2")
                        .arg(transformedMin)
                        .arg(transformedMax));
    }
    else if( page == 1 )
    {
        int transformedMean =
                (int)(1250.0 * (pow(1.000219747, ui->sldParetoMean->value()) - 1.0));
        int transformedBound =
                (int)(1250.0 * (pow(1.000219747, ui->sldParetoBound->value()) - 1.0));
        double shape = (double)ui->sldParetoShape->value() / 1000.0;
        ui->txtDistribution->setText(
                    QString("Pareto %1, %2, %3")
                        .arg(transformedMean)
                        .arg(shape, 0, 'f', 3)
                        .arg(transformedBound));
    }
    else if( page == 2 )
    {
        int transformedMean =
                (int)(1250.0 * (pow(1.000219747, ui->sldExponentialMean->value()) - 1.0));
        int transformedBound =
                (int)(1250.0 * (pow(1.000219747, ui->sldExponentialBound->value()) - 1.0));
        ui->txtDistribution->setText(
                    QString("Exponential %1, %2")
                        .arg(transformedMean)
                        .arg(transformedBound));
    }

    ui->cdfWidget->setDistribution(ui->txtDistribution->text());
    ui->pdfWidget->setDistribution(ui->txtDistribution->text());
}
