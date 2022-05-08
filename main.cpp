#include <QApplication>
#include <QDialog>
#include <QGridLayout>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QSettings>

#include <JenkinsWidget.h>

int main(int argc, char *argv[])
{

   qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");

   QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

   QApplication app(argc, argv);
   QSettings settings;
   QMainWindow mainWindow;

   const auto jenkinsWidget = new Jenkins::JenkinsWidget();

   mainWindow.setCentralWidget(jenkinsWidget);

   if (jenkinsWidget->configure({ settings.value("JenkinsUser").toString(), settings.value("JenkinsToken").toString(),
                                  settings.value("JenkinsUrl").toString() },
                                ""))
   {
      mainWindow.show();

      return app.exec();
   }

   return 0;
}
