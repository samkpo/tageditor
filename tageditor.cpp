#include <QtGui>
#include <stringprocessor>

#include "fileinfo.h"
#include "tageditor.h"

void showList(QStringList list)
{
    qDebug()<<"-----------------------------";
    foreach(QString str, list){
        qDebug()<<str;
    }
    qDebug()<<"-----------------------------";
}

TagEditor::TagEditor()
{
    ui.setupUi(this);
    firstTime = false;
    file = new FileInfo;

    /*create buttons and actions*/
    createActions();

    /*create connections*/
    createConnections();

    /*create toolbar*/
    createToolBar();

    //some stuff
    ui.clearButton1->setIcon(QIcon::fromTheme("edit-clear-locationbar-rtl"));
    ui.clearButton2->setIcon(QIcon::fromTheme("edit-clear-locationbar-rtl"));
    ui.clearButton3->setIcon(QIcon::fromTheme("edit-clear-locationbar-rtl"));
    ui.clearButton4->setIcon(QIcon::fromTheme("edit-clear-locationbar-rtl"));
    ui.clearButton5->setIcon(QIcon::fromTheme("edit-clear-locationbar-rtl"));
    ui.clearButton6->setIcon(QIcon::fromTheme("edit-clear-locationbar-rtl"));
    ui.clearButton7->setIcon(QIcon::fromTheme("edit-clear-locationbar-rtl"));
    ui.clearButton8->setIcon(QIcon::fromTheme("edit-clear-locationbar-rtl"));

    connect(ui.applyFormatLabel, SIGNAL(linkActivated(QString)), this, SLOT(formatFields()));

    //to and from buttons
    ui.toFileNameLine->setToolTip(this->fieldsToolTip());
    ui.toFileName->setEnabled(true);
    ui.toFileName->setIcon(QIcon::fromTheme("go-up"));
    ui.toFileName->setToolTip(tr("to file name"));

    ui.fromFileNameLine->setToolTip(this->fieldsToolTip());
    ui.fromFileName->setEnabled(true);
    ui.fromFileName->setIcon(QIcon::fromTheme("go-down"));
    ui.fromFileName->setToolTip(tr("From file name"));

    //lets configure the string processor
    this->configureStringProcessor();

    //setup fields
    this->addFields('t', tr("title"), ui.titleLine);
    this->addFields('a', tr("artist"), ui.artistLine);
    this->addFields('b', tr("album"), ui.albumLine);
    this->addFields('n', tr("track"), NULL);

}

void TagEditor::configureStringProcessor()
{
    p_string_processor = new StringProcessor;
    p_string_processor->clearIdentifiers();
}

void TagEditor::addFields(QChar simbol, QString name, QLineEdit *le)
{
    Field f = {simbol, name, le};
    fields<<f;
    p_string_processor->addIdenfifier(simbol.toAscii(), name.toStdString());
}

QString TagEditor::fieldsToolTip()
{
    QString asd;
    asd.append("<h3>" + tr("Fields:") + "</h3>");
    asd.append("<ul>");
    foreach(Field f, fields){
        asd.append("<li><b>%" + QString(f.simbol) + ":</b> " + f.name + "</li>");
    }
    asd.append("</ul>");
    return asd;
}

void TagEditor::open()//slot
{
  QString fileName = QFileDialog::getOpenFileName(this,
                                  tr("Open file", "dialog to open file"));
  if(!(fileName.isEmpty())){
    file->setFile(fileName);
    firstTime = false;
    loadFields();
  }
}

void TagEditor::toFileName()
{
    qDebug()<<"to file name";

    QString toFN = ui.toFileNameLine->text();
    if(toFN.indexOf("%t")!=-1)
        toFN.replace("%t", ui.titleLine->text());
    if(toFN.indexOf("%a")!=-1)
        toFN.replace("%a", ui.artistLine->text());
    if(toFN.indexOf("%b")!=-1)
        toFN.replace("%b", ui.albumLine->text());
    if(toFN.indexOf("%n")!=-1)
        toFN.replace("%n", QVariant(ui.trackLine->value()).toString());

    ui.fileNameLine->setText(toFN);
}

void TagEditor::fromFileName()
{
    QString _fromFN = ui.fromFileNameLine->text();

    //StringPRocessor needs a special format
    foreach(Field f, fields)
        _fromFN.replace(QString("%%1").arg(f.simbol),QString("${%1}").arg(f.simbol));

    //let's clear any previous shit
    p_string_processor->clear();

    //set the format
    p_string_processor->setFormat(_fromFN.toStdString());
    qDebug() << __LINE__ << QString::fromStdString(p_string_processor->getFormat());
    //set the string to process
    p_string_processor->setString(ui.fileNameLine->text().toStdString());
    qDebug() << __LINE__ << QString::fromStdString(p_string_processor->getString());

    //some utilitie
    typedef std::map<std::string, std::string> StringMap;

    //get the result
    StringMap resultado;
    StringMap::iterator _finder;

    //process the file
    if(!p_string_processor->processFile()){
        QMessageBox::warning(this, tr("Error"), tr("There was a trouble."), QMessageBox::Ok);
        return ;
    }

    resultado = p_string_processor->getProcessedTokens();
    foreach(Field f, fields){
        _finder = resultado.find(f.name.toStdString());
        if(_finder != resultado.end()){
            qDebug() << f.name << ": " << QString::fromStdString(_finder->second);
            if(f.p_line != NULL)
                f.p_line->setText(QString::fromStdString(_finder->second));
            else if(f.name == "track")
                ui.trackLine->setValue(QString::fromStdString(_finder->second).toInt());
        }
    }

    qDebug()<<"from file name";
}

