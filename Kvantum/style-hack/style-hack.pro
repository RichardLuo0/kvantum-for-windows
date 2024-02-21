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

TARGET = kvantum-hack-qt$$QT_MAJOR_VERSION
TEMPLATE = lib
CONFIG += c++11

STYLE = ../style
INCLUDEPATH = $$STYLE

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
  SOURCES += $$STYLE/Kvantum.cpp \
             $$STYLE/eventFiltering.cpp \
             $$STYLE/polishing.cpp \
             $$STYLE/rendering.cpp \
             $$STYLE/standardIcons.cpp \
             $$STYLE/viewItems.cpp \
             $$STYLE/shortcuthandler.cpp \
             $$STYLE/drag/windowmanager.cpp \
             $$STYLE/blur/blurhelper.cpp \
             $$STYLE/animation/animation.cpp \
             $$STYLE/themeconfig/ThemeConfig.cpp
  HEADERS += $$STYLE/Kvantum.h \
             KvantumPlugin.h \
             $$STYLE/shortcuthandler.h \
             $$STYLE/drag/windowmanager.h \
             $$STYLE/blur/blurhelper.h \
             $$STYLE/animation/animation.h \
             $$STYLE/themeconfig/ThemeConfig.h \
             $$STYLE/themeconfig/specs.h
  OTHER_FILES += kvantum.json
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

unix|win32 {
  #VARIABLES
  COLORSDIR =$$PREFIX/share/kde4/apps/color-schemes
  KF5COLORSDIR =$$PREFIX/share/color-schemes
  DATADIR =$$PREFIX/share

  DEFINES += DATADIR="\"qgetenv(\\\"KVANTUM_DATA\\\")\""

  #MAKE INSTALL
  target.path = $$PREFIX/styles-hack
  INSTALLS += target
}
