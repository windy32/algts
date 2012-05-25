#include "rating.h"

Rating::Rating(QMap<QString, QVector<RegularTraceItem> > &trace)
    : m_trace(trace)
{
}

void Rating::calcGlobalTx(const QString &username, int taskIndex, TaskScore &ts, int usRate)
{
    // 1. Make a copy of the trace structure, and remove the items that does
    //    not contain a "TxRate" field.
    //
    //    e.g., When the original trace structure is:
    //
    //    - User Harry
    //        0. BulkDownload Trace Item [RxRate / MaxRxRate / Active]
    //      * 1. BulkUpload Trace Item [TxRate / MaxTxRate / Active]
    //        2. BulkUpload Trace Item [TxRate / MaxTxRate / Active]
    //    - User Sally
    //        0. TcpEcho Trace Item [Delay / Active]
    //        1. BulkUpload Trace Item [TxRate / MaxTxRate / Active]
    //
    //    After this operation, the structure would be:
    //
    //    - User Harry
    //      * 0. BulkUpload Trace Item [TxRate / MaxTxRate / Active]
    //        1. BulkUpload Trace Item [TxRate / MaxTxRate / Active]
    //    - User Sally
    //        0. BulkUpload Trace Item [TxRate / MaxTxRate / Active]
    //
    //    Note that the target trace item's index is likely to have been
    //    changed after the operation.
    //
    int newTaskIndex = taskIndex;
    QMap<QString, QVector<RegularTraceItem> > trace = m_trace;
    QMap<QString, QVector<RegularTraceItem> >::iterator it;
    int i;

    for(it = trace.begin(); it != trace.end(); ++it) // for each user
    {
        QString curUser = it.key();

        for(int j = trace[curUser].size() - 1; j >= 0; j--) // for each task
        {
            if( !trace[curUser][j].contains("TxRate"))
            {
                trace[curUser].remove(j);
                if( curUser == username && j < taskIndex )
                {
                    newTaskIndex -= 1;
                }
            }
        }
    }

    // 2. Initialize relative data structure
    //
    // Currently, realtime tasks (tcp echo & async udp ehco) don't have
    // a txrate or rxrate field, hence C1 is alwasy equal to usRate / N
    //
    // The environment paremeters are calculated as shown below:
    //  - C1     = usRate / N
    //  - C2(i)  = MaxRate(i,1) + MaxRate(i,2) + ... + MaxRate(i,N(i))
    //  - INF(i) = (C2(i) > C1) ? 1 : 0
    //  - C3  = [ max(C1 - C2(1), 0) + max(C1 - C2(2), 0) + ... +
    //          max(C1 - C2(N(i)), 0) ] / [ INF(1) + INF(2) + ... + INF(N(i)) ]
    //
    int userCount = trace.size();
    int seconds = trace.begin().value().first()["TxRate"].size();

    // qDebug() << "GlobalTx usercount = " << userCount << " seconds = " << seconds;

    int C1 = usRate / userCount;
    QVector<QVector<int> > C2, C3, CP, INF; // for each users and secondds
    QVector<QVector<double> > AVG;
    const double K = 1.4;

    C2.resize(userCount);
    C3.resize(userCount);
    CP.resize(userCount);
    INF.resize(userCount);
    AVG.resize(userCount);

    for(i = 0; i < userCount; i++)
    {
        C2[i].fill(0, seconds);
        C3[i].fill(0, seconds);
        CP[i].fill(0, seconds);
        INF[i].fill(0, seconds);
        AVG[i].fill(0.0, seconds);
    }

    // 3. Calc C2 and INF
    for(int t = 0; t < seconds; t++) // For each seconds
    {
        for(i = 0, it = trace.begin(); it != trace.end(); ++it, ++i) // For each user
        {
            QString username = it.key();
            for(int j = 0; j < trace[username].size(); j++) // For each task
            {
                if( trace[username][j]["MaxTxRate"][t] == -1 )
                {
                    C2[i][t] = -1;
                    break;
                }
                else
                {
                    C2[i][t] += trace[username][j]["MaxTxRate"][t];
                }
            }
            INF[i][t] = ( C2[i][t] == -1 || C2[i][t] > C1 ) ? 1 : 0;
        }
    }

    // 4. Calc C3 and CP
    for(int t = 0; t < seconds; t++) // For each seconds
    {
        int infCount = 0; // How many users have infinite demand in this second
        int extraBandwidth = 0; // Free bandwidth available in this second

        for(i = 0; i < userCount; i++)
        {
            infCount += INF[i][t];
            if( !INF[i][t] )
            {
                extraBandwidth += qMax(C1 - C2[i][t], 0);
            }
        }

        for(i = 0; i < trace.size(); i++) // For each user
        {
            C3[i][t] = (infCount == 0) ? 0 : extraBandwidth / infCount;
            CP[i][t] = INF[i][t] ? C1 + C3[i][t] :
                                   qMin(C2[i][t], C1 + C3[i][t]);
        }
    }

    // 5. Calc user average score - S(i)
    for(int t = 0; t < seconds; t++) // For each seconds
    {
        for(i = 0, it = trace.begin(); it != trace.end(); ++it, ++i) // For each user
        {
            QString username = it.key();

            // Calc the user's overall tx rate
            int userTxRate = 0;
            for(int j = 0; j < trace[username].size(); j++) // For each task
            {
                userTxRate += trace[username][j]["TxRate"][t];
            }

            // Calc the user's average score at this second
            if( userTxRate < CP[i][t] )
            {
                AVG[i][t] = (double)userTxRate / CP[i][t];
            }
            else
            {
                AVG[i][t] = 1 + K * (userTxRate - CP[i][t]) / (usRate / userCount);
            }
        }
    }

    // 6. Calc Task Score - First Pass
    ts.valid.fill(false, seconds);
    ts.score.fill(0, seconds);

    for(int t = 0; t < seconds; t++) // For each seconds
    {
        for(i = 0, it = trace.begin(); it != trace.end(); ++it, ++i) // For each user
        {
            QString username = it.key();

            // User's average tx rate (for each task) in this second
            int totalRate = 0;
            int tasks = 0;
            for(int j = 0; j < trace[username].size(); j++) // For each task
            {
                totalRate += trace[username][j]["TxRate"][t];
                tasks += 1;
            }
            int avgRate = (tasks == 0) ? 0 : totalRate / tasks;

            // Task score when L(i, j) = 1
            for(int j = 0; j < trace[username].size(); j++) // For each task
            {
                RegularTraceItem &rti = trace[username][j];

                if( rti["MaxTxRate"][t] < avgRate )
                {
                    ts.valid[t] = true; // true ??
                    ts.score[t] = AVG[i][t] * rti["TxRate"][t] / rti["MaxTxRate"][t];
                }
            }
        }
    }

    // 7. Calc Task Score - Second Pass
    for(int t = 0; t < seconds; t++) // For each seconds
    {
        for(i = 0, it = trace.begin(); it != trace.end(); ++it, ++i) // For each user
        {
            QString username = it.key();

            // User's average tx rate (for each task) in this second
            int totalRate = 0;
            int tasks = 0;
            for(int j = 0; j < trace[username].size(); j++) // For each task
            {
                totalRate += trace[username][j]["TxRate"][t];
                tasks += 1;
            }
            int avgRate = (tasks == 0) ? 0 : totalRate / tasks;

            // S(i, j) = S1 * Rate(i, j) / S(2)
            double S1 = AVG[i][t] * trace[username].size();
            double S2 = totalRate;

            // Task score when L(i, j) = 1
            for(int j = 0; j < trace[username].size(); j++) // For each task
            {
                if( trace[username][j]["MaxTxRate"][t] < avgRate )
                {
                    S1 -= ts.score[t];
                    S2 -= trace[username][j]["TxRate"][t];
                }
            }

            // Task score when L(i, j) = 0
            for(int j = 0; j < trace[username].size(); j++) // For each task
            {
                if( trace[username][j]["MaxTxRate"][t] >= avgRate )
                {
                    ts.valid[t] = true; // true ??
                    ts.score[t] = S1 * trace[username][j]["TxRate"][t] / S2;
                }
            }
        }
    }
}

