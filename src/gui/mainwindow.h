#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHostAddress>
#include <QStandardItemModel>

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

    // Page 3: Script
    void btnP3New();
    void btnP3Save();
    void btnP3Auto();
    void btnP3AddSelected();
    void btnP3DelSelected();

    void rdoP3SetupScript();
    void rdoP3ResetScript();

    void p3UpdateList();

private:
    Ui::MainWindow *ui;

    // Page 1: Welcome
    QString m_interface;
    QList<QHostAddress> m_addrs;

    int m_txRate; // kbps, -1 for infinite
    int m_rxRate; // kbps, -1 for infinite

    // Page 3: Script
    QStandardItemModel *m_model;
};

#endif // MAINWINDOW_H
