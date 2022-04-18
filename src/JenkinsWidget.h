#pragma once

#include <IJenkinsWidget.h>

#include <QMap>
#include <QWidget>

#include <IFetcher.h>
#include <JenkinsViewInfo.h>

class GitBase;
class QStackedLayout;
class QButtonGroup;
class QHBoxLayout;
class QVBoxLayout;
class QTimer;
class GitQlientSettings;

namespace Jenkins
{

class RepoFetcher;
class JobContainer;

class JenkinsWidget final : public IJenkinsWidget
{
   Q_OBJECT
   Q_PLUGIN_METADATA(IID "francescmm.JenkinsPlugin" FILE "JenkinsPlugin.json")
   Q_INTERFACES(IJenkinsWidget)

public:
   JenkinsWidget();
   ~JenkinsWidget() override;

   void init(const QString &url, const QString &user, const QString &token) override;
   void update() const override;
   IJenkinsWidget *createWidget() override;

private:
   IFetcher::Config mConfig;
   QStackedLayout *mStackedLayout = nullptr;
   RepoFetcher *mRepoFetcher = nullptr;
   QHBoxLayout *mBodyLayout = nullptr;
   QButtonGroup *mBtnGroup = nullptr;
   QVBoxLayout *mButtonsLayout = nullptr;
   QVector<JenkinsViewInfo> mViews;
   QMap<QString, JobContainer *> mJobsMap;
   QTimer *mTimer = nullptr;

   void configureGeneralView(const QVector<JenkinsViewInfo> &views);
};

}
