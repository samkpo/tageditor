#ifndef FILEINFO_H
#define FILEINFO_H


class QString;
class QFileInfo;

class FileInfo
{
public:
    /*!
      constructor
      */
    FileInfo();

    /*!
      loads the given \a file
      */
    void setFile(const QString file);

    /*!
      resets everything
      */
    void clear();
    
    /*!
      saves the changes
      */
    void save();

    /*!
      @returns the file name
      \see setFileName(const QString name)
      */
    QString getFileName();

    /*!
      sets \a file to be the new file name
      \see getFileName();
      */
    void setFileName(const QString file);

    /*!
      @returns the artist
      \see setArtist(const QString artist)
      */
    QString getArtist();

    /*!
      sets \a artist as the new artist
      \see getArtist()
      */
    void setArtist(const QString artist);

    /*!
      @returns the title
      \see setTitle(const QString title)
      */
    QString getTitle();

    /*!
      set \a title as the title
      \see getTitle()
      */
    void setTitle(const QString title);

    /*!
      @returns the genere
      \see setGenre(const QString genre)
      */
    QString getGenre();

    /*!
      sets \a as the genre
      \see getGenre()
      */
    void setGenre(const QString genre);

    /*!
      @returns the album
      \see setAlbum(const QString album)
      */
    QString getAlbum();

    /*!
      sets \a album as the album
      \see getAlbum()
      */
    void setAlbum(const QString album);

    /*!
      @returns the comment
      \see setComment(const QString comment)
      */
    QString getComment();

    /*!
      sets \a as the comment
      \see getComment()
      */
    void setComment(const QString comment);

    /*!
      @returns the year
      \see setYear(int year)
      */
    int getYear();

    /*!
      sets \a year as the year
      \see getYear()
      */
    void setYear(int year);

    /*!
      @returns the track
      \see setTrack(int track)
      */
    int getTrack();

    /*!
      sets \a track as the track
      \see getTrakc()
      */
    void setTrack(int track);

private:
    void loadFields();

    QFileInfo *finfo;
    QString fName;
    QString fTitle;
    QString fArtist;
    QString fAlbum;
    QString fGenre;
    QString fComment;
    int fYear;
    int fTrack;
};

#endif // FILEINFO_H
