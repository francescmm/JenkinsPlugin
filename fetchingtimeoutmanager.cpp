#include "fetchingtimeoutmanager.h"

using namespace JenkinsCI::Internal;

FetchingTimeoutManager::FetchingTimeoutManager(QObject *parent)
   : QObject(parent)
{
}

void FetchingTimeoutManager::processTimeout()
{
   if (_isViewsFetched && _forcedJobUpdateRequested)
   {
      _forcedJobUpdateRequested = false;
      emit jobForcedUpdateRequested();
   }
}

bool FetchingTimeoutManager::isViewsFetched() const
{
   return _isViewsFetched;
}

void FetchingTimeoutManager::setIsViewsFetched(bool isViewsFetched)
{
   _isViewsFetched = isViewsFetched;
}

void FetchingTimeoutManager::triggerFetching(FetchType fetchType)
{
   if (fetchType == FetchType::ForcedJobFetching)
      _forcedJobUpdateRequested = true;
   processTimeout();
}
