#ifndef JENKINSVIEWFETCHER_H
#define JENKINSVIEWFETCHER_H

/***********************************************************************************
 **
 ** The MIT License (MIT)
 **
 ** Copyright (c) 2016 Vyacheslav Shevchuk
 ** Copyright (c) 2020 Francesc Martinez
 **
 ** LinkedIn: www.linkedin.com/in/cescmm/
 ** Web: www.francescmm.co
 **
 ** Permission is hereby granted, free of charge, to any person obtaining a
 ** copy of this software and associated documentation files (the "Software"),
 ** to deal in the Software without restriction, including without limitation
 ** the rights to use, copy, modify, merge, publish, distribute, sublicense,
 ** and/or sell copies of the Software, and to permit persons to whom the
 ** Software is furnished to do so, subject to the following conditions:
 **
 ** The above copyright notice and this permission notice shall be included in all
 ** copies or substantial portions of the Software.
 **
 ** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 ** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 ** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 ** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 ** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 ** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 ** SOFTWARE.
 ***********************************************************************************/

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QTimer>

#include <memory>

#include "restrequestbuilder.h"

namespace JenkinsCI
{
namespace Internal
{

struct ViewInfo
{
   QUrl url;
   QString name;
};

inline uint qHash(const ViewInfo &key, uint seed)
{
   return qHash(key.url, seed) ^ qHash(key.name, seed);
}

inline bool operator==(const ViewInfo &first, const ViewInfo &second)
{
   return first.name == second.name && first.url == second.url;
}

class JenkinsViewFetcher : public QObject
{
   Q_OBJECT
public:
   explicit JenkinsViewFetcher(const std::shared_ptr<RestRequestBuilder> builder, QObject *parent = nullptr);

   void fetchViews();

signals:
   void viewsFetched(QSet<ViewInfo> views);

private:
   void readReply(QNetworkReply *reply);

   std::shared_ptr<RestRequestBuilder> _builder;
   QNetworkAccessManager *_manager;
};
}
}

#endif // JENKINSVIEWFETCHER_H
