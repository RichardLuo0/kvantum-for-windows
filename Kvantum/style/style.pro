CONFIG += qt \
          plugin \
          x11 \
          warn_on

QT += svg

greaterThan(QT_MAJOR_VERSION, 4) {
  lessThan(QT_MAJOR_VERSION, 6) {
    lessThan(QT_MINOR_VERSION, 15) {
      error("Kvantum needs at least Qt 5.15.0.")
    }
    !win32: QT += x11extras
  } else {
    equals(QT_MAJOR_VERSION, 6) {
      lessThan(QT_MINOR_VERSION, 2) {
        error("Kvantum needs at least Qt 6.2.0.")
      } else {
        QT += widgets
      }
    } else {
      error("Kvantum cannot be compiled against this version of Qt.")
    }
  }
}

TARGET = kvantum-qt$$QT_MAJOR_VERSION
TEMPLATE = lib
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4) {
  lessThan(QT_MAJOR_VERSION, 6) {
    contains(WITHOUT_KF, YES) {
      DEFINES += NO_KF
      message("Compiling without KDE Frameworks...")
    } else {
      QT += KWindowSystem
    }
  } else {
    contains(WITHOUT_KF, YES) {
      DEFINES += NO_KF
      message("Compiling without KDE Frameworks...")
    }
  }
  SOURCES += Kvantum.cpp \
             eventFiltering.cpp \
             polishing.cpp \
             rendering.cpp \
             standardIcons.cpp \
             viewItems.cpp \
             KvantumPlugin.cpp \
             shortcuthandler.cpp \
             drag/windowmanager.cpp \
             blur/blurhelper.cpp \
             animation/animation.cpp \
             themeconfig/ThemeConfig.cpp
  HEADERS += Kvantum.h \
             KvantumPlugin.h \
             shortcuthandler.h \
             drag/windowmanager.h \
             blur/blurhelper.h \
             animation/animation.h \
             themeconfig/ThemeConfig.h \
             themeconfig/specs.h
  OTHER_FILES += kvantum.json
} else {
  SOURCES += qt4/Kvantum4.cpp \
             qt4/KvantumPlugin4.cpp \
             qt4/shortcuthandler4.cpp \
             qt4/x11wmmove4.cpp \
             qt4/windowmanager4.cpp \
             qt4/blurhelper4.cpp \
             qt4/ThemeConfig4.cpp
  HEADERS += qt4/Kvantum4.h \
             qt4/KvantumPlugin4.h \
             qt4/shortcuthandler4.h \
             qt4/x11wmmove4.h \
             qt4/windowmanager4.h \
             qt4/blurhelper4.h \
             qt4/ThemeConfig4.h \
             qt4/specs4.h
}

RESOURCES += themeconfig/defaulttheme.qrc

win32: LIBS += -ldwmapi

unix:!macx: LIBS += -lX11

unix {
  isEmpty(PREFIX) {
    PREFIX = /usr
  }
}

win32 {
  isEmpty(PREFIX) {
    PREFIX = "C:/Kvantum"
  }
}

isEmpty(STYLES) {
  STYLES = $$PREFIX/styles
}

unix|win32 {
  #VARIABLES
  COLORSDIR =$$PREFIX/share/kde4/apps/color-schemes
  KF5COLORSDIR =$$PREFIX/share/color-schemes
  DATADIR =$$PREFIX/share

  DEFINES += DATADIR="\"qgetenv(\\\"KVANTUM_DATA\\\")\""

  #MAKE INSTALL
  target.path = $$STYLES
  colors.path = $$COLORSDIR
  colors.files += ../color/Kvantum.colors
  kf5colors.path = $$KF5COLORSDIR
  kf5colors.files += ../color/Kvantum.colors
  lessThan(QT_MAJOR_VERSION, 5) {
    INSTALLS += target colors
  } else {
    lessThan(QT_MAJOR_VERSION, 6) {
      INSTALLS += target kf5colors
    } else {
      INSTALLS += target
    }
  }
}
