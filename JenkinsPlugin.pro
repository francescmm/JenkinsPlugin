TEMPLATE = lib

CONFIG += plugin

VERSION = 0.2.0

DEFINES += JENKINSPLUGIN_LIBRARY QT_DEPRECATED_WARNINGS

TARGET = JenkinsPlugin

QT += network widgets

CONFIG += qt warn_on c++17 c++1z

DESTDIR = lib

include(src/Jenkins.pri)
include(src/AuxiliarCustomWidgets/AuxiliarCustomWidgets.pri)
include(src/QLogger/QLogger.pri)
include(src/QPinnableTabWidget/QPinnableTabWidget.pri)

win32 {
    QMAKE_TARGET_PRODUCT = "$$TARGET"
    QMAKE_TARGET_COMPANY = "Cesc Software"
    QMAKE_TARGET_COPYRIGHT = "Francesc M."
} else:mac {
    QMAKE_TARGET_BUNDLE_PREFIX = "com.francescmm."
}

CONFIG(debug, debug|release) {
    mac: TARGET = $$join(TARGET,,,_debug)
    win32: TARGET = $$join(TARGET,,d)
    linux: TARGET = $$join(TARGET,,,d_$$VERSION)
}

win32 {
   copydata.commands = $(COPY_DIR) $$shell_quote($$shell_path($$PWD/src/interface/*.h)) $${DESTDIR}
} else {
   copydata.commands = $(COPY_DIR) $$PWD/src/interface/IJenkinsWidget.h $$PWD/src/interface/jenkinsplugin_global.h $${DESTDIR}
}

first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
