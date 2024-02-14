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
#undef DATADIR
#include <Windows.h>
#include <dwmapi.h>
#pragma comment(lib, "Dwmapi.lib")

#include <QDebug>
#include <QEvent>
#include <QFrame>
#include <QMenu>
#include <QWindow>
#include <QtMath>

#ifdef NO_KF
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
#include <QVector>
#endif
#include <QApplication>
#else
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
#include <KWindowEffects>
#endif
#endif

DWM_WINDOW_CORNER_PREFERENCE getWindowsCorner(int radius) {
  switch (radius) {
    case 0:
      return DWMWCP_DEFAULT;
    case 1:
      return DWMWCP_ROUNDSMALL;
    default:
      return DWMWCP_ROUND;
  }
}

QHash<QString, int> backdropTypeMap = {
    {"default", -1},
    {"none", DWM_SYSTEMBACKDROP_TYPE::DWMSBT_NONE},
    {"mica", DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW},
    {"mica_alt", DWM_SYSTEMBACKDROP_TYPE::DWMSBT_TABBEDWINDOW},
    {"acrylic", DWM_SYSTEMBACKDROP_TYPE::DWMSBT_TRANSIENTWINDOW}};

namespace Kvantum {
BlurHelper::BlurHelper(QObject *parent, QList<qreal> menuS,
                       QList<qreal> tooltipS, int menuBlurRadius,
                       int toolTipBlurRadius, qreal contrast, qreal intensity,
                       qreal saturation, bool onlyActiveWindow, bool darkMode,
                       QString blurType)
    : QObject(parent), darkMode(darkMode) {
  menuCorner = getWindowsCorner(menuBlurRadius);
  tooltipsCorner = getWindowsCorner(toolTipBlurRadius);
  this->blurType = backdropTypeMap[blurType];
}

MARGINS margins = {-1};

int defaultDarkMode = 0;
auto defaultBackdropType = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_AUTO;
auto defaultWindowCorner = DWM_WINDOW_CORNER_PREFERENCE::DWMWCP_DEFAULT;

void BlurHelper::registerWidget(QWidget *widget) {
  widget->installEventFilter(this);
}

void BlurHelper::unregisterWidget(QWidget *widget) {
  widget->removeEventFilter(this);

  HWND hwnd = (HWND)widget->winId();

  DwmSetWindowAttribute(hwnd, DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
                        &defaultDarkMode, sizeof(defaultDarkMode));
  DwmSetWindowAttribute(hwnd, DWMWINDOWATTRIBUTE::DWMWA_SYSTEMBACKDROP_TYPE,
                        &defaultBackdropType, sizeof(defaultBackdropType));
  DwmSetWindowAttribute(hwnd,
                        DWMWINDOWATTRIBUTE::DWMWA_WINDOW_CORNER_PREFERENCE,
                        &defaultWindowCorner, sizeof(defaultWindowCorner));
}

bool BlurHelper::eventFilter(QObject *watched, QEvent *event) {
  if (watched->isWidgetType() && (event->type() == QEvent::Show ||
                                  event->type() == QEvent::WindowActivate)) {
    QWidget *widget = qobject_cast<QWidget *>(watched);
    applyBackdrop(widget);
  }
  return false;
}

void BlurHelper::applyBackdrop(QWidget *widget) {
  HWND hwnd = (HWND)widget->winId();

  DwmExtendFrameIntoClientArea(hwnd, &margins);
  DwmSetWindowAttribute(hwnd, DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
                        &darkMode, sizeof(darkMode));
  DwmSetWindowAttribute(hwnd, DWMWINDOWATTRIBUTE::DWMWA_SYSTEMBACKDROP_TYPE,
                        &blurType, sizeof(blurType));

  DWM_WINDOW_CORNER_PREFERENCE corner =
      DWM_WINDOW_CORNER_PREFERENCE::DWMWCP_DEFAULT;
  if (qobject_cast<QMenu *>(widget) ||
      widget->inherits("QComboBoxPrivateContainer"))
    corner = (DWM_WINDOW_CORNER_PREFERENCE)menuCorner;
  else if (widget->inherits("QTipLabel")) {
    corner = (DWM_WINDOW_CORNER_PREFERENCE)tooltipsCorner;
  }
  DwmSetWindowAttribute(hwnd,
                        DWMWINDOWATTRIBUTE::DWMWA_WINDOW_CORNER_PREFERENCE,
                        &corner, sizeof(corner));
}
}  // namespace Kvantum
