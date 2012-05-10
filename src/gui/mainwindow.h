#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHostAddress>
#include <QStandardItemModel>

#include "script.h"
#include "scenarioex.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    // Menu
    void MenuWelcome();
    void MenuScenario();
    void MenuScript();
    void MenuNewTest();
    void MenuViewTests();

    // Page 1: Welcome
    void btnAddrSetting();
    void btnEmulationSetting();
    void updateEmulationState();
    void updateStatistics();

    // Page 2: Scenario
    //void p21UpdateLength();
    //void p21UpdateSeed();

    void txtP21NameChanged();
    void sldP21LengthChange();
    void sldP21SeedChanged();
    void btnP21RandomSeed();

    void p21ScenarioSelected();
    void p21UserSelected(const QString &username);
    void p21TaskSelected(const QString &username, int index);

    void txtP22NameChanged(const QString &newUsername);

    void cmbP23TaskTypeChanged(int index);

    // Page 3: Script
    void btnP3New();
    void btnP3Save();
    void btnP3Auto();
    void btnP3AddSelected();
    void btnP3DelSelected();

    void rdoP3SetupScript();
    void rdoP3ResetScript();

    void cmbP3ScriptChanged(int index);
    void txtP3ScriptChanged();
    void lstP3ParamsChanged();

    void p3RebuildScriptList();
    void p3UpdateModifiedState();
    void p3UpdateScript();
    void p3UpdateParamList();

private:
    Ui::MainWindow *ui;

    // Page 1: Welcome
    QString m_interface;
    QList<QHostAddress> m_addrs;

    int m_txRate; // kbps, -1 for infinite
    int m_rxRate; // kbps, -1 for infinite

    // Page 2: Scenario
    ScenarioEx m_scenario;
    QString m_p2user;
    int m_p2index;

    // Page 3: Script
    QStandardItemModel *m_setupModel;
    QStandardItemModel *m_resetModel;
    Script m_curScript;
    int m_curIndex;
    bool m_modified;

    enum TextState
    {
        PlainText = 0,
        HtmlText  = 1
    } m_textState;
};

#endif // MAINWINDOW_H
