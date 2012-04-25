#ifndef EMULATORDIALOG_H
#define EMULATORDIALOG_H

#include <QDialog>

namespace Ui {
class EmulatorDialog;
}

class EmulatorDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EmulatorDialog(QWidget *parent = 0);
    ~EmulatorDialog();
    
private:
    Ui::EmulatorDialog *ui;
};

#endif // EMULATORDIALOG_H
