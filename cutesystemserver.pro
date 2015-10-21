TEMPLATE = app
SOURCES = main.cpp

INCLUDEPATH += $$[QT_INSTALL_HEADERS]

QT -= gui
LIBS += -lcutils -lbinder -lutils -lcutebinder

 
target.path = $$[QT_SYSROOT]/usr/bin

INSTALLS += target
