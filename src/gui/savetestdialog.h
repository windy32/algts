#ifndef SAVETESTDIALOG_H
#define SAVETESTDIALOG_H

#include <QDialog>

namespace Ui {
class SaveTestDialog;
}

class SaveTestDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SaveTestDialog(QWidget *parent = 0);
    ~SaveTestDialog();
    
private:
    Ui::SaveTestDialog *ui;
};

#endif // SAVETESTDIALOG_H
