#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHostAddress>
#include <QStandardItemModel>

#include "script.h"
#include "scenarioex.h"
#include "test.h"

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
    void cmbP2ScenarioChanged(int index);
    void btnP2NewScenario();
    void btnP2SaveScenario();
    void p2RebuildScenarioList();

    void txtP21NameChanged();
    void sldP21LengthChange();
    void sldP21SeedChanged();
    void btnP21RandomSeed();

    void p2ScenarioSelected();
    void p2UserSelected(const QString &username);
    void p2TaskSelected(const QString &username, int index);

    void p2UserDeleted(const QString &username);
    void p2TaskDeleted(const QString &username, int index);

    void p2NewUser();
    void p2NewTask(const QString &username);

    void txtP22NameChanged(const QString &newUsername);

    void cmbP23TaskTypeChanged(int index);

    void sldP231MaxBytesChanged(int value);
    void sldP231MaxRateChanged(int value);

    void sldP232MaxBytesChanged(int value);
    void sldP232MaxRateChanged(int value);

    void sldP233MaxRateChanged(int value);
    void sldP233PacketSizeChanged(int value);
    void sldP233RequestSizeChanged(int value);
    void btnP233OnTime();
    void btnP233OffTime();

    void btnP234InputSize();
    void btnP234EchoSize();
    void btnP234Interval();

    void btnP235InputSize();
    void btnP235EchoSize();
    void btnP235Interval();

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

    // Page 4: New Test
    void btnP4SelectScenario();
    void btnP4SelectScript();
    void btnP4Run();
    void btnP4DefaultServer();
    void btnP4TerminalSettings();

    void p4NewLog(const QString &newLog);
    void p4UpdateProgress(int value);

    void p4TestFinished();
    void p4UpdateRunButtonState();

    static void logCallback(const char *content);

    // Page 5: View Tests
    void cmbP5ResultChanged(int index);
    void p5RebuildResultList();
    void p53RebuildUserList();

    void cmbP53UserChanged(int index);
    void lstP53TaskSelected(QModelIndex index);

    void rdoP52SetupScript();
    void rdoP52ResetScript();

private:
    Ui::MainWindow *ui;
    static Ui::MainWindow *uis;

    // Page 1: Welcome
    QString m_interface;
    QList<QHostAddress> m_addrs;

    int m_txRate; // kbps, -1 for infinite
    int m_rxRate; // kbps, -1 for infinite

    // Page 2: Scenario
    ScenarioEx m_scenario;
    int m_scenarioIndex;
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

    // Page 4: Test
    Test m_p4test;
    TestResult m_p4testResult;

    QString m_p4routerAddr;
    QString m_p4username;
    QString m_p4password;

    // Page 5: View tests
    TestResult m_p5testResult;
    int m_p5index;

    QStandardItemModel *m_p5scoreModel;
};

#endif // MAINWINDOW_H
