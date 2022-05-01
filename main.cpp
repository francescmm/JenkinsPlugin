#include <QApplication>
#include <QDialog>
#include <QGridLayout>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QSettings>

#include <JenkinsWidget.h>

class InputDlg : public QDialog
{
public:
   InputDlg(QWidget *parent = nullptr)
      : QDialog(parent)
   {
      auto layout = new QGridLayout(this);
      layout->addWidget(mUrl = new QLineEdit(), 0, 0);
      layout->addWidget(mUser = new QLineEdit(), 1, 0);
      layout->addWidget(mToken = new QLineEdit(), 2, 0);

      auto pbAccept = new QPushButton(tr("Save"));
      connect(pbAccept, &QPushButton::clicked, this, &InputDlg::saveData);

      layout->addWidget(pbAccept, 3, 0);

      setAttribute(Qt::WA_DeleteOnClose);

      mUrl->setPlaceholderText("Jenkins URL");
      mUser->setPlaceholderText("Jenkins User");
      mToken->setPlaceholderText("Jenkins Token");
   }

private:
   QLineEdit *mUrl;
   QLineEdit *mUser;
   QLineEdit *mToken;

   void saveData()
   {
      QSettings settings;
      settings.setValue("JenkinsUrl", mUrl->text());
      settings.setValue("JenkinsUser", mUser->text());
      settings.setValue("JenkinsToken", mToken->text());

      QDialog::accept();
   }
};

int main(int argc, char *argv[])
{

   qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");

   QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

   QApplication app(argc, argv);
   QSettings settings;

   const auto noInitdata = settings.value("JenkinsUrl").toString().isEmpty()
       || settings.value("JenkinsUser").toString().isEmpty() || settings.value("JenkinsToken").toString().isEmpty();

   if (noInitdata)
   {
      if (const auto dlg = new InputDlg(); dlg->exec() != QDialog::Accepted)
         return 0;
   }

   QMainWindow mainWindow;

   const auto jenkinsWidget = new Jenkins::JenkinsWidget();

   mainWindow.setCentralWidget(jenkinsWidget);

   jenkinsWidget->init(settings.value("JenkinsUrl").toString(), settings.value("JenkinsUser").toString(),
                       settings.value("JenkinsToken").toString());
   mainWindow.show();

   return app.exec();
}