void Rating::calcGlobalRx(Score &score, int dsRate)
{

}

void Rating::calcDefaultUnary(Score &score, const QString &username, int taskIndex)
{
    // Create the default model
    UnaryFunctionModel model(0, 500);
    model.addPoint(0, 1.0);
    model.addPoint(60, 1.0);
    model.addPoint(500, 0.0);

    // Calculate
    RegularTraceItem &rti = m_trace[username][taskIndex];
    TaskScore &ts = score.task[username][taskIndex];

    ts.overall = 0.0;
    ts.valid.clear();
    ts.score.clear();

    Q_ASSERT(rti.contains("Delay"));
    Q_ASSERT(rti.contains("Active"));
    Q_ASSERT(rti["Active"].size() == rti["Delay"].size());

    for(int t = 0; t < rti["Delay"].size(); t++) // For each seconds
    {
        if( rti["Active"][t] == 1 )
        {
            ts.valid.append(true);
            ts.score.append(model.calc(rti["Delay"][t]));
        }
        else
        {
            ts.valid.append(false);
            ts.score.append(0.0);
        }
    }
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
    QMap<QString, QVector<RegularTraceItem> >::iterator it;

    // Initialize the score structure
    score.overall = 0.0;
    score.user.clear();
    score.task.clear();

    for(it = m_trace.begin(); it != m_trace.end(); ++it)
    {
        QString username = it.key();

        score.user[username] = 0;
        score.task[username] = QVector<TaskScore>();
        score.task[username].resize(m_trace[username].size());
    }

    // Calculate scores
    for(it = m_trace.begin(); it != m_trace.end(); ++it) // For each user
    {
        QString username = it.key();

        for(int j = 0; j < m_trace[username].size(); j++) // For each task
        {
            if( m_trace[username][j].contains("Delay"))
            {
                calcDefaultUnary(score, username, j);
            }
            else if( m_trace[username][j].contains("TxRate"))
            {
                calcGlobalTx(username, j, score.task[username][j], usRate);
            }
            else if( m_trace[username][j].contains("RxRate"))
            {
                calcGlobalRx(score, dsRate);
            }
            else
            {
                qDebug() << "Unknown trace format";
            }
        }
    }
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
