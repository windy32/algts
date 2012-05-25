#ifndef RATING_H
#define RATING_H

#include "../core/core.h"

struct TaskScore
{
    double overall;
    QVector<bool> valid;
    QVector<double> score;
};

struct Score
{
    double overall;
    QMap<QString, double> user;
    QMap<QString, QVector<TaskScore> > task;
};

class Rating
{
protected:
    QMap<QString, QVector<RegularTraceItem> > &m_trace;

protected:
    void calcGlobalTx(const QString &username, int taskIndex, TaskScore &ts, int usRate);
    void calcGlobalRx(const QString &username, int taskIndex, TaskScore &ts, int dsRate);
    void calcDefaultUnary(Score &score, const QString &username, int taskIndex);

public:
    Rating(QMap<QString, QVector<RegularTraceItem> > &trace);

    void calc(Score &score, int dsRate, int usRate);
};

class UnaryFunctionModel
{
private:
    int m_minX;
    int m_maxX;

    QVector<int> m_xValues;
    QVector<double> m_scores;

public:
    UnaryFunctionModel(int minX, int maxX);

    void clearPoints();
    void addPoint(int xValue, double score);

    double calc(int x);
};

#endif // RATING_H
