#include "mytooltipqmenu.h"

MyTooltipQMenu::MyTooltipQMenu(QWidget* parrent)
  : QMenu(parrent)
{
}

bool MyTooltipQMenu::event(QEvent *e)
{
    const QHelpEvent *helpEvent = static_cast <QHelpEvent *>(e);
    if (helpEvent->type() == QEvent::ToolTip && activeAction() != 0) {
            // call QToolTip::showText on that QAction's tooltip.
            QToolTip::showText(helpEvent->globalPos(), activeAction()->toolTip());
    } else {
            QToolTip::hideText();
    }
    return QMenu::event(e);
}
