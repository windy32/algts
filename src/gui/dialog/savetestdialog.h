#ifndef SAVETESTDIALOG_H
#define SAVETESTDIALOG_H

#include <QDialog>
#include "../test.h"

namespace Ui {
class SaveTestDialog;
}

class SaveTestDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::SaveTestDialog *ui;
    TestResult &m_result;

protected:
    void accept();

private slots:
    void onPlatformChanged();
    void onDetailChanged();

public:
    explicit SaveTestDialog(TestResult &result, QWidget *parent = 0);
    ~SaveTestDialog();
    
};

#endif // SAVETESTDIALOG_H
