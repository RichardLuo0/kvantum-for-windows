/*
 * Copyright (C) Pedram Pourang (aka Tsu Jan) 2014-2022 <tsujan2000@gmail.com>
 *
 * Kvantum is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Kvantum is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "blurhelper.h"

#include <QEvent>
#include <QMenu>
#include <QFrame>
#include <QWindow>

#ifdef NO_KF
#if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
#include <QVector>
#endif
#include <QApplication>
#else
#if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
#include <KWindowEffects>
#endif
#endif

namespace Kvantum {
BlurHelper::BlurHelper (QObject* parent, QList<qreal> menuS, QList<qreal> tooltipS,
                        int menuBlurRadius, int toolTipBlurRadius,
                        qreal contrast, qreal intensity, qreal saturation,
                        bool onlyActiveWindow) : QObject (parent)
{
}
/*************************/
void BlurHelper::registerWidget (QWidget* widget)
{
}
/*************************/
void BlurHelper::unregisterWidget (QWidget* widget)
{
}
/*************************/
bool BlurHelper::isWidgetActive (const QWidget *widget) const
{
  return (widget->window()->windowFlags().testFlag(Qt::WindowDoesNotAcceptFocus)
          || widget->window()->windowFlags().testFlag(Qt::X11BypassWindowManagerHint)
          || widget->isActiveWindow()
          // make exception for tooltips
          || widget->inherits("QTipLabel")
          || ((widget->windowFlags() & Qt::WindowType_Mask) == Qt::ToolTip
              && !qobject_cast<const QFrame*>(widget)));
}
/*************************/
bool BlurHelper::eventFilter (QObject* object, QEvent* event)
{
  // never eat events
  return false;
}
/*************************/
static inline int ceilingInt(const qreal r)
{
  int res = qRound(r);
  if (r - static_cast<qreal>(res) > static_cast<qreal>(0.1))
    res += 1;
  return res;
}

QRegion BlurHelper::blurRegion (QWidget* widget) const
{
  if (!widget->isVisible())
    return QRegion();

  if (onlyActiveWindow_ && !isWidgetActive(widget))
    return QRegion();

  QRect rect = widget->rect();
  QRegion wMask = widget->mask();

  qreal dpr = 1;
#ifdef NO_KF
  if (isX11_) {
    QWindow* win = widget->window()->windowHandle();
    dpr = win ? win->devicePixelRatio() : qApp->devicePixelRatio();
  }
#endif

  /* blurring may not be suitable when the available
     painting area is restricted by a widget mask */
  if (!wMask.isEmpty()) {
    if (wMask != QRegion(rect))
      return QRegion();
#ifdef NO_KF
    QRect mr = wMask.boundingRect();
    if (dpr > static_cast<qreal>(1))
      mr.setSize(QSizeF(mr.size() * dpr).toSize());
    return mr;
#else
    return wMask;
#endif
  }

  QList<qreal> r;
  int radius = 0;
  if ((qobject_cast<QMenu*>(widget)
       && !widget->testAttribute(Qt::WA_X11NetWmWindowTypeMenu)) // not a detached menu
      || widget->inherits("QComboBoxPrivateContainer")) {
    if (!widget->testAttribute(Qt::WA_StyleSheetTarget))
      r = menuShadow_;
    radius = menuBlurRadius_;
  } else if (widget->inherits("QTipLabel")
             /* unusual tooltips (like in KDE system settings) */
             || ((widget->windowFlags() & Qt::WindowType_Mask) == Qt::ToolTip
                 && !qobject_cast<QFrame*>(widget))) {
    if (!widget->testAttribute(Qt::WA_StyleSheetTarget))
      r = tooltipShadow_;
    radius = toolTipBlurRadius_;
  }

#ifdef NO_KF
  if (dpr > static_cast<qreal>(1)) {
    rect.setSize(QSizeF(rect.size() * dpr).toSize());
    radius *= qRound(dpr * 2);
  }
#endif

  if (!r.isEmpty()) {
    rect.adjust(ceilingInt(dpr * r.at(0)),
                ceilingInt(dpr * r.at(1)),
                -ceilingInt(dpr * r.at(2)),
                -ceilingInt(dpr * r.at(3)));
  }

  if (radius > 0) {
    radius = qMin(radius, qMin(rect.width(), rect.height()) / 2);
    QSize rSize(radius, radius);
    QRegion topLeft(QRect(rect.topLeft(), 2 * rSize), QRegion::Ellipse);
    QRegion topRight(QRect(rect.topLeft() + QPoint(rect.width() - 2 * radius, 0), 2 * rSize),
                     QRegion::Ellipse);
    QRegion bottomLeft(QRect(rect.topLeft() + QPoint(0, rect.height() - 2 * radius), 2 * rSize),
                       QRegion::Ellipse);
    QRegion bottomRight(QRect(rect.topLeft()
                                  + QPoint(rect.width() - 2 * radius, rect.height() - 2 * radius),
                              2 * rSize),
                        QRegion::Ellipse);
    return topLeft.united(topRight)
        .united(bottomLeft)
        .united(bottomRight)
        .united(QRect(rect.topLeft() + QPoint(radius, 0),
                      QSize(rect.width() - 2 * radius, rect.height())))
        .united(QRect(rect.topLeft() + QPoint(0, radius),
                      QSize(rect.width(), rect.height() - 2 * radius)));
  } else
    return rect;
}
/*************************/
void BlurHelper::update (QWidget* widget) const
{
}
/*************************/
void BlurHelper::clear (QWidget* widget) const
{
}

}
