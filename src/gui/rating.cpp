#include "rating.h"

Rating::Rating(QMap<QString, QVector<RegularTraceItem> > &trace)
    : m_trace(trace)
{
}

void Rating::calc(Score &score)
{

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

void UnaryFunctionModel::addPoint(int xValue, int score)
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
