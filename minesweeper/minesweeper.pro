QT = widgets

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        CmdGame.cpp \
        GUIGame.cpp \
        GameBoard.cpp \
        GeneralGameWidget.cpp \
        MineButton.cpp \
        QT_ResourcePack.cpp \
        command.cpp \
        main.cpp

CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    GUIGame.h \
    GameBoard.h \
    GeneralGameWidget.h \
    MineButton.h \
    QT_ResourcePack.h \
    command.h

CONFIG(debug, debug|release) {
    DEFINES += QT_DEBUG
}

## copy files into exe directory
win32 {
    copydata.commands = $(COPY_DIR) \"$$PWD/resource\" \"$$OUT_PWD/$(OBJECTS_DIR)/resource\"
} else {
    copydata.commands = $(COPY_DIR) $$PWD/resource $$OUT_PWD/$(OBJECTS_DIR)
}
#copydata.target = copydata

QMAKE_EXTRA_TARGETS += copydata
PRE_TARGETDEPS  += copydata
