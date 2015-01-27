#include <QString>
#include <QFileInfo>
#include <QDebug>
#include <QVariant>
#include <tag.h>
#include <tstring.h>
#include <fileref.h>
#include "fileinfo.h"

FileInfo::FileInfo()
{
    finfo = new QFileInfo;
    fTrack = 0;
    fYear = 2010;
}

void FileInfo::setFile(const QString file)
{
    finfo->setFile(file);
    this->loadFields();
}

void FileInfo::clear()
{
    qDebug() << "clear";
}

void FileInfo::save()
{int i = 0;
    qDebug()<<"hola2" << i; i++;
    //create a taglib fileref
    std::string a = finfo->absoluteFilePath().toStdString();
    TagLib::FileRef *fileRef = new TagLib::FileRef(a.data());
    TagLib::Tag *t = fileRef->tag();

    //check if the file isn't null
    if (!fileRef->isNull()){
        t->setAlbum(fAlbum.toStdString());
        t->setArtist(fArtist.toStdString());
        t->setComment(fComment.toStdString());
        t->setGenre(fGenre.toStdString());
        t->setTitle(fTitle.toStdString());
        t->setTrack(fTrack);
        t->setYear(fYear);
    }

    qDebug()<<fileRef->save();
    fileRef->~FileRef();
    t->~Tag();
    qDebug()<<"hola2" << i; i++;

    QString ab;
    ab = QString("%3/%1.%2").arg(fName).arg(finfo->completeSuffix()).arg(finfo->absolutePath());
    qDebug()<<"pudo renombrear:"<<QFile(finfo->absoluteFilePath()).rename(ab);
    qDebug()<<ab;
    //finfo->
}

void FileInfo::loadFields()
{
    //create a taglib fileref
    std::string a = finfo->absoluteFilePath().toStdString();
    TagLib::FileRef* fileRef = new TagLib::FileRef(a.data());

    //check if the file isn't null
    if (!fileRef->isNull())
    {
        //sets the file name
        fName = finfo->baseName();

        //title
        fTitle = TStringToQString(fileRef->tag()->title());

        //artist
        fArtist = TStringToQString(fileRef->tag()->artist());

        //album
        fAlbum = TStringToQString(fileRef->tag()->album());

        //genre
        fGenre = TStringToQString(fileRef->tag()->genre());

        //comment
        fComment = TStringToQString(fileRef->tag()->comment());

        //track
        fTrack = QVariant(fileRef->tag()->track()).toInt();

        //year
        fYear = QVariant(fileRef->tag()->year()).toInt();
    }

    fileRef->~FileRef();
}

QString FileInfo::getComment()
{
    return fComment;
}

QString FileInfo::getAlbum()
{
    return fAlbum;
}

QString FileInfo::getArtist()
{
    return fArtist;
}

QString FileInfo::getFileName()
{
    return fName;
}

QString FileInfo::getGenre()
{
    return fGenre;
}

QString FileInfo::getTitle()
{
    return fTitle;
}

int FileInfo::getTrack()
{
    return fTrack;
}

int FileInfo::getYear()
{
    return fYear;
}

void FileInfo::setAlbum(const QString album)
{
    fAlbum = album;
}

void FileInfo::setArtist(const QString artist)
{
    fArtist = artist;
}

void FileInfo::setComment(const QString comment)
{
    fComment = comment;
}

void FileInfo::setFileName(const QString file)
{
    fName = file;
}

void FileInfo::setGenre(const QString genre)
{
    fGenre = genre;
}

void FileInfo::setTitle(const QString title)
{
    fTitle = title;
}

void FileInfo::setTrack(int track)
{
    fTrack = track;
}

void FileInfo::setYear(int year)
{
    fYear = year;
}
