#ifndef SELECTSCRIPTDIALOG_H
#define SELECTSCRIPTDIALOG_H

#include <QDialog>
#include <QStandardItemModel>

#include "../script.h"
#include "../globaldatabase.h"

namespace Ui {
class SelectScriptDialog;
}

class SelectScriptDialog : public QDialog
{
    Q_OBJECT
        
private:
    Ui::SelectScriptDialog *ui;
    ScriptData &m_script; // the output

    QStandardItemModel *m_setupModel;
    QStandardItemModel *m_resetModel;
    QString m_setupText;
    QString m_resetText;

    Script m_curScript;
    int m_curIndex;

private slots:
    void rdoSetupScript();
    void rdoResetScript();

    void cmbScriptChanged(int index);
    void lstParamsChanged();

    void rebuildScriptList();
    void updateScript();
    void updateParamList();

    void onOK();

public:
    explicit SelectScriptDialog(ScriptData &script, QWidget *parent = 0);
    ~SelectScriptDialog();
};

#endif // SELECTSCRIPTDIALOG_H
