QT +=  \
    testlib \
    gui \
    core \
    widgets \
    charts \
    printsupport

CONFIG += qt warn_on depend_includepath testcase
CONFIG += windows

TEMPLATE = app

SOURCES +=  \
    ../Core/FileManipulation.cpp \
    ../Core/clusterManager.cpp \
    clusterTester.cpp \
    main.cpp

HEADERS += \
    ../Core/FileManipulation.h \
    ../Core/clusterManager.h \
    clusterTester.h

INCLUDEPATH += ../Core/

LIBS += -LC:/Users/USERNAME/vcpkg/packages/libssh_x64-windows/lib/ -lssh
INCLUDEPATH += C:/Users/USERNAME/vcpkg/packages/libssh_x64-windows/include
DEPENDPATH += C:/Users/USERNAME/vcpkg/packages/libssh_x64-windows/include

LIBS += -LC:/Users/USERNAME/vcpkg/packages/openssl_x64-windows/lib/ -llibcrypto -llibssl
INCLUDEPATH += C:/Users/USERNAME/vcpkg/packages/openssl_x64-windows/include
DEPENDPATH += C:/Users/USERNAME/vcpkg/packages/openssl_x64-windows/include
