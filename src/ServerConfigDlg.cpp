#include "ServerConfigDlg.h"
#include "ui_ServerConfigDlg.h"

#include <QLogger.h>

#include <QDesktopServices>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>

#include <map>
#include <utility>

using namespace QLogger;

ServerConfigDlg::ServerConfigDlg(const JenkinsPlugin::ConfigData &data, const QString &styleSheet, QWidget *parent)
   : QDialog(parent)
   , ui(new Ui::ServerConfigDlg)
   , mData(data)
   , mManager(new QNetworkAccessManager())
{
   setStyleSheet(styleSheet);

   ui->setupUi(this);

   ui->leUserName->setText(mData.user);
   ui->leUserToken->setText(mData.token);
   ui->leEndPoint->setText(mData.endPoint);

   ui->lAccessToken->setText(tr("How to get a token?"));
   connect(ui->lAccessToken, &ButtonLink::clicked, []() {
      QDesktopServices::openUrl(QUrl(QString::fromUtf8("https://www.jenkins.io/doc/book/using/using-credentials/")));
   });

   connect(ui->leUserToken, &QLineEdit::editingFinished, this, &ServerConfigDlg::checkToken);
   connect(ui->leUserToken, &QLineEdit::returnPressed, this, &ServerConfigDlg::accept);
   connect(ui->pbAccept, &QPushButton::clicked, this, &ServerConfigDlg::accept);
   connect(ui->pbCancel, &QPushButton::clicked, this, &ServerConfigDlg::reject);
}

ServerConfigDlg::~ServerConfigDlg()
{
   delete mManager;
   delete ui;
}

JenkinsPlugin::ConfigData ServerConfigDlg::getNewConfigData() const
{
   return JenkinsPlugin::ConfigData { ui->leUserName->text(), ui->leUserToken->text(), mData.endPoint };
}

void ServerConfigDlg::checkToken()
{
   if (ui->leUserToken->text().isEmpty())
      ui->leUserName->setStyleSheet("border: 1px solid red;");
}

void ServerConfigDlg::accept()
{
   mEndPoint = ui->leEndPoint->text();

   QDialog::accept();
}
