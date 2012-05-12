#ifndef PROGRESSTHREAD_H
#define PROGRESSTHREAD_H

#include <QThread>
#include <QProgressBar>

class ProgressThread : public QThread
{
    Q_OBJECT

private:
    QProgressBar *m_bar;
    int m_seconds;

protected:
    void run();

public:
    explicit ProgressThread(QProgressBar *bar, int seconds, QObject *parent = 0);
};

#endif // PROGRESSTHREAD_H
