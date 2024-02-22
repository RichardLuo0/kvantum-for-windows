SUBDIRS += style style-hack

greaterThan(QT_MAJOR_VERSION, 4) {
  lessThan(QT_MAJOR_VERSION, 6) {
    isEmpty(WITHOUT_MANAGER) {
      SUBDIRS += kvantumpreview \
                kvantummanager \
                themes
    }
  }
}

TEMPLATE = subdirs

CONFIG += qt \
          warn_on

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

others.files = ../README.md COPYING
others.path = $$PREFIX

INSTALLS += others
