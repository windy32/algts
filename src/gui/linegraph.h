#include <QPainter>

//  +-----Widget-Area------------------------+
//  |                                        |
//  |      +----Graph-Area--------------+    |
//  | 0.02 |                            |    |
//  |      |        /| /\               |    |
//  | 0.01 |       / |/  \____          |    |
//  |      | _____/           \________ |    |
//  | 0.00 +----------------------------+    |
//  |      0    100             1000   1100  |
//  |                                        |
//  +----------------------------------------+
class LineGraph // Should be reconstructed lator
{
private:
    QPainter *m_painter;

    int m_width; // Size of the widget area
    int m_height;

    int m_graphWidth; // Size of the graph area
    int m_graphHeight;

    int m_minX;
    int m_maxX;
    double m_minY;
    double m_maxY;

    int m_baseX; // Lower left corner of the graph area
    int m_baseY;

    QColor m_borderColor;
    QColor m_lineColor;

public:
    LineGraph(QPainter *painter, int width, int height,
              int minX, int maxX, double minY, double maxY);

    void addBorder();
    void addXScale(int flags, int xValue);
    void addYScale(int flags, double yValue, int precision = 2);
    void addLine(double x1, double x2, double y1, double y2);
};

