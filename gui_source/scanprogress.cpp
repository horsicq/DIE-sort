// copyright (c) 2019-2021 hors<horsicq@gmail.com>
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

#include "scanprogress.h"

ScanProgress::ScanProgress(QObject *parent) : QObject(parent)
{
    bIsStop=false;
    _pOptions=nullptr;
    currentStats={};
    pElapsedTimer=nullptr;
    pSemaphore=nullptr;
}

void ScanProgress::setData(QString sDirectoryName, ScanProgress::SCAN_OPTIONS *pOptions)
{
    this->_sDirectoryName=sDirectoryName;
    this->_pOptions=pOptions;
}

quint32 ScanProgress::getFileCount(quint32 nCRC)
{
    QMutexLocker locker(&mutexDB);

    quint32 nResult=0;

    QSqlQuery query(_pOptions->dbSQLLite);

    query.exec(QString("SELECT FILECOUNT FROM records where FILECRC='%1'").arg(nCRC));

    if(query.next())
    {
        nResult=query.value("FILECOUNT").toString().trimmed().toUInt();
    }

    if(query.lastError().text().trimmed()!="")
    {
        qDebug(query.lastQuery().toLatin1().data());
        qDebug(query.lastError().text().toLatin1().data());
    }

    return nResult;
}

void ScanProgress::setFileCount(quint32 nCRC, quint32 nCount)
{
    QMutexLocker locker(&mutexDB);

    QSqlQuery query(_pOptions->dbSQLLite);

    query.exec(QString("INSERT OR REPLACE INTO records(FILECRC,FILECOUNT) VALUES('%1','%2')").arg(nCRC).arg(nCount));

    if(query.lastError().text().trimmed()!="")
    {
        qDebug(query.lastQuery().toLatin1().data());
        qDebug(query.lastError().text().toLatin1().data());
    }
}

void ScanProgress::setFileStat(QString sFileName, QString sTimeCount, QString sDate)
{
    QMutexLocker locker(&mutexDB);

    QSqlQuery query(_pOptions->dbSQLLite);

    query.exec(QString("INSERT OR REPLACE INTO files(FILENAME,TIMECOUNT,DATETIME) VALUES('%1','%2','%3')")
               .arg(sFileName.replace("'","''"))
               .arg(sTimeCount)
               .arg(sDate));

    // QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")

    if(query.lastError().text().trimmed()!="")
    {
        qDebug(query.lastQuery().toLatin1().data());
        qDebug(query.lastError().text().toLatin1().data());
    }
}

void ScanProgress::createTables()
{
    QSqlQuery query(_pOptions->dbSQLLite);

    query.exec("DROP TABLE if exists records");
    query.exec("DROP TABLE if exists files");
    query.exec("CREATE TABLE if not exists records(FILECRC text,FILECOUNT text,PRIMARY KEY(FILECRC))");
    query.exec("CREATE TABLE if not exists files(FILENAME text,TIMECOUNT text,DATETIME text,PRIMARY KEY(FILENAME))");
}

QString ScanProgress::getCurrentFileName()
{
    QMutexLocker locker(&mutexDB);

    QString sResult;

    QSqlQuery query(_pOptions->dbSQLLite);

    query.exec(QString("SELECT FILENAME FROM files where TIMECOUNT='' AND DATETIME='' LIMIT 1"));

    if(query.next())
    {
        sResult=query.value("FILENAME").toString().trimmed();
    }

    if(query.lastError().text().trimmed()!="")
    {
        qDebug(query.lastQuery().toLatin1().data());
        qDebug(query.lastError().text().toLatin1().data());
    }

    return sResult;
}

QString ScanProgress::getCurrentFileNameAndLock()
{
    QMutexLocker locker(&mutexDB);

    QString sResult;

    QSqlQuery query(_pOptions->dbSQLLite);

    query.exec(QString("SELECT FILENAME FROM files where TIMECOUNT='' AND DATETIME='' LIMIT 1"));

    if(query.next())
    {
        sResult=query.value("FILENAME").toString().trimmed();
    }

    if(query.lastError().text().trimmed()!="")
    {
        qDebug(query.lastQuery().toLatin1().data());
        qDebug(query.lastError().text().toLatin1().data());
    }

    query.exec(QString("INSERT OR REPLACE INTO files(FILENAME,TIMECOUNT,DATETIME) VALUES('%1','%2','%3')")
               .arg(sResult.replace("'","''"))
               .arg(0)
               .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));

    if(query.lastError().text().trimmed()!="")
    {
        qDebug(query.lastQuery().toLatin1().data());
        qDebug(query.lastError().text().toLatin1().data());
    }

    return sResult;
}

