lessThan(QT_MAJOR_VERSION, 5) {
  error("Kvantum Preview needs at least Qt 5.15.0.")
} else {
  lessThan(QT_MAJOR_VERSION, 6) {
    lessThan(QT_MINOR_VERSION, 15) {
      error("Kvantum Preview needs at least Qt 5.15.0.")
    }
  } else {
    equals(QT_MAJOR_VERSION, 6) {
      lessThan(QT_MINOR_VERSION, 2) {
        error("Kvantum needs at least Qt 6.2.0.")
      }
    } else {
      error("Kvantum cannot be compiled against this version of Qt.")
    }
  }
}

TEMPLATE = app
TARGET = kvantumpreview
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += KvantumPreview.h
FORMS += KvantumPreviewBase.ui
SOURCES += main.cpp
RESOURCES += KvantumPreviewResources.qrc
QT += widgets

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
  #TRANSLATIONS
  exists($$[QT_INSTALL_BINS]/lrelease) {
    TRANSLATIONS = $$system("find data/translations/ -name 'kvantumpreview_*.ts'")
    updateqm.input = TRANSLATIONS
    updateqm.output = data/translations/translations/${QMAKE_FILE_BASE}.qm
    updateqm.commands = $$[QT_INSTALL_BINS]/lrelease ${QMAKE_FILE_IN} -qm data/translations/translations/${QMAKE_FILE_BASE}.qm
    updateqm.CONFIG += no_link target_predeps
    QMAKE_EXTRA_COMPILERS += updateqm
  }

  #VARIABLES
  BINDIR = $$PREFIX
  DATADIR =$$PREFIX/share

  DEFINES += DATADIR="qgetenv(\\\"KVANTUM_DATA\\\")"

  trans.path = $$DATADIR/kvantumpreview
  trans.files += ./data/translations/translations

  target.path =$$BINDIR
  INSTALLS += target trans
}

message($$DATADIR)
