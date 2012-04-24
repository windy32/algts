#ifndef STACKLIST_H
#define STACKLIST_H

#include <QWidget>
#include <QPen>
#include <QBrush>

class StackList : public QWidget
{
    Q_OBJECT
private:
    static const int topmargin;
    static const int leftmargin;
    static const int rightmargin;
    static const int height;
    static const int ygap;
    int m_page;

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

public:
    explicit StackList(QWidget *parent = 0);
    
signals:
    void OnPageChanged(int index);

public slots:
    void SetPage(int index);
};

#endif // STACKLIST_H
