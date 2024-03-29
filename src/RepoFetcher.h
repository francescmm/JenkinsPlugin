#pragma once

#include <IFetcher.h>
#include <JenkinsViewInfo.h>

namespace Jenkins
{

class RepoFetcher final : public IFetcher
{
   Q_OBJECT

signals:
   void signalViewsReceived(const QVector<Jenkins::JenkinsViewInfo> &views);

public:
   explicit RepoFetcher(const IFetcher::Config &config, QObject *parent = nullptr);
   ~RepoFetcher();

   void triggerFetch() override;

private:
   void processData(const QJsonDocument &json) override;
};

}
