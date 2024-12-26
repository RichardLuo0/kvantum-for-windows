SUBDIRS += style style-hack

equals(QT_MAJOR_VERSION, 6) {
  SUBDIRS += kvantumpreview \
             kvantummanager \
             themes
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
