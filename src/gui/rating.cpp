#include "rating.h"

Rating::Rating(QMap<QString, QVector<RegularTraceItem> > &trace)
    : m_trace(trace)
{
}

void Rating::calc(Score &score, int dsRate, int usRate)
{
    // 0. Initialize the score structure
    //
    // 1. Traverse the trace structure and calculate task score using the
    //    default unary function model if the task trace has a "Delay" field
    //
    //    The unary function model has minimum x of 0 and a maximum of 500 (ms)
    //    Critical points are (0, 1.00), (60, 1.00) and (500, 0.00)
    //
    // 2. Traverse the trace structure and calculate task score using the
    //    "Global TxRate" strategy if the task trace has a "TxRate" field
    //
    // 3. Traverse the trace structure and calculate task score using the
    //    "Global RxRate" strategy if the task trace has an "RxRate" field
    //
    // 4. Calculate task scores, user scores and finally the overall score
    TODO: This function is corrupt

    QMap<QString, QVector<RegularTraceItem> >::iterator it;
    int i;
    int userCount = m_trace.size();
    int seconds = m_trace.begin().value().first()["Active"].size();

    // Step 0.
    score.overall = 0.0;
    score.user.clear();
    score.task.clear();
    qDebug() << "1";
    for(it = m_trace.begin(); it != m_trace.end(); ++it) // for each user
    {
        QString username = it.key();
        score.user[username] = 0;
        score.task[username] = QVector<TaskScore>();

        for(int j = 0; j < m_trace[username].size(); j++) // for each task
        {
            score.task[username].append(TaskScore());
            score.task[username][j].overall = 0.0;
            score.task[username][j].score.fill(0.0, seconds);
            score.task[username][j].valid.fill(0, false);
        }
    }
    qDebug() << "2";

    // Step 1.
    UnaryFunctionModel model(0, 500);
    model.addPoint(0, 1.0);
    model.addPoint(60, 1.0);
    model.addPoint(500, 0.0);

    qDebug() << "3";

    for(it = m_trace.begin(); it != m_trace.end(); ++it) // For each user
    {
        QString username = it.key();
        QVector<RegularTraceItem> &userTraceItems = it.value();

        for(i = 0; i < userTraceItems.size(); i++) // For each task
        {
            // typedef QMap<QString, QList<qint32> > RegularTraceItem;
            RegularTraceItem &traceItem = userTraceItems[i];
            if( traceItem.contains("Delay"))
            {
                Q_ASSERT(traceItem.contains("Active"));
                Q_ASSERT(traceItem["Active"].size() == traceItem["Delay"].size());

                for(int j = 0; j < traceItem["Delay"].size(); j++) // For each seconds
                {
                    if(traceItem["Active"][j] == 1)
                    {
                        score.task[username][i].valid.append(true);
                        score.task[username][i].score.append(
                                    model.calc(traceItem["Delay"][j]));
                    }
                    else
                    {
                        score.task[username][i].valid.append(false);
                        score.task[username][i].score.append(0.0);
                    }
                }
            }
        }
    }
    qDebug() << "4";

    // Step 2. (Global Tx Rating)
    QVector<QVector<int> > C1, C2, C3, CP, INF;
    QVector<QVector<double> > AVG;
    const double K = 1.4;

    C1.fill(QVector<int>(), userCount);
    C2.fill(QVector<int>(), userCount);
    C3.fill(QVector<int>(), userCount);
    CP.fill(QVector<int>(), userCount);
    INF.fill(QVector<int>(), userCount);
    AVG.fill(QVector<double>(), userCount);

    for(i = 0; i < userCount; i++)
    {
        C1[i].fill(usRate / userCount, seconds);
        C2[i].fill(0, seconds);
        C3[i].fill(0, seconds);
        CP[i].fill(0, seconds);
        INF[i].fill(0, seconds);
        AVG[i].fill(0.0, seconds);
    }
    qDebug() << "5";

    // Step 2.1. C2 and INF
    for(int t = 0; t < seconds; t++) // For each seconds
    {
        for(i = 0, it = m_trace.begin(); it != m_trace.end(); ++it, ++i) // For each user
        {
            QString username = it.key();
            for(int j = 0; j < m_trace[username].size(); j++) // For each task
            {
                if( m_trace[username][j].contains("TxRate"))
                {
                    if( m_trace[username][j]["MaxTxRate"][t] == -1 )
                    {
                        C2[i][t] = -1;
                        INF[i][t] = 1;
                        break;
                    }
                    else
                    {
                        C2[i][t] += m_trace[username][j]["MaxTxRate"][t];
                    }
                }
            }
        }
    }
    qDebug() << "6";

    // Step 2.2. C3 and CP
    for(int t = 0; t < seconds; t++) // For each seconds
    {
        int infCount = 0; // How many users have infinite demand in this second
        for(i = 0; i < userCount; i++)
        {
            infCount += INF[i][t];
        }

        for(i = 0, it = m_trace.begin(); it != m_trace.end(); ++it, ++i) // For each user
        {
            QString username = it.key();
            int extraBandwidth = 0;
            for(int j = 0; j < userCount; j++)
            {
                if( i == j ) continue;

                if( m_trace[username][j].contains("TxRate"))
                {
                    if( !INF[j][t] ) // no infinite demand
                    {
                        extraBandwidth += qMax(C1[j][t] - C2[j][t], 0);
                    }
                }
            }
            C3[i][t] = (infCount == 0) ? 0 : extraBandwidth / infCount;
            CP[i][t] = INF[i][t] ? C1[i][t] + C3[i][t] :
                                   qMin(C2[i][t], C1[i][t] + C3[i][t]);
        }
    }
    qDebug() << "7";

    // Step 2.3. User average score - S(i)
    for(int t = 0; t < seconds; t++) // For each seconds
    {
        for(i = 0, it = m_trace.begin(); it != m_trace.end(); ++it, ++i) // For each user
        {
            QString username = it.key();

            // Calc the user's overall tx rate
            int userTxRate = 0;
            for(int j = 0; j < m_trace[username].size(); j++) // For each task
            {
                if( m_trace[username][j].contains("TxRate"))
                {
                    userTxRate += m_trace[username][j]["TxRate"][t];
                }
            }

            // Calc the user's average score at this second
            if( userTxRate < CP[i][t] )
            {
                AVG[i][t] = userTxRate / CP[i][t];
            }
            else
            {
                AVG[i][t] = 1 + K * (userTxRate - CP[i][t]) / (usRate / userCount);
            }
        }
    }
    qDebug() << "8";

    // Step 2.4. Task Score - First Pass
    for(int t = 0; t < seconds; t++) // For each seconds
    {
        for(i = 0, it = m_trace.begin(); it != m_trace.end(); ++it, ++i) // For each user
        {
            QString username = it.key();
            for(int j = 0; j < m_trace[username].size(); j++) // For each task
            {
                if( m_trace[username][j]["MaxTxRate"][t] < AVG[i][t] )
                {
                    score.task[username][j].valid[t] = true; // true ??
                    score.task[username][j].score[t] =
                            AVG[i][t] * m_trace[username][j]["TxRate"][t] /
                                        m_trace[username][j]["MaxTxRate"][t];
                }
            }
        }
    }
    qDebug() << "9";

    // Step 2.5. Task Score - Second Pass
    for(int t = 0; t < seconds; t++) // For each seconds
    {
        for(i = 0, it = m_trace.begin(); it != m_trace.end(); ++it, ++i) // For each user
        {
            QString username = it.key();
            for(int j = 0; j < m_trace[username].size(); j++) // For each task
            {
                if( m_trace[username][j]["MaxTxRate"][t] >= AVG[i][t] )
                {
                    double fixedScore = AVG[i][t] * m_trace[username].size();
                    double fixedRate = 0;

                    for(int k = 0; k < m_trace[username].size(); k++)
                    {
                        if( m_trace[username][j]["MaxTxRate"][t] < AVG[i][t] )
                        {
                            fixedScore -= score.task[username][k].score[t];
                            fixedRate += m_trace[username][k]["TxRate"][t];
                        }
                    }
                    score.task[username][j].valid[t] = true; // true ??
                    score.task[username][j].score[t] =
                            fixedScore * m_trace[username][j]["TxRate"][t] /
                            ( m_trace[username][j]["TxRate"][t] - fixedRate);
                }
            }
        }
    }
    qDebug() << "10";
}

