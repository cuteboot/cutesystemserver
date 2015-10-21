TEMPLATE = app
SOURCES = main.cpp

INCLUDEPATH += $$[QT_INSTALL_HEADERS]

QT -= gui
LIBS += -lcutils -lbinder -lutils -lcutebinder
