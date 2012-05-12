#ifndef SELECTSCENARIODIALOG_H
#define SELECTSCENARIODIALOG_H

#include <QDialog>
#include "scenarioex.h"

namespace Ui {
class SelectScenarioDialog;
}

class SelectScenarioDialog : public QDialog
{
    Q_OBJECT
    
private:
    Ui::SelectScenarioDialog *ui;
    ScenarioEx &m_scenario;
    int m_scenarioIndex;

private slots:
    void cmbScenarioChanged(int index);
    void rebuildScenarioList();
    void onOK();

public:
    explicit SelectScenarioDialog(ScenarioEx &scenario, QWidget *parent = 0);
    ~SelectScenarioDialog();

};

#endif // SELECTSCENARIODIALOG_H
