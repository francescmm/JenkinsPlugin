#pragma once

/****************************************************************************************
 ** GitQlient is an application to manage and operate one or several Git repositories. With
 ** GitQlient you will be able to add commits, branches and manage all the options Git provides.
 ** Copyright (C) 2021  Francesc Martinez
 **
 ** LinkedIn: www.linkedin.com/in/cescmm/
 ** Web: www.francescmm.com
 **
 ** This program is free software; you can redistribute it and/or
 ** modify it under the terms of the GNU Lesser General Public
 ** License as published by the Free Software Foundation; either
 ** version 2 of the License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 ** Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public
 ** License along with this library; if not, write to the Free Software
 ** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 ***************************************************************************************/

#include <QDialog>

#include <IJenkinsWidget.h>

namespace Ui
{
class ServerConfigDlg;
}

class QNetworkAccessManager;
class QNetworkReply;

/**
 * @brief The ServerConfigDlg class creates a small dialog where the user can add the user name and the user token of
 * the Jenkins server being used.
 *
 * @class ServerConfigDlg ServerConfigDlg.h "ServerConfigDlg.h"
 */
class ServerConfigDlg : public QDialog
{
   Q_OBJECT

public:
   /**
    * @brief Constructor builds the UI layout and configures some widgets based on the configuration.
    *
    * @param parent The parent widget if needed
    */
   explicit ServerConfigDlg(const JenkinsPlugin::ConfigData &data, const QString &styleSheet,
                            QWidget *parent = nullptr);
   /**
    * @brief Destructor that deallocates the Ui::ServerConfigDlg class.
    */
   ~ServerConfigDlg() override;

   JenkinsPlugin::ConfigData getNewConfigData() const;

private:
   Ui::ServerConfigDlg *ui = nullptr;
   JenkinsPlugin::ConfigData mData;
   QNetworkAccessManager *mManager;
   QString mEndPoint;

   void checkToken();
   void accept() override;
};
