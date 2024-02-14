// Some functions were adapted from Oxygen-Transparent -> oxygenblurhelper.h

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

#ifndef BLURHELPER_H
#define BLURHELPER_H

#include <QBasicTimer>
#include <QHash>
#include <QPointer>
#include <QRegion>
#include <QTimerEvent>
#include <QWidget>

#ifdef NO_KF
#endif

namespace Kvantum {
/* A class for blurring the region behind a translucent window in KDE. */
class BlurHelper : public QObject {
  Q_OBJECT

 public:
  BlurHelper(QObject *, QList<qreal> menuS, QList<qreal> tooltipS,
             int menuBlurRadius = 0, int toolTipBlurRadius = 0,
             qreal contrast = static_cast<qreal>(1),
             qreal intensity = static_cast<qreal>(1),
             qreal saturation = static_cast<qreal>(1),
             bool onlyActiveWindow = false, bool darkMode = false, QString blurType = "acrylic");

  virtual ~BlurHelper() {}

  void registerWidget(QWidget *);
  void unregisterWidget(QWidget *);

 private:
  int darkMode = false;
  int menuCorner;
  int tooltipsCorner;

  int blurType;

  bool eventFilter(QObject *watched, QEvent *event);
  void applyBackdrop(QWidget *widget);
};
}  // namespace Kvantum

#endif
