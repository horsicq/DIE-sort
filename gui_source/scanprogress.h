// copyright (c) 2019 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef SCANPROGRESS_H
#define SCANPROGRESS_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "die_script.h"
#include <QMutex>
#include <QMutexLocker>
#include <QFutureWatcher>
#include <QtConcurrent>
#include <QSemaphore>

class ScanProgress : public QObject
{
    Q_OBJECT

public:
    struct SCAN_OPTIONS
    {
        bool bDeepScan;
        bool bSubdirectories;
        bool bShowVersion;
        bool bShowOptions;
        QSet<XBinary::FT> stFileTypes;
        bool bAllTypes;
        QSet<QString> stTypes;
        qint32 nCopyCount;
        QString sResultDirectory;
        QString sSignatures;
        QSqlDatabase dbSQLLite;
        bool bContinue;
        bool bDebug;
    };
    struct STATS
    {
        qint32 nTotal;
        qint32 nCurrent;
        qint64 nElapsed;
        QString sStatus;
        qint32 nNumberOfThreads;
    };

    explicit ScanProgress(QObject *parent=nullptr);

    void setData(QString sDirectoryName,ScanProgress::SCAN_OPTIONS *pOptions);

    quint32 getFileCount(quint32 nCRC);
    void setFileCount(quint32 nCRC,quint32 nCount);
    void setFileStat(QString sFileName, QString sTimeCount, QString sDate);
    void createTables();
    QString getCurrentFileName();
    QString getCurrentFileNameAndLock();
    qint64 getNumberOfFile();
    void findFiles(QString sDirectoryName);

    void startTransaction();
    void endTransaction();

    void _processFile(QString sFileName);

signals:
    void completed(qint64 nElapsedTime);

public slots:
    void process();
    void stop();
    STATS getCurrentStats();
    static bool createDatabase(QSqlDatabase *pDb,QString sDatabaseName);

private:
    const int N_MAXNUMBEROFTHREADS=8;
    QString _sDirectoryName;
    SCAN_OPTIONS *_pOptions;
    bool bIsStop;
    STATS currentStats;
    QElapsedTimer *pElapsedTimer;
    QMutex mutexDB;
    QMutex mutexStats;
    QSemaphore *pSemaphore;
    DiE_Script dieScript;
};

#endif // SCANPROGRESS_H