// Unary Function Model ///////////////////////////////////////////////////////
UnaryFunctionModel::UnaryFunctionModel(int minX, int maxX)
    : m_minX(minX), m_maxX(maxX)
{
}

void UnaryFunctionModel::clearPoints()
{
    m_xValues.clear();
    m_scores.clear();
}

void UnaryFunctionModel::addPoint(int xValue, double score)
{
    m_xValues.append(xValue); // Code for order checking to be included
    m_scores.append(score);
}

double UnaryFunctionModel::calc(int x)
{
    if( m_xValues.empty())
    {
        qDebug() << "UnaryFunctionModel::calc - model is empty";
        return 0.0;
    }

    if( x <= m_xValues.first())
    {
        return m_scores.first();
    }
    if( x >= m_xValues.last())
    {
        return m_scores.last();
    }

    int x1 = 0, x2 = 0;
    double s1 = 0, s2 = 0;

    for(int i = 0; i < m_xValues.size() - 1; i++)
    {
        if( x > m_xValues[i] && x <= m_xValues[i + 1] )
        {
            x1 = m_xValues[i];
            x2 = m_xValues[i + 1];
            s1 = m_scores[i];
            s2 = m_scores[i + 1];
            break;
        }
    }

    if( x1 == 0 && x2 == 0)
    {
        qDebug() << "An error occurred in UnaryFunctionModel::calc";
        return 0.0;
    }

    double lamda = ((double)x - (double)x1) / ((double)x2 - (double) x1);
    return (1 - lamda) * s1 + lamda * s2;
}
