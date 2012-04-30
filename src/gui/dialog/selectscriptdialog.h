#ifndef SELECTSCRIPTDIALOG_H
#define SELECTSCRIPTDIALOG_H

#include <QDialog>

namespace Ui {
class SelectScriptDialog;
}

class SelectScriptDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SelectScriptDialog(QWidget *parent = 0);
    ~SelectScriptDialog();
    
private:
    Ui::SelectScriptDialog *ui;
};

#endif // SELECTSCRIPTDIALOG_H
