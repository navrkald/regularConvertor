#ifndef MYTOOLTIPQMENU_H
#define MYTOOLTIPQMENU_H

#include <QtWidgets>

class MyTooltipQMenu : public QMenu
{
    Q_OBJECT
public:
    MyTooltipQMenu(QWidget* parrent = 0);
    bool event (QEvent * e);
};

#endif // MYTOOLTIPQMENU_H
