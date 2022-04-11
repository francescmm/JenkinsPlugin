INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/BuildGeneralInfoFetcher.cpp \
    $$PWD/IFetcher.cpp \
    $$PWD/JenkinsJobPanel.cpp \
    $$PWD/JenkinsWidget.cpp \
    $$PWD/JobButton.cpp \
    $$PWD/JobContainer.cpp \
    $$PWD/JobDetailsFetcher.cpp \
    $$PWD/JobFetcher.cpp \
    $$PWD/RepoFetcher.cpp \
    $$PWD/StageFetcher.cpp

HEADERS += \
    $$PWD/BuildGeneralInfoFetcher.h \
    $$PWD/IFetcher.h \
    $$PWD/IJenkinsWidget.h \
    $$PWD/JenkinsJobInfo.h \
    $$PWD/JenkinsJobPanel.h \
    $$PWD/JenkinsViewInfo.h \
    $$PWD/JenkinsWidget.h \
    $$PWD/JobButton.h \
    $$PWD/JobContainer.h \
    $$PWD/JobDetailsFetcher.h \
    $$PWD/JobFetcher.h \
    $$PWD/RepoFetcher.h \
    $$PWD/StageFetcher.h \
    $$PWD/jenkinsplugin_global.h

OTHER += $$PWD/JenkinsPlugin.json
