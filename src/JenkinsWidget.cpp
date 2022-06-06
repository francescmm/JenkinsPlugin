#include "JenkinsWidget.h"

#include <JobContainer.h>
#include <RepoFetcher.h>
#include <ServerConfigDlg.h>

#include <QButtonGroup>
#include <QHBoxLayout>
#include <QNetworkAccessManager>
#include <QPushButton>
#include <QSettings>
#include <QStackedLayout>
#include <QTimer>

#include <QLogger.h>

using namespace Jenkins;

JenkinsWidget::JenkinsWidget()
   : mStackedLayout(new QStackedLayout())
   , mBodyLayout(new QHBoxLayout())
   , mBtnGroup(new QButtonGroup())
   , mButtonsLayout(new QVBoxLayout())
   , mTimer(new QTimer(this))
{
   setObjectName("JenkinsWidget");

   const auto superBtnsLayout = new QVBoxLayout();
   superBtnsLayout->setContentsMargins(QMargins());
   superBtnsLayout->setSpacing(0);
   superBtnsLayout->addLayout(mButtonsLayout);
   superBtnsLayout->addStretch();

   mBodyLayout->setSpacing(0);
   mBodyLayout->addLayout(superBtnsLayout);
   mBodyLayout->addLayout(mStackedLayout);

   const auto layout = new QHBoxLayout(this);
   layout->setContentsMargins(QMargins());
   layout->setSpacing(0);
   layout->addLayout(mBodyLayout);

   setMinimumSize(800, 600);

   mTimer->setInterval(15 * 60 * 1000); // 15 mins

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
   connect(mBtnGroup, &QButtonGroup::idClicked, mStackedLayout, &QStackedLayout::setCurrentIndex);
#else
   connect(mBtnGroup, SIGNAL(buttonClicked(int)), mStackedLayout, SLOT(setCurrentIndex(int)));
#endif
}

JenkinsWidget::~JenkinsWidget()
{
   delete mBtnGroup;
}

bool JenkinsWidget::configure(JenkinsPlugin::ConfigData config, const QString &styles)
{
   if (!config.endPoint.isEmpty())
      return false;

   if (mConfigured)
      return true;

   if (config.user.isEmpty() || config.token.isEmpty())
   {
      const auto configDlg = new ServerConfigDlg(config, styles, this);

      mConfigured = configDlg->exec() == QDialog::Accepted;

      if (mConfigured)
      {
         const auto data = configDlg->getNewConfigData();

         QSettings().setValue(QString("%1/user").arg(data.endPoint), data.user);
         QSettings().setValue(QString("%1/token").arg(data.endPoint), data.token);
         QSettings().setValue(QString("%1/endpoint").arg(data.endPoint), data.endPoint);

         mConfig = IFetcher::Config { config.user, config.token, config.endPoint, nullptr };
         mConfig.accessManager.reset(new QNetworkAccessManager());
      }
   }

   return mConfigured;
}

void JenkinsWidget::start()
{
   if (mConfigured)
   {
      mRepoFetcher = new RepoFetcher(mConfig, this);
      connect(mRepoFetcher, &RepoFetcher::signalViewsReceived, this, &JenkinsWidget::configureGeneralView);
   }
   else
      QLog_Error("JenkinsPlugin", "Trying to start JenkinsPlugin without configuring it.");
}

void JenkinsWidget::update() const
{
   mTimer->stop();
   mRepoFetcher->triggerFetch();
   mTimer->start();
}

void JenkinsWidget::configureGeneralView(const QVector<JenkinsViewInfo> &views)
{
   for (auto &view : views)
   {
      if (!mViews.contains(view))
      {
         const auto button = new QPushButton(view.name);
         button->setObjectName("JenkinsWidgetTabButton");
         button->setCheckable(true);

         const auto container = new JobContainer(mConfig, view, this);
         container->setObjectName("JobContainer");
         connect(container, &JobContainer::signalJobAreViews, this, &JenkinsWidget::configureGeneralView);
         connect(container, &JobContainer::gotoBranch, this, &JenkinsWidget::gotoBranch);
         connect(container, &JobContainer::gotoPullRequest, this, &JenkinsWidget::gotoPullRequest);

         mJobsMap.insert(view.name, container);

         mButtonsLayout->addWidget(button);
         const auto id = mStackedLayout->addWidget(container);
         mBtnGroup->addButton(button, id);

         mViews.append(view);

         if (mViews.count() == 1)
            button->setChecked(true);
      }
      else
         mJobsMap[view.name]->reload();
   }
}

IJenkinsWidget *JenkinsWidget::createWidget()
{
   return new JenkinsWidget();
}

IJenkinsWidget *createWidget()
{
   return new JenkinsWidget();
}
