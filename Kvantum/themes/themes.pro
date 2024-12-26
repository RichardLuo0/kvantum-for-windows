TEMPLATE = aux

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
  KVDIR = $$PREFIX/share/Kvantum
  KFCOLORSDIR = $$PREFIX/share/color-schemes

  #MAKE INSTALL
  QMAKE_INSTALL_DIR = cp -f -R --no-preserve=mode
  kv.path = $$KVDIR
  kv.files += ./kvthemes/*
  kfcolors.path = $$KFCOLORSDIR
  kfcolors.files += ./colors/*.colors
  INSTALLS += kv kfcolors
}
