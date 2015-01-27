#ifndef TAGEDITOR_H
#define TAGEDITOR_H

#include <QMainWindow>
#include "ui_tageditor.h"

class QLabel;
class QString;
class QAction;
class QFileInfo;
class FileInfo;
class StringProcessor;

class TagEditor : public QMainWindow
{
    Q_OBJECT

public:
    TagEditor();
    void passFileName(const QString);
    ~TagEditor();

protected:
    void closeEvent(QCloseEvent* event);

private:
    FileInfo* file;
    enum Type {allToUpper, allToLower, firstToUpper, firstEveryWordUpper};
    struct Field{
        QChar simbol;
        QString name;
        QLineEdit *p_line;
    };
    StringProcessor *p_string_processor;

    Type type;
    QList<Field> fields;
    Ui_MainWindow ui;
    bool firstTime;

    void loadFields();
    void createConnections();
    void createToolBar();
    void createActions();
    void configureStringProcessor();
    void addFields(QChar simbol, QString name, QLineEdit*);
    QString fieldsToolTip();

    QAction* openAct;
    QAction* saveAct;

private slots:
    void open();
    void updateActionsAndButtons();
    void saveFields();
    void formatFields();
    void toFileName();
    void fromFileName();

};

#endif
