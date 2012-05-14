#ifndef PROGRESSTHREAD_H
#define PROGRESSTHREAD_H

#include <QThread>
#include <QProgressBar>

class ProgressThread : public QThread
{
    Q_OBJECT

private:
    int m_seconds;
    bool m_running;

protected:
    void run();

public:
    explicit ProgressThread(int seconds, QObject *parent = 0);

signals:
    void updateValue(int value);

public slots:
    void testFinished();
};

#endif // PROGRESSTHREAD_H
