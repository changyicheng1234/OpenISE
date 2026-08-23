#include "BasicWidget.h"

BasicWidget::BasicWidget(int x, int y, int w, int h)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

BasicWidget::BasicWidget(const BasicWidget& m)
{
    x = m.x;
    y = m.y;
    w = m.w;
    h = m.h;
}