qint64 ScanProgress::getNumberOfFile()
{
    QMutexLocker locker(&mutexDB);

    qint64 nResult=0;

    QSqlQuery query(_pOptions->dbSQLLite);

    query.exec(QString("SELECT COUNT(FILENAME) AS VALUE FROM files where TIMECOUNT='' AND DATETIME=''"));

    if(query.next())
    {
        nResult=query.value("VALUE").toULongLong();
    }

    return nResult;
}

void ScanProgress::findFiles(QString sDirectoryName)
{
    if(!bIsStop)
    {
        QFileInfo fi(sDirectoryName);

        if(fi.isFile())
        {
            currentStats.nTotal++;
            setFileStat(fi.absoluteFilePath(),"","");
        }
        else if(fi.isDir()&&(_pOptions->bSubdirectories))
        {
            QDir dir(sDirectoryName);

            QFileInfoList eil=dir.entryInfoList();
            
            int nCount=eil.count();

            for(int i=0; (i<nCount)&&(!bIsStop); i++)
            {
                QString sFN=eil.at(i).fileName();

                if((sFN!=".")&&(sFN!=".."))
                {
                    findFiles(eil.at(i).absoluteFilePath());
                }
            }
        }
    }
}

void ScanProgress::startTransaction()
{
    QSqlQuery query(_pOptions->dbSQLLite);

    query.exec("BEGIN TRANSACTION");
}

void ScanProgress::endTransaction()
{
    QSqlQuery query(_pOptions->dbSQLLite);

    query.exec("COMMIT");
}

