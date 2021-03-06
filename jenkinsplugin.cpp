#include "jenkinsplugin.h"
#include "jenkinspluginconstants.h"

#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/icontext.h>
#include <coreplugin/icore.h>

#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>

#include <QtPlugin>

#include "buildhistorydialog.h"
#include "jenkinsjobsmodel.h"

#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/task.h>
#include <projectexplorer/taskhub.h>

#include "warningpopup.h"

using namespace JenkinsCI::Internal;

JenkinsCIPlugin::JenkinsCIPlugin()
{
   // Create your members
   _settings.load(Core::ICore::settings());
}

JenkinsCIPlugin::~JenkinsCIPlugin()
{
   // Unregister objects from the plugin manager's object pool
   // Delete members
   _settings.save(Core::ICore::settings());
}

bool JenkinsCIPlugin::initialize(const QStringList &arguments, QString *errorString)
{
   // Register objects in the plugin manager's object pool
   // Load settings
   // Add actions to menus
   // Connect to other plugins' signals
   // In the initialize function, a plugin can be sure that the plugins it
   // depends on have initialized their members.

   Q_UNUSED(arguments)
   Q_UNUSED(errorString)
   _restRequestBuilder = std::make_shared<RestRequestBuilder>(_settings);

   _pane = new JenkinsPane(_restRequestBuilder, this);
   connect(_pane, &JenkinsPane::buildHistoryRequested, this, &JenkinsCIPlugin::showJobHistoryDialog);
   connect(_pane, &JenkinsPane::signalForceFetch, _viewFetcher, &JenkinsViewFetcher::fetchViews);

   _fetchTimeoutManager = new FetchingTimeoutManager(this);

   _fetcher = new JenkinsDataFetcher(_restRequestBuilder, this);

   _viewFetcher = new JenkinsViewFetcher(_restRequestBuilder, this);

   //    onSettingsChanged(_settings);
   createOptionsPage();
   _buildNotificator = std::make_shared<BuildNotificator>(_optionsPage->widget());

   JenkinsJobsModel::instance()->resetModel(_settings.jenkinsUrl());

   connect(_fetchTimeoutManager, &FetchingTimeoutManager::jobForcedUpdateRequested, this, [=]() {
      QUrl currentViewUrl = _restRequestBuilder->buildThisOrDefaultViewUrl(_pane->getSelectedView().url.toString());
      _fetcher->forceRefetch(currentViewUrl);
   });

   connect(_fetcher, &JenkinsDataFetcher::jobUpdated, this, &JenkinsCIPlugin::updateJob);
   connect(_viewFetcher, &JenkinsViewFetcher::viewsFetched, this, [=](QSet<ViewInfo> info) {
      _pane->updateViews(info);
      _fetchTimeoutManager->setIsViewsFetched(!info.isEmpty());
   });

   connect(_pane, &JenkinsPane::currentViewChanged, this, [=]() {
      QUrl currentViewUrl = _restRequestBuilder->buildThisOrDefaultViewUrl(_pane->getSelectedView().url.toString());
      QUrl settingsUrl = currentViewUrl;
      settingsUrl.setPort(-1);
      _settings.setSelectedViewUrl(settingsUrl.toString());
      onSettingsChanged(_settings);
   });

   connect(JenkinsJobsModel::instance(), &JenkinsJobsModel::jobFailed, _buildNotificator.get(),
           &BuildNotificator::notifyAboutFailure);

   return true;
}

void JenkinsCIPlugin::extensionsInitialized()
{
   // Retrieve objects from the plugin manager's object pool
   // In the extensionsInitialized function, a plugin can be sure that all
   // plugins that depend on it are completely initialized.
}

ExtensionSystem::IPlugin::ShutdownFlag JenkinsCIPlugin::aboutToShutdown()
{
   // Save settings
   // Disconnect from signals that are not needed during shutdown
   // Hide UI (if you add UI that is not in the main window directly)
   return SynchronousShutdown;
}

void JenkinsCIPlugin::updateJobs(QList<JenkinsJob> jobs)
{
   JenkinsJobsModel::instance()->setJenkinsJobs(jobs);
}

void JenkinsCIPlugin::updateJob(JenkinsJob job)
{
   JenkinsJobsModel::instance()->setOrUpdateJob(job);
}

void JenkinsCIPlugin::onSettingsChanged(const JenkinsSettings &settings)
{
   settings.save(Core::ICore::settings());

   _buildNotificator->setSettings(settings);
   JenkinsJobsModel::instance()->resetModel(settings.jenkinsUrl());
   _restRequestBuilder->setJenkinsSettings(settings);
   _fetchTimeoutManager->setIsViewsFetched(false);
   // reset view list only when user update jenkins server settings in options
   if (_settings.isServerSettingsDiffers(settings))
      _pane->clearViews();
   _fetchTimeoutManager->triggerFetching(FetchingTimeoutManager::FetchType::ForcedJobFetching);

   _settings = settings;
}

void JenkinsCIPlugin::showJobHistoryDialog(JenkinsJob job)
{
   BuildHistoryDialog *dialog = new BuildHistoryDialog(job, createBuildHistoryModel(), nullptr);
   dialog->setAttribute(Qt::WA_DeleteOnClose, true);
   dialog->show();
}

void JenkinsCIPlugin::createOptionsPage()
{
   _optionsPage = new OptionsPage(_settings, this);
   connect(_optionsPage, &OptionsPage::settingsChanged, this, &JenkinsCIPlugin::onSettingsChanged);
}

BuildHistoryModel *JenkinsCIPlugin::createBuildHistoryModel()
{
   BuildHistoryFetcher *_fetcher = new BuildHistoryFetcher(_restRequestBuilder);
   BuildHistoryModel *model = new BuildHistoryModel(_fetcher);
   return model;
}
