TARGET = SailfishRhodes
QT += webkit core widgets network
CONFIG += sailfishapp c++14 sailfishapp_i18n ordered

OTHER_FILES += ./data/*

DEPLOYMENT_PATH = /usr/share/$${TARGET}
data.files = data
data.path = $${DEPLOYMENT_PATH}

INSTALLS += data

SOURCES += \
    $$PWD/src/main.cpp \
    $$PWD/src/QtMainWindow.cpp \
    $$PWD/src/QtNativeTabBar.cpp \
    $$PWD/src/QtWebPage.cpp \
    $$PWD/src/RhoNativeApiCall.cpp \
    $$PWD/src/ExternalWebView.cpp

HEADERS += $$PWD/src/custommenuitem.h \
    $$PWD/src/QtMainWindow.h \
    $$PWD/src/QtNativeTabBar.h \
    $$PWD/src/QtWebPage.h \
    $$PWD/src/RhoNativeApiCall.h\
    $$PWD/src/ExternalWebView.h

DISTFILES += qml/SailfishRhodes.qml \
    qml/cover/CoverPage.qml \
    qml/pages/FirstPage.qml \
    rpm/SailfishRhodes.changes.in \
    rpm/SailfishRhodes.changes.run.in \
    rpm/SailfishRhodes.spec \
    rpm/SailfishRhodes.yaml \
    SailfishRhodes.desktop \
    translations/*.ts \
    qml/pages/MenuPage.qml

SAILFISHAPP_ICONS = 86x86 108x108 128x128

INCLUDEPATH += $$PWD/../../../../wm/rhodes
INCLUDEPATH += $$PWD/../../curl/include
INCLUDEPATH += $$PWD/../../sqlite
DEPENDPATH  += $$PWD/../../sqlite
INCLUDEPATH += $$PWD/../../ruby/include
INCLUDEPATH += $$PWD/../../
INCLUDEPATH += $$PWD/../../ruby
INCLUDEPATH += $$PWD/../rhodes

DEFINES += CPP_ELEVEN
DEFINES += OS_SAILFISH
DEFINES += OS_LINUX
DEFINES += CPP_ELEVEN

TEMPLATE = subdirs
SUBDIRS += $$PWD/../rubylib $$PWD/../rholib $$PWD/../sqlite3 $$PWD/../syncengine \
$$PWD/../../../../lib/commonAPI/coreapi/ext/platform/qt/coreapi.pro \
#$$PWD/../../../../lib/commonAPI/logCapture/ext/platform/qt/logCapture.pro \
#$$PWD/../../../../../rhoconnect-client/ext/rhoconnect-client/ext/platform/qt/rhoconnect-client.pro \
#$$PWD/../../../../lib/extensions/digest/ext/digest.pro \
#$$PWD/../../../../lib/extensions/digest-md5/ext/digest-md5.pro \
#$$PWD/../../../../lib/extensions/digest-sha1/ext/digest-sha1.pro \
#$$PWD/../../../../lib/extensions/digest-sha2/ext/digest-sha2.pro \
#$$PWD/../../../../lib/extensions/fcntl/ext/fcntl.pro \
#$$PWD/../../../../lib/extensions/openssl/ext/openssl.pro \
#$$PWD/../../../../lib/extensions/serialport/ext/serialport.pro \
$$PWD/../../../../lib/extensions/zlib/ext/zlib.pro# \
#$$PWD/../../../../lib/commonAPI/mediacapture/ext/platform/qt/MediaCapture.pro \
#$$PWD/../../../../lib/commonAPI/barcode/ext/platform/qt/Barcode.pro

SUBDIRS += $$PWD/../curl
SUBDIRS += $$PWD/../rhodes

QMAKE_CXXFLAGS_RELEASE += -MP9
QMAKE_CXXFLAGS_DEBUG += -MP9