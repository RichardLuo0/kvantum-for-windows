// Adapted from Oxygen-Transparent -> oxygenblurhelper.cpp

/*
 * Copyright (C) Pedram Pourang (aka Tsu Jan) 2014-2019 <tsujan2000@gmail.com>
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

#if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
#include <KWindowEffects>
#include <KF5/kwindowsystem_version.h>
#endif

namespace Kvantum {
BlurHelper::BlurHelper (QObject* parent, QList<qreal> menuS, QList<qreal> tooltipS,
                        qreal contrast, qreal intensity, qreal saturation) : QObject (parent)
{
  contrast_ = qBound (static_cast<qreal>(0), contrast, static_cast<qreal>(2));
  intensity_ = qBound (static_cast<qreal>(0), intensity, static_cast<qreal>(2));
  saturation_ = qBound (static_cast<qreal>(0), saturation, static_cast<qreal>(2));

  if (!menuS.isEmpty() && menuS.size() >= 4)
    menuShadow_ = menuS;
  if (!tooltipS.isEmpty() && tooltipS.size() >= 4)
    tooltipShadow_ = tooltipS;
}
/*************************/
void BlurHelper::registerWidget (QWidget* widget)
{
  /* these conditions are taken care of in polishing.cpp -> polish(QWidget *widget) */
  /*if (widget->isWindow()
      && widget->testAttribute (Qt::WA_TranslucentBackground)
      && widget->windowType() != Qt::Desktop
      && !widget->testAttribute (Qt::WA_X11NetWmWindowTypeDesktop)
      && !widget->testAttribute (Qt::WA_PaintOnScreen)
      && !widget->inherits ("KScreenSaver")
      && !widget->inherits ("QTipLabel")
      && !widget->inherits ("QSplashScreen")
      && !widget->windowFlags().testFlag(Qt::FramelessWindowHint))*/

    widget->installEventFilter (this);
}
/*************************/
void BlurHelper::unregisterWidget (QWidget* widget)
{
  if (widget)
  {
    widget->removeEventFilter (this);
    clear (widget);
  }
}
/*************************/
bool BlurHelper::eventFilter (QObject* object, QEvent* event)
{
  switch (event->type())
  {
    case QEvent::Show:
    case QEvent::Hide:
    case QEvent::Resize:
    /* the theme may change from
       Kvantum and to it again */
    case QEvent::StyleChange: {
      QWidget* widget (qobject_cast<QWidget*>(object));
      /* take precautions */
      if (!widget || !widget->isWindow()) break;
      pendingWidgets_.insert (widget, widget);
      delayedUpdate();
      break;
    }

    default: break;
  }

  // never eat events
  return false;
}
/*************************/
static inline int ceilingInt (const qreal r)
{ // to prevent 1-px blurred strips outside menu/tooltip borders
  int res = qRound(r);
  if (r - static_cast<qreal>(res) > static_cast<qreal>(0.1))
    res += 1;
  return res;
}

QRegion BlurHelper::blurRegion (QWidget* widget) const
{
  if (!widget->isVisible()) return QRegion();

  QRect rect = widget->rect();
  QRegion wMask = widget->mask();

  /* blurring may not be suitable when the available
     painting area is restricted by a widget mask */
  if (!wMask.isEmpty() && wMask != QRegion(rect))
    return QRegion();

  QList<qreal> r;
  if ((qobject_cast<QMenu*>(widget)
       && !widget->testAttribute(Qt::WA_X11NetWmWindowTypeMenu)) // not a detached menu
      || widget->inherits("QComboBoxPrivateContainer"))
  {
    r = menuShadow_;
  }
  else if (widget->inherits("QTipLabel")
           /* unusual tooltips (like in KDE system settings) */
           || ((widget->windowFlags() & Qt::WindowType_Mask) == Qt::ToolTip
               && !qobject_cast<QFrame*>(widget)))
  {
    r = tooltipShadow_;
  }

  return (wMask.isEmpty()
            ? r.isEmpty()
                ? rect
                : rect.adjusted (ceilingInt(r.at(0)),
                                 ceilingInt(r.at(1)),
                                 -ceilingInt(r.at(2)),
                                 -ceilingInt(r.at(3)))
            : wMask); // is the same as rect (see above)
}
/*************************/
void BlurHelper::update (QWidget* widget) const
{
#if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
#if KWINDOWSYSTEM_VERSION < QT_VERSION_CHECK(5,82,0)
  if (!(widget->testAttribute (Qt::WA_WState_Created) || widget->internalWinId()))
    return;
#else
  QWindow *win = widget->windowHandle();
  if (win == nullptr)
    return;
#endif

  const QRegion region (blurRegion (widget));
  if (region.isEmpty())
    clear (widget);
  else
  {
#if KWINDOWSYSTEM_VERSION < QT_VERSION_CHECK(5,82,0)
    KWindowEffects::enableBlurBehind (widget->internalWinId(), true, region);
#else
    KWindowEffects::enableBlurBehind (win, true, region);
#endif
    /*NOTE: The contrast effect isn't used with menus and tooltips
            because their borders may be anti-aliased. */
    if ((contrast_ != static_cast<qreal>(1)
         || intensity_ != static_cast<qreal>(1)
         || saturation_ != static_cast<qreal>(1))
        && !qobject_cast<QMenu*>(widget)
        && !widget->inherits("QTipLabel")
        && ((widget->windowFlags() & Qt::WindowType_Mask) != Qt::ToolTip
            && !qobject_cast<QFrame*>(widget)))
    {
#if KWINDOWSYSTEM_VERSION < QT_VERSION_CHECK(5,82,0)
      KWindowEffects::enableBackgroundContrast (widget->internalWinId(), true,
                                                contrast_, intensity_, saturation_,
                                                region);
#else
      KWindowEffects::enableBackgroundContrast (win, true,
                                                contrast_, intensity_, saturation_,
                                                region);
#endif
    }
  }
  // force update
  if (widget->isVisible())
    widget->update();
#endif
}
/*************************/
void BlurHelper::clear (QWidget* widget) const
{
#if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
#if KWINDOWSYSTEM_VERSION < QT_VERSION_CHECK(5,82,0)
  // WARNING never use winId()
  if (widget->internalWinId())
  {
    KWindowEffects::enableBlurBehind (widget->internalWinId(), false);
#else
  QWindow *win = widget->windowHandle();
  if (win != nullptr)
  {
    KWindowEffects::enableBlurBehind (win, false);
#endif
    if ((contrast_ != static_cast<qreal>(1)
         || intensity_ != static_cast<qreal>(1)
         || saturation_ != static_cast<qreal>(1))
        && !qobject_cast<QMenu*>(widget)
        && !widget->inherits("QTipLabel")
        && ((widget->windowFlags() & Qt::WindowType_Mask) != Qt::ToolTip
            && !qobject_cast<QFrame*>(widget)))
    {
#if KWINDOWSYSTEM_VERSION < QT_VERSION_CHECK(5,82,0)
      KWindowEffects::enableBackgroundContrast (widget->internalWinId(), false);
#else
      KWindowEffects::enableBackgroundContrast (win, false);
#endif
    }
  }
#endif
}

}
