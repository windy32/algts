#ifndef RATINGWIDGET_H
#define RATINGWIDGET_H

#include <QWidget>
#include "../core/trace.h"

// 1. For bulk download/upload & on/off download tasks, the regular trace items
//    consists of rate, max rate & active state. When rating curve is not
//    displayed, the widget looks like:
//
//  +-----Widget-Area----------------------------------------------+
//  |           +---------------Graph-Area-----------------------+ |
//  | 0.71 Mbps |                        +--------------------+  | |
//  |           |                        | TxRate      -----  |  | |
//  |           |                        | MaxTxRate   =====  |  | |
//  |           |                        +--------------------+  | |
//  |           |                                                | |
//  |           |                                                | |
//  |           |                                                | |
//  | 0.00 Mbps |                                                | |
//  |           +------------------------------------------------+ |
//  |           00:00                                        00:30 |
//  +--------------------------------------------------------------+
//
// 2. When rating curve is displayed, the widget looks like:
//    (Note that the score can be greater than 1.0 for a non-realtime task)
//
//  +-----Widget-Area----------------------------------------------+
//  |           +----------Graph-Area-----------------------+      |
//  | 0.71 Mbps |                   +--------------------+  | 1.17 |
//  |           |                   | TxRate      -----  |  |      |
//  |           |                   | MaxTxRate   =====  |  |      |
//  |           |                   | Score       ~~~~~  |  |      |
//  |           |                   +--------------------+  |      |
//  |           |                                           |      |
//  |           |                                           |      |
//  |           |                                           |      |
//  | 0.00 Mbps |                                           | 0.00 |
//  |           +-------------------------------------------+      |
//  |           00:00                                   00:30      |
//  +--------------------------------------------------------------+
//
// 3. For a tcp echo task, the regular trace items consists of delay & active
//    state, so the figure is similar:
//
//  +-----Widget-Area---------------------------------------------+
//  |        +---------------Graph-Area-------------------------+ |
//  | 227 ms |                          +--------------------+  | |
//  |        |                          | Delay      -----   |  | |
//  |        |                          +--------------------+  | |
//  |        |                                                  | |
//  |        |                                                  | |
//  |        |                                                  | |
//  |   0 ms |                                                  | |
//  |        +--------------------------------------------------+ |
//  |           00:00                                       00:30 |
//  +-------------------------------------------------------------+
//
//  +-----Widget-Area---------------------------------------------+
//  |        +-------------Graph-Area----------------------+      |
//  | 227 ms |                         +-----------------+ | 1.00 |
//  |        |                         | Delay   -----   | |      |
//  |        |                         | Score   ~~~~~   | |      |
//  |        |                         +-----------------+ |      |
//  |        |                                             |      |
//  |        |                                             |      |
//  |        |                                             |      |
//  |   0 ms |                                             | 0.00 |
//  |        +---------------------------------------------+      |
//  |        00:00                                     00:30      |
//  +-------------------------------------------------------------+
//
//  (Note that for a realtime task, the score cannot be greater than 1.00)
//
// 4. For an asynchronous udp echo task, an extra loss rate item should be
//    displayed. (The maximum lost rate is the graph area is fixed to 100%)
//
//  +-----Widget-Area---------------------------------------------+
//  |        +---------------Graph-Area-------------------------+ |
//  | 227 ms |                          +--------------------+  | |
//  |   100% |                          | Delay      -----   |  | |
//  |        |                          | Loss       =====   |  | |
//  |        |                          +--------------------+  | |
//  |        |                                                  | |
//  |        |                                                  | |
//  |   0 ms |                                                  | |
//  |     0% |                                                  | |
//  |        +--------------------------------------------------+ |
//  |        00:00                                          00:30 |
//  +-------------------------------------------------------------+
//
class RatingWidget : public QWidget
{
    Q_OBJECT

private:
    RegularTraceItem m_trace;
    QList<double> m_rating;
    bool m_ratingVisible;

protected:
    void paintEvent(QPaintEvent */*event*/);

public:
    explicit RatingWidget(QWidget *parent = 0);

    void setTrace(const RegularTraceItem &trace);
    void setRating(const QList<double> &rating);
    void hideRating();
    void showRating();
};

#endif // RATINGWIDGET_H
