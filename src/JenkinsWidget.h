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

class JenkinsWidget final : public QWidget, public IJenkinsWidget
{
   Q_OBJECT
   Q_PLUGIN_METADATA(IID "francescmm.JenkinsPlugin" FILE "JenkinsPlugin.json")
   Q_INTERFACES(IJenkinsWidget)

signals:
   void gotoPullRequest(int prNumber);
   void gotoBranch(const QString &branchName);

public:
   JenkinsWidget(QWidget *parent = nullptr);
   ~JenkinsWidget() override;

   void initialize(const QString &url, const QString &user, const QString &token) override;

   void reload() const override;

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
