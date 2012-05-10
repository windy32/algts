#include "stacklist.h"
#include <QPainter>
#include <QMouseEvent>

const int StackList::topmargin = 5;
const int StackList::leftmargin = 5;
const int StackList::rightmargin = 5;
const int StackList::height = 20;
const int StackList::ygap = 5;

StackList::StackList(QWidget *parent) :
    QWidget(parent)
{
    m_page = 0;
}

void StackList::paintEvent(QPaintEvent * /* event */)
{
    QBrush blueBrush(QColor(18, 98, 184));
    QBrush ltBlueBrush(QColor(45, 116, 196));
    QFont font("Sans", 9, QFont::Bold);

    QPainter painter(this);
    painter.setFont(font);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Clear background
    painter.setBrush(blueBrush);
    painter.fillRect(this->rect(), blueBrush);

    // Draw buttons
    const char *text[5] =
    {
        "Welcome", "Scenario", "Script", "New Test", "View Tests"
    };

    for(int i = 0; i < 5; i++)
    {
        int x = leftmargin;
        int y = topmargin + height * i + ygap * i;
        int width = this->width() - leftmargin - rightmargin;

        if( m_page == i )
        {
            painter.setPen(QColor(255, 255, 255));
            painter.setBrush(ltBlueBrush);
        }
        else
        {
            painter.setPen(QColor(131, 175, 225));
            painter.setBrush(Qt::NoBrush);
        }
        painter.drawRoundedRect(x, y, width, height, 5, 5);
        painter.drawText(x, y, width, height, Qt::AlignCenter, text[i]);
    }

}

void StackList::mousePressEvent(QMouseEvent *event)
{
    for(int i = 0; i < 5; i++)
    {
        int x = leftmargin;
        int y = topmargin + height * i + ygap * i;
        int width = this->width() - leftmargin - rightmargin;

        QRect rect(x, y, width, height);
        if( rect.contains(event->x(), event->y()))
        {
            if( m_page != i )
            {
                m_page = i;
                update();
                emit OnPageChanged(i);
            }
        }
    }
}

void StackList::SetPage(int index)
{
    if( m_page != index)
    {
        m_page = index;
        update();
    }
}