void TagEditor::createActions()
{
  openAct = new QAction(tr("&Open"), this);
  openAct->setIcon(QIcon::fromTheme("document-open"));
  openAct->setToolTip("Open a file");
  openAct->setShortcut(tr("Ctrl+O"));
  connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
  
  saveAct = new QAction(tr("&Save"), this);
  saveAct->setEnabled(false);
  saveAct->setIcon(QIcon::fromTheme("document-save"));
  saveAct->setToolTip(tr("Save the changes and exit"));
  saveAct->setShortcut(tr("Ctrl+S"));
  connect(saveAct, SIGNAL(triggered()), this, SLOT(saveFields()));
}

void TagEditor::formatFields()
{
    QStringList fields, fields2;

    //title, artist, album, filename
    fields<<ui.titleLine->text()
          <<ui.artistLine->text()
          <<ui.albumLine->text()
          <<ui.fileNameLine->text();

    //show the loaded fields
    showList(fields);

    //
    type = Type(ui.comboBox->currentIndex());

    //modifico los campos
    for(int i=0; i < fields.size(); i++){
        QString field;
        field = fields.at(i);
        switch(type){
        case allToUpper:
            field = field.toUpper();
            break;
        case allToLower:
            field = field.toLower();
            break;
        case firstToUpper:
            field = field.at(0).toUpper() + field.right(field.length() - 1).toLower();
            break;
        case firstEveryWordUpper:
            //next stuff taken from kid3 code, so go ask them if you want
            //to know how it works
            QString newstr;
            bool wordstart = true;
            for (unsigned i = 0; i < static_cast<unsigned>(field.length()); ++i) {
                QChar ch = field.at(i);
                if (!ch.isLetterOrNumber() &&
                        ch != '\'' && ch != '`') {
                        wordstart = true;
                        newstr.append(ch);
                } else if (wordstart) {
                        wordstart = false;
                        newstr.append(ch.toUpper());
                } else {
                        newstr.append(ch.toLower());
                }
            }
            field = newstr;
            break;
        }
        fields2<<field;
    }

    //muestro los nuevos campos
    showList(fields2);

    ////title, artist, album, filename
    ui.titleLine->setText(fields2.at(0));
    ui.artistLine->setText(fields2.at(1));
    ui.albumLine->setText(fields2.at(2));
    ui.fileNameLine->setText(fields2.at(3));
}

void TagEditor::createToolBar()
{
  ui.toolBar->setMovable(false);
  ui.toolBar->addAction(openAct);
  ui.toolBar->addAction(saveAct);
}

void TagEditor::loadFields()
{
    ui.fileNameLine->setText(file->getFileName());
    ui.titleLine->setText(file->getTitle());
    ui.artistLine->setText(file->getArtist());
    ui.albumLine->setText(file->getAlbum());
    ui.genreLine->setText(file->getGenre());
    ui.commentLine->setPlainText(file->getComment());
    ui.trackLine->setValue(file->getTrack());
    ui.yearLine->setValue(file->getYear());
    firstTime = true;
}

void TagEditor::saveFields()
{
    file->setAlbum(ui.albumLine->text());
    file->setArtist(ui.artistLine->text());
    file->setComment(ui.commentLine->toPlainText());
    file->setFileName(ui.fileNameLine->text());
    file->setGenre(ui.genreLine->text());
    file->setTitle(ui.titleLine->text());
    file->setTrack(ui.trackLine->value());
    file->setYear(ui.yearLine->value());
    file->save();
}

void TagEditor::createConnections()
{
  connect(ui.artistLine, SIGNAL(textChanged(const QString)),
          this, SLOT(updateActionsAndButtons()));
  connect(ui.titleLine, SIGNAL(textChanged(const QString)),
          this, SLOT(updateActionsAndButtons()));
  connect(ui.albumLine, SIGNAL(textChanged(const QString)),
          this, SLOT(updateActionsAndButtons()));
  connect(ui.trackLine, SIGNAL(valueChanged(int)),
          this, SLOT(updateActionsAndButtons()));
  connect(ui.yearLine, SIGNAL(valueChanged(int)),
          this, SLOT(updateActionsAndButtons()));
  connect(ui.genreLine, SIGNAL(textChanged(const QString)),
          this, SLOT(updateActionsAndButtons()));
  connect(ui.commentLine, SIGNAL(textChanged()),
          this, SLOT(updateActionsAndButtons()));
  connect(ui.fileNameLine, SIGNAL(textChanged(const QString)),
          this, SLOT(updateActionsAndButtons()));
  connect(ui.toFileName, SIGNAL(pressed()), this, SLOT(toFileName()));
  connect(ui.fromFileName, SIGNAL(pressed()), this, SLOT(fromFileName()));
}

void TagEditor::updateActionsAndButtons()
{
  saveAct->setEnabled(firstTime);
}

void TagEditor::passFileName(const QString d)
{
  file->setFile(d);
  this->loadFields();
}

void TagEditor::closeEvent(QCloseEvent *event)
{
  qDebug()<<tr("Te acordaste de salvar los datos, no?");
  event->accept();
}

TagEditor::~TagEditor()
{
    delete p_string_processor;
}