void ScanProgress::_processFile(QString sFileName)
{
    pSemaphore->acquire();

    mutexStats.lock();
    currentStats.nCurrent++;
    currentStats.sStatus=sFileName;
    currentStats.stFiles.insert(sFileName);
    stScripts.insert(&dieScript);
    mutexStats.unlock();

    if(sFileName!="")
    {
        QString sTempFile;

        if(_pOptions->bDebug)
        {
            sTempFile=_pOptions->sResultDirectory;

            XBinary::createDirectory(sFileName);

            sTempFile+=QDir::separator()+XBinary::getBaseFileName(sFileName);

            XBinary::copyFile(sFileName,sTempFile);
        }

        DiE_Script::SCAN_OPTIONS options={};

        options.bDeepScan=_pOptions->bDeepScan;
        options.bShowType=true;
        options.bShowVersion=_pOptions->bShowVersion;
        options.bShowOptions=_pOptions->bShowOptions;

        setFileStat(sFileName,"",QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

        DiE_Script::SCAN_RESULT scanResult=dieScript.scanFile(sFileName,&options);

        QString sEP;
        QString sEPREL;

        if(     (_pOptions->copyFormat==ScanProgress::CF_ARCH_FT_TYPE_NAME_EP)||
                (_pOptions->copyFormat==ScanProgress::CF_FT_ARCH_TYPE_NAME_EP)||
                (_pOptions->copyFormat==ScanProgress::CF_FT_TYPE_NAME_EP)||
                (_pOptions->copyFormat==ScanProgress::CF_ARCH_FT_TYPE_NAME_EPREL)||
                (_pOptions->copyFormat==ScanProgress::CF_FT_ARCH_TYPE_NAME_EPREL)||
                (_pOptions->copyFormat==ScanProgress::CF_FT_TYPE_NAME_EPREL))
        {
            QFile _file;
            _file.setFileName(sFileName);

            if(_file.open(QIODevice::ReadOnly))
            {
                XBinary::_MEMORY_MAP memoryMap=XFormats::getMemoryMap(scanResult.scanHeader.fileType,&_file);
                qint64 nEPAddress=XFormats::getEntryPointAddress(scanResult.scanHeader.fileType,&_file);

                sEP=XCapstone::getSignature(&_file,&memoryMap,nEPAddress,XCapstone::ST_MASK,10).replace(".","_");
                sEPREL=XCapstone::getSignature(&_file,&memoryMap,nEPAddress,XCapstone::ST_MASKREL,10).replace(".","_");

                _file.close();
            }
        }

        QString _sBaseFileName=QFileInfo(scanResult.sFileName).completeBaseName();

        if((_pOptions->fileFormat==FF_MD5)||(_pOptions->fileFormat==FF_MD5_ORIGINAL))
        {
            QString sMD5=XBinary::getHash(XBinary::HASH_MD5,scanResult.sFileName);

            if(_pOptions->fileFormat==FF_MD5)
            {
                _sBaseFileName=sMD5;
            }
            else if(_pOptions->fileFormat==FF_MD5_ORIGINAL)
            {
                _sBaseFileName=sMD5+_sBaseFileName;
            }
        }

        int nCount=scanResult.listRecords.count();

        bool bGlobalCopy=false;
        bool bIdentified=false;

        if(nCount)
        {
            for(int i=0;i<nCount;i++)
            {
                DiE_Script::SCAN_STRUCT ss=scanResult.listRecords.at(i);

                if(_pOptions->stFileTypes.contains(ss.scanHeader.fileType))
                {
                    if((_pOptions->stTypes.contains(ss.sType))||(_pOptions->bAllTypes))
                    {
                        bIdentified=true;

                        if((_pOptions->copyType==CT_IDENT)||(_pOptions->copyType==CT_IDENT_UNK))
                        {
                            QString sResult=ss.sResult;

                            sResult=XBinary::convertFileNameSymbols(sResult);

                            quint32 nCRC=XBinary::getStringCustomCRC32(XBinary::fileTypeIdToString(ss.scanHeader.fileType)+ss.scanHeader.sArch+ss.sType+sResult);

                            bool bCopy=true;

                            int nCurrentCount=getFileCount(nCRC);

                            if(_pOptions->nCopyCount)
                            {
                                if(nCurrentCount>=_pOptions->nCopyCount)
                                {
                                    bCopy=false;
                                }
                            }

                            if(bCopy)
                            {
                                QString _sFileName=  _pOptions->sResultDirectory+QDir::separator()+
                                                    createPath(_pOptions->copyFormat,ss.scanHeader)+QDir::separator()+
                                                    ss.sType+QDir::separator()+sResult;

                                if(     (_pOptions->copyFormat==ScanProgress::CF_ARCH_FT_TYPE_NAME_EP)||
                                        (_pOptions->copyFormat==ScanProgress::CF_FT_ARCH_TYPE_NAME_EP)||
                                        (_pOptions->copyFormat==ScanProgress::CF_FT_TYPE_NAME_EP))
                                {
                                    _sFileName+=QDir::separator()+sEP;
                                }
                                else if((_pOptions->copyFormat==ScanProgress::CF_ARCH_FT_TYPE_NAME_EPREL)||
                                        (_pOptions->copyFormat==ScanProgress::CF_FT_ARCH_TYPE_NAME_EPREL)||
                                        (_pOptions->copyFormat==ScanProgress::CF_FT_TYPE_NAME_EPREL))
                                {
                                    _sFileName+=QDir::separator()+sEPREL;
                                }

                                XBinary::createDirectory(_sFileName);

                                _sFileName+=QDir::separator()+_sBaseFileName;

                                if(XBinary::copyFile(scanResult.sFileName,_sFileName))
                                {
                                    bGlobalCopy=true;

                                    setFileCount(nCRC,nCurrentCount+1);
                                }
                            }
                        }
                    }
                }
            }
        }

        if((!bIdentified)&&((_pOptions->copyType==CT_IDENT_UNK)||(_pOptions->copyType==CT_UNK)))
        {
            if(_pOptions->stFileTypes.contains(scanResult.scanHeader.fileType))
            {
                DiE_Script::SCAN_HEADER sh=scanResult.scanHeader;

                quint32 nCRC=XBinary::getStringCustomCRC32(XBinary::fileTypeIdToString(sh.fileType)+sh.sArch+"__UNKNOWN");

                bool bCopy=true;

                int nCurrentCount=getFileCount(nCRC);

                if(_pOptions->nCopyCount)
                {
                    if(nCurrentCount>=_pOptions->nCopyCount)
                    {
                        bCopy=false;
                    }
                }

                if(bCopy)
                {
                    QString _sFileName=  _pOptions->sResultDirectory+QDir::separator()+
                                        createPath(_pOptions->copyFormat,sh)+QDir::separator()+
                                        "__UNKNOWN";

                    if(     (_pOptions->copyFormat==ScanProgress::CF_ARCH_FT_TYPE_NAME_EP)||
                            (_pOptions->copyFormat==ScanProgress::CF_FT_ARCH_TYPE_NAME_EP)||
                            (_pOptions->copyFormat==ScanProgress::CF_FT_TYPE_NAME_EP))
                    {
                        _sFileName+=QDir::separator()+sEP;
                    }
                    else if((_pOptions->copyFormat==ScanProgress::CF_ARCH_FT_TYPE_NAME_EPREL)||
                            (_pOptions->copyFormat==ScanProgress::CF_FT_ARCH_TYPE_NAME_EPREL)||
                            (_pOptions->copyFormat==ScanProgress::CF_FT_TYPE_NAME_EPREL))
                    {
                        _sFileName+=QDir::separator()+sEPREL;
                    }

                    bool bIsOverlayPresent=false;

                    if(_pOptions->unknownPrefix!=UP_NONE)
                    {
                        QString sFolderName;
                        qint32 nUnknownCount=_pOptions->nUnknownCount;

                        QFile file;

                        file.setFileName(scanResult.sFileName);

                        if(file.open(QIODevice::ReadOnly))
                        {
                            if((sh.fileType==XBinary::FT_PE32)||(sh.fileType==XBinary::FT_PE64))
                            {
                                XPE pe(&file);

                                bIsOverlayPresent=pe.isOverlayPresent();

                                if(_pOptions->unknownPrefix==UP_EP_BYTES)
                                {
                                    sFolderName=pe.getSignature(pe._getEntryPointOffset(),nUnknownCount);
                                }
                                else if(_pOptions->unknownPrefix==UP_OVERLAY_BYTES)
                                {
                                    if(bIsOverlayPresent)
                                    {
                                        sFolderName=pe.getSignature(pe.getOverlayOffset(),nUnknownCount);
                                    }
                                }
                                else if(_pOptions->unknownPrefix==UP_HEADER_BYTES)
                                {
                                    sFolderName=pe.getSignature(0,nUnknownCount);
                                }
                            }
                            else if((sh.fileType==XBinary::FT_ELF32)||(sh.fileType==XBinary::FT_ELF64))
                            {
                                XELF elf(&file);

                                bIsOverlayPresent=elf.isOverlayPresent();

                                if(_pOptions->unknownPrefix==UP_EP_BYTES)
                                {
                                    sFolderName=elf.getSignature(elf._getEntryPointOffset(),nUnknownCount);
                                }
                                else if(_pOptions->unknownPrefix==UP_OVERLAY_BYTES)
                                {
                                    if(bIsOverlayPresent)
                                    {
                                        sFolderName=elf.getSignature(elf.getOverlayOffset(),nUnknownCount);
                                    }
                                }
                                else if(_pOptions->unknownPrefix==UP_HEADER_BYTES)
                                {
                                    sFolderName=elf.getSignature(0,nUnknownCount);
                                }
                            }
                            else if((sh.fileType==XBinary::FT_MACHO32)||(sh.fileType==XBinary::FT_MACHO64))
                            {
                                XMACH mach(&file);

                                bIsOverlayPresent=mach.isOverlayPresent();

                                if(_pOptions->unknownPrefix==UP_EP_BYTES)
                                {
                                    sFolderName=mach.getSignature(mach._getEntryPointOffset(),nUnknownCount);
                                }
                                else if(_pOptions->unknownPrefix==UP_OVERLAY_BYTES)
                                {
                                    if(bIsOverlayPresent)
                                    {
                                        sFolderName=mach.getSignature(mach.getOverlayOffset(),nUnknownCount);
                                    }
                                }
                                else if(_pOptions->unknownPrefix==UP_HEADER_BYTES)
                                {
                                    sFolderName=mach.getSignature(0,nUnknownCount);
                                }
                            }
                            else if(sh.fileType==XBinary::FT_MSDOS)
                            {
                                XMSDOS msdos(&file);

                                bIsOverlayPresent=msdos.isOverlayPresent();

                                if(_pOptions->unknownPrefix==UP_EP_BYTES)
                                {
                                    sFolderName=msdos.getSignature(msdos._getEntryPointOffset(),nUnknownCount);
                                }
                                else if(_pOptions->unknownPrefix==UP_OVERLAY_BYTES)
                                {
                                    if(bIsOverlayPresent)
                                    {
                                        sFolderName=msdos.getSignature(msdos.getOverlayOffset(),nUnknownCount);
                                    }
                                }
                                else if(_pOptions->unknownPrefix==UP_HEADER_BYTES)
                                {
                                    sFolderName=msdos.getSignature(0,nUnknownCount);
                                }
                            }
                            else
                            {
                                XBinary binary(&file);

                                bIsOverlayPresent=binary.isOverlayPresent();

                                if(_pOptions->unknownPrefix==UP_EP_BYTES)
                                {
                                    sFolderName=binary.getSignature(binary._getEntryPointOffset(),nUnknownCount);
                                }
                                else if(_pOptions->unknownPrefix==UP_OVERLAY_BYTES)
                                {
                                    if(bIsOverlayPresent)
                                    {
                                        sFolderName=binary.getSignature(binary.getOverlayOffset(),nUnknownCount);
                                    }
                                }
                                else if(_pOptions->unknownPrefix==UP_HEADER_BYTES)
                                {
                                    sFolderName=binary.getSignature(0,nUnknownCount);
                                }
                            }

                            if(_pOptions->unknownPrefix==UP_OPCODES)
                            {
                                XBinary::_MEMORY_MAP memoryMap=XFormats::getMemoryMap(sh.fileType,&file);
                                qint64 nEntryPointAddress=XFormats::getEntryPointAddress(sh.fileType,&file);

                                sFolderName=XCapstone::getSignature(&file,&memoryMap,nEntryPointAddress,XCapstone::ST_MASK,nUnknownCount);
                            }
                            else if(_pOptions->unknownPrefix==UP_OPCODES_REL)
                            {
                                XBinary::_MEMORY_MAP memoryMap=XFormats::getMemoryMap(sh.fileType,&file);
                                qint64 nEntryPointAddress=XFormats::getEntryPointAddress(sh.fileType,&file);

                                sFolderName=XCapstone::getSignature(&file,&memoryMap,nEntryPointAddress,XCapstone::ST_MASKREL,nUnknownCount);
                            }

                            file.close();
                        }

                        if(sFolderName!="")
                        {
                            _sFileName+=QDir::separator()+sFolderName;
                        }
                    }

                    XBinary::createDirectory(_sFileName);

                    _sFileName+=QDir::separator()+_sBaseFileName;

                    bool bCopyEnable=true;

                    if(_pOptions->overlay==OVERLAY_PRESENT)
                    {
                        if(bCopyEnable)
                        {
                            bCopyEnable=(bIsOverlayPresent);
                        }
                    }
                    else if(_pOptions->overlay==OVERLAY_NOTPRESENT)
                    {
                        if(bCopyEnable)
                        {
                            bCopyEnable=(!bIsOverlayPresent);
                        }
                    }

                    if(_pOptions->entropy==ENTROPY_MORETHAN)
                    {
                        if(bCopyEnable)
                        {
                            bCopyEnable=(XBinary::getEntropy(scanResult.sFileName)>=_pOptions->dEntropyValue);
                        }
                    }
                    else if(_pOptions->entropy==ENTROPY_LESSTHAN)
                    {
                        if(bCopyEnable)
                        {
                            bCopyEnable=(XBinary::getEntropy(scanResult.sFileName)<=_pOptions->dEntropyValue);
                        }
                    }

                    if(bCopyEnable)
                    {
                        if(XBinary::copyFile(scanResult.sFileName,_sFileName))
                        {
                            bGlobalCopy=true;

                            setFileCount(nCRC,nCurrentCount+1);
                        }
                    }
                }
            }
        }

        if(scanResult.listErrors.count())
        {
            QString _sFileName=_pOptions->sResultDirectory;

            XBinary::createDirectory(_sFileName);
            _sFileName+=QDir::separator()+QString("__ERRORS");
            XBinary::createDirectory(_sFileName);
            _sFileName+=QDir::separator()+XBinary::getBaseFileName(scanResult.sFileName);

            XBinary::copyFile(scanResult.sFileName,_sFileName);
        }

        setFileStat(scanResult.sFileName,QString::number(scanResult.nScanTime),QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

        if(_pOptions->bDebug)
        {
            XBinary::removeFile(sTempFile);
        }

        if((_pOptions->bRemoveCopied)&&bGlobalCopy)
        {
            XBinary::removeFile(scanResult.sFileName);
        }
    }

    mutexStats.lock();
    currentStats.stFiles.remove(sFileName);
    stScripts.remove(&dieScript);
    mutexStats.unlock();

    pSemaphore->release();
}

QString ScanProgress::createPath(ScanProgress::CF copyFormat, DiE_Script::SCAN_HEADER scanHeader)
{
    QString sResult;

    if(     (copyFormat==ScanProgress::CF_FT_TYPE_NAME)||
            (copyFormat==ScanProgress::CF_FT_TYPE_NAME_EP)||
            (copyFormat==ScanProgress::CF_FT_TYPE_NAME_EPREL))
    {
        sResult=XBinary::fileTypeIdToString(scanHeader.fileType);
    }
    else if((copyFormat==ScanProgress::CF_FT_ARCH_TYPE_NAME)||
            (copyFormat==ScanProgress::CF_FT_ARCH_TYPE_NAME_EP)||
            (copyFormat==ScanProgress::CF_FT_ARCH_TYPE_NAME_EPREL))
    {
        sResult=XBinary::fileTypeIdToString(scanHeader.fileType)+QDir::separator()+scanHeader.sArch;
    }
    else if((copyFormat==ScanProgress::CF_ARCH_FT_TYPE_NAME)||
            (copyFormat==ScanProgress::CF_ARCH_FT_TYPE_NAME_EP)||
            (copyFormat==ScanProgress::CF_ARCH_FT_TYPE_NAME_EPREL))
    {
        sResult=scanHeader.sArch+QDir::separator()+XBinary::fileTypeIdToString(scanHeader.fileType);
    }

    // TODO

//    CF_FT_TYPE_NAME_EP,
//    CF_ARCH_FT_TYPE_NAME_EP,
//    CF_FT_ARCH_TYPE_NAME_EP,
//    CF_FT_TYPE_NAME_EPREL,
//    CF_ARCH_FT_TYPE_NAME_EPREL,
//    CF_FT_ARCH_TYPE_NAME_EPREL

    return sResult;
}

void ScanProgress::process()
{
    pSemaphore=new QSemaphore(N_MAXNUMBEROFTHREADS);
    pElapsedTimer=new QElapsedTimer;
    pElapsedTimer->start();

    if(!(_pOptions->bContinue))
    {
        createTables();
    }
    currentStats.nTotal=0;
    currentStats.nCurrent=0;

    bIsStop=false;

    currentStats.sStatus=tr("Directory scan");

    if(!(_pOptions->bContinue))
    {
        startTransaction();

        findFiles(_sDirectoryName);

        endTransaction();
    }

    currentStats.nTotal=getNumberOfFile();

    dieScript.loadDatabase(_pOptions->sSignatures);

    while(!bIsStop)
    {
        QString sFileName=getCurrentFileNameAndLock();

        if(currentStats.sStatus=="")
        {
            break;
        }

        QFuture<void> future=QtConcurrent::run(this,&ScanProgress::_processFile,sFileName);

        QThread::msleep(50);

        while(true)
        {
            int nAvailable=pSemaphore->available();
            currentStats.nNumberOfThreads=N_MAXNUMBEROFTHREADS-nAvailable;
            if(nAvailable)
            {
                break;
            }

            QThread::msleep(500);
        }
    }

    while(true)
    {
        int nAvailable=pSemaphore->available();
        currentStats.nNumberOfThreads=N_MAXNUMBEROFTHREADS-nAvailable;

        if(nAvailable==N_MAXNUMBEROFTHREADS)
        {
            break;
        }

        QThread::msleep(1000);
    }

    delete pSemaphore;

    emit completed(pElapsedTimer->elapsed());
    delete pElapsedTimer;
    pElapsedTimer=nullptr;

    bIsStop=false;
}

void ScanProgress::stop()
{
    bIsStop=true;

    mutexStats.lock();
    QList<DiE_Script *> listScripts=stScripts.toList();

    for(int i=0;i<listScripts.count();i++)
    {
        listScripts.at(i)->stop();
    }

    mutexStats.unlock();
}

ScanProgress::STATS ScanProgress::getCurrentStats()
{
    if(pElapsedTimer)
    {
        currentStats.nElapsed=pElapsedTimer->elapsed();
    }

    return currentStats;
}

bool ScanProgress::createDatabase(QSqlDatabase *pDb, QString sDatabaseName)
{
    bool bResult=false;

    *pDb=QSqlDatabase::addDatabase("QSQLITE", "sqllite");
    pDb->setDatabaseName(sDatabaseName);

    if(pDb->open())
    {
        QSqlQuery query(*pDb);

        query.exec("PRAGMA journal_mode = WAL");
        query.exec("PRAGMA synchronous = NORMAL");

        bResult=true;
    }

    return bResult;
}
