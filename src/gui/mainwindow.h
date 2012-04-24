#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void MenuWelcome();
    void MenuScenario();
    void MenuScript();
    void MenuNewTest();
    void MenuViewTests();
    
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
