//Author: Aguilera Dario, dario_21_06@hotmail.com
#include <QtGui>

#include "tageditor.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
    
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
  QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

  //all the translation stuff was taken from minitube
  const QString locale = QLocale::system().name();
  
  // app translations
#ifdef PKGDATADIR
  QString dataDir = QLatin1String(PKGDATADIR);
#else
  QString dataDir = "";
#endif
  
  QString localeDir = qApp->applicationDirPath() + QDir::separator()+ "locale";
  //qDebug() << "Using locale dir" << localeDir << locale;
  if (!QFile::exists(localeDir)) {
    localeDir = dataDir + QDir::separator() + "locale";
  }
  //qDebug() << "Using locale dir" << localeDir << locale;
  QTranslator translator;
  translator.load(locale, localeDir);
  app.installTranslator(&translator);
  
  TagEditor tagEditor;
  tagEditor.show();
  if(argc>=2){
    tagEditor.passFileName(QApplication::arguments().at(1));
  }
  return app.exec();
}
