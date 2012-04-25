#ifndef SELECTSCENARIODIALOG_H
#define SELECTSCENARIODIALOG_H

#include <QDialog>

namespace Ui {
class SelectScenarioDialog;
}

class SelectScenarioDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SelectScenarioDialog(QWidget *parent = 0);
    ~SelectScenarioDialog();
    
private:
    Ui::SelectScenarioDialog *ui;
};

#endif // SELECTSCENARIODIALOG_H
