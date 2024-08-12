/* Copyright (c) 2019-2023 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "scanprogress.h"

ScanProgress::ScanProgress(QObject *pParent) : QObject(pParent)
{
    _pOptions = nullptr;
    pSemaphore = nullptr;
    g_pPdStruct = nullptr;
    g_nFreeIndex = 0;
}

void ScanProgress::setData(QString sDirectoryName, ScanProgress::SCAN_OPTIONS *pOptions, XBinary::PDSTRUCT *pPdStruct)
{
    this->_sDirectoryName = sDirectoryName;
    this->_pOptions = pOptions;
    this->g_pPdStruct = pPdStruct;
}

quint32 ScanProgress::getFileCount(quint32 nCRC)
{
    QMutexLocker locker(&mutexDB);

    quint32 nResult = 0;

    QSqlQuery query(_pOptions->dbSQLLite);

    query.exec(QString("SELECT FILECOUNT FROM records where FILECRC='%1'").arg(nCRC));

    if (query.next()) {
        nResult = query.value("FILECOUNT").toString().trimmed().toUInt();
    }

    if (query.lastError().text().trimmed() != "") {
        qDebug("%s", query.lastQuery().toLatin1().data());
        qDebug("%s", query.lastError().text().toLatin1().data());
    }

    return nResult;
}

void ScanProgress::setFileCount(quint32 nCRC, quint32 nCount)
{
    QMutexLocker locker(&mutexDB);

    QSqlQuery query(_pOptions->dbSQLLite);

    query.exec(QString("INSERT OR REPLACE INTO records(FILECRC,FILECOUNT) VALUES('%1','%2')").arg(nCRC).arg(nCount));

    if (query.lastError().text().trimmed() != "") {
        qDebug("%s", query.lastQuery().toLatin1().data());
        qDebug("%s", query.lastError().text().toLatin1().data());
    }
}

void ScanProgress::setFileStat(QString sFileName, QString sTimeCount, QString sDate)
{
    QMutexLocker locker(&mutexDB);

    QSqlQuery query(_pOptions->dbSQLLite);

    query.exec(QString("INSERT OR REPLACE INTO files(FILENAME,TIMECOUNT,DATETIME) VALUES('%1','%2','%3')").arg(sFileName.replace("'", "''")).arg(sTimeCount).arg(sDate));

    // QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")

    if (query.lastError().text().trimmed() != "") {
        qDebug("%s", query.lastQuery().toLatin1().data());
        qDebug("%s", query.lastError().text().toLatin1().data());
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

    if (query.next()) {
        sResult = query.value("FILENAME").toString().trimmed();
    }

    if (query.lastError().text().trimmed() != "") {
        qDebug("%s", query.lastQuery().toLatin1().data());
        qDebug("%s", query.lastError().text().toLatin1().data());
    }

    return sResult;
}

QString ScanProgress::getCurrentFileNameAndLock()
{
    QMutexLocker locker(&mutexDB);

    QString sResult;

    QSqlQuery query(_pOptions->dbSQLLite);

    query.exec(QString("SELECT FILENAME FROM files where TIMECOUNT='' AND DATETIME='' LIMIT 1"));

    if (query.next()) {
        sResult = query.value("FILENAME").toString().trimmed();
    }

    if (query.lastError().text().trimmed() != "") {
        qDebug("%s", query.lastQuery().toLatin1().data());
        qDebug("%s", query.lastError().text().toLatin1().data());
    }

    query.exec(QString("INSERT OR REPLACE INTO files(FILENAME,TIMECOUNT,DATETIME) VALUES('%1','%2','%3')")
                   .arg(sResult.replace("'", "''"))
                   .arg(0)
                   .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));

    if (query.lastError().text().trimmed() != "") {
        qDebug("%s", query.lastQuery().toLatin1().data());
        qDebug("%s", query.lastError().text().toLatin1().data());
    }

    return sResult;
}

qint64 ScanProgress::getNumberOfFile()
{
    QMutexLocker locker(&mutexDB);

    qint64 nResult = 0;

    QSqlQuery query(_pOptions->dbSQLLite);

    query.exec(QString("SELECT COUNT(FILENAME) AS VALUE FROM files where TIMECOUNT='' AND DATETIME=''"));

    if (query.next()) {
        nResult = query.value("VALUE").toULongLong();
    }

    return nResult;
}

void ScanProgress::findFiles(QString sDirectoryName)
{
    if (!(g_pPdStruct->bIsStop)) {
        XBinary::setPdStructStatus(g_pPdStruct, g_nFreeIndex, sDirectoryName);

        QFileInfo fi(sDirectoryName);

        if (fi.isFile()) {
            //            g_pPdStruct->pdRecordFiles.nTotal++;
            setFileStat(fi.absoluteFilePath(), "", "");
        } else if (fi.isDir() && (_pOptions->bSubdirectories)) {
            QDir dir(sDirectoryName);

            QFileInfoList eil = dir.entryInfoList();

            int nCount = eil.count();

            XBinary::setPdStructCurrent(g_pPdStruct, g_nFreeIndex, nCount);

            for (int i = 0; (i < nCount) && (!(g_pPdStruct->bIsStop)); i++) {
                QString sFN = eil.at(i).fileName();

                if ((sFN != ".") && (sFN != "..")) {
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
    if (_pOptions->nThreads > 1) {
        pSemaphore->acquire();
    }

    if (sFileName != "") {
        QSet<XBinary::FT> fileTypes = XFormats::getFileTypes(sFileName, true, g_pPdStruct);

        XBinary::FT ftPref = XBinary::_getPrefFileType(&fileTypes);

        bool bProcess = _pOptions->stFileTypes.contains(ftPref);
        bool bIsOverlayPresent = false;
        bool bIsValid = false;

        if (bProcess) {
            QFile file;

            file.setFileName(sFileName);

            if (file.open(QIODevice::ReadOnly)) {
                if ((ftPref == XBinary::FT_PE32) || (ftPref == XBinary::FT_PE64)) {
                    XPE pe(&file);

                    bIsOverlayPresent = pe.isOverlayPresent();
                    bIsValid = pe.checkFileFormat(XPE::CFF_ENTRYPOINT, nullptr, g_pPdStruct);
                } else if ((ftPref == XBinary::FT_ELF32) || (ftPref == XBinary::FT_ELF64)) {
                    XELF elf(&file);

                    bIsOverlayPresent = elf.isOverlayPresent();
                    bIsValid = elf.checkFileFormat(0, nullptr, g_pPdStruct);
                } else if ((ftPref == XBinary::FT_MACHO32) || (ftPref == XBinary::FT_MACHO64)) {
                    XMACH mach(&file);

                    bIsOverlayPresent = mach.isOverlayPresent();
                    bIsValid = mach.checkFileFormat(0, nullptr, g_pPdStruct);
                } else if ((ftPref == XBinary::FT_LE) || (ftPref == XBinary::FT_LX)) {
                    XLE le(&file);

                    bIsOverlayPresent = le.isOverlayPresent();
                    bIsValid = le.checkFileFormat(0, nullptr, g_pPdStruct);
                } else if (ftPref == XBinary::FT_NE) {
                    XNE ne(&file);

                    bIsOverlayPresent = ne.isOverlayPresent();
                    bIsValid = ne.checkFileFormat(0, nullptr, g_pPdStruct);
                } else if (ftPref == XBinary::FT_MSDOS) {
                    XMSDOS msdos(&file);

                    bIsOverlayPresent = msdos.isOverlayPresent();
                    bIsValid = msdos.checkFileFormat(0, nullptr, g_pPdStruct);
                } else {
                    XBinary binary(&file);

                    bIsOverlayPresent = binary.isOverlayPresent();
                    bIsValid = binary.checkFileFormat(0, nullptr, g_pPdStruct);
                }
            }
        }

        if (bProcess && (_pOptions->bValidOnly)) {
            bProcess = bIsValid;
        }

        if (_pOptions->overlay == OVERLAY_PRESENT) {
            if (bProcess) {
                bProcess = (bIsOverlayPresent);
            }
        } else if (_pOptions->overlay == OVERLAY_NOTPRESENT) {
            if (bProcess) {
                bProcess = (!bIsOverlayPresent);
            }
        }

        if (_pOptions->entropy == ENTROPY_MORETHAN) {
            if (bProcess) {
                bProcess = (XBinary::getEntropy(sFileName) >= _pOptions->dEntropyValue);
            }
        } else if (_pOptions->entropy == ENTROPY_LESSTHAN) {
            if (bProcess) {
                bProcess = (XBinary::getEntropy(sFileName) <= _pOptions->dEntropyValue);
            }
        }

        if (bProcess) {
            QString sTempFile;

            if (_pOptions->bDebug) {
                sTempFile = _pOptions->sResultDirectory;

                XBinary::createDirectory(sTempFile);

                sTempFile += QDir::separator() + QFileInfo(sFileName).fileName();

                XBinary::copyFile(sFileName, sTempFile);
            }

            XScanEngine::SCAN_OPTIONS scanOptions = {};

            scanOptions.bIsDeepScan = _pOptions->bIsDeepScan;
            scanOptions.bIsVerbose = _pOptions->bIsVerbose;
            scanOptions.bIsRecursiveScan = _pOptions->bIsRecursive;
            scanOptions.bIsHeuristicScan = _pOptions->bIsHeuristicScan;
            scanOptions.bShowType = true;
            scanOptions.bShowVersion = _pOptions->bShowVersion;
            scanOptions.bShowInfo = _pOptions->bShowInfo;
            scanOptions.nBufferSize = 8 * 1024 * 1024;
            scanOptions.bUseExtraDatabase = _pOptions->bSignaturesExtraUse;
            scanOptions.bUseCustomDatabase = _pOptions->bSignaturesCustomUse;
            // scanOptions.bShowUnknown = false;

            setFileStat(sFileName, "", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

            DiE_Script::SCAN_RESULT scanResult = dieScript.scanFile(sFileName, &scanOptions, g_pPdStruct);

            QString _sBaseFileName = QFileInfo(scanResult.sFileName).fileName();

            if ((_pOptions->fileFormat == FF_MD5) || (_pOptions->fileFormat == FF_MD5_ORIGINAL)) {
                QString sMD5 = XBinary::getHash(XBinary::HASH_MD5, scanResult.sFileName, g_pPdStruct);

                if (_pOptions->fileFormat == FF_MD5) {
                    _sBaseFileName = sMD5;
                } else if (_pOptions->fileFormat == FF_MD5_ORIGINAL) {
                    _sBaseFileName = sMD5 + _sBaseFileName;
                }
            }

            int nCount = scanResult.listRecords.count();

            bool bGlobalCopy = false;
            bool bIdentified = false;

            QString sEPBytes;
            QString sEP;
            QString sEPREL;
            QString sImport;

            XScanEngine::SCANID _id = {};

            if (nCount) {
                if (_pOptions->bCopyTheFirstOnly) {
                    nCount = 1;
                }

                for (int i = 0; (i < nCount) && (!(g_pPdStruct->bIsStop)); i++) {
                    XScanEngine::SCANSTRUCT ss = scanResult.listRecords.at(i);
                    _id = ss.id;

                    if (_pOptions->stFileTypes.contains(ss.id.fileType) && (ss.sName != "") && (!ss.bIsUnknown)) {
                        if ((_pOptions->copyFormat == ScanProgress::CF_ARCH_FT_TYPE_NAME_EPBYTES) ||
                            (_pOptions->copyFormat == ScanProgress::CF_FT_ARCH_TYPE_NAME_EPBYTES) || (_pOptions->copyFormat == ScanProgress::CF_FT_TYPE_NAME_EPBYTES) ||
                            (_pOptions->copyFormat == ScanProgress::CF_ARCH_FT_TYPE_NAME_EPSIG) || (_pOptions->copyFormat == ScanProgress::CF_FT_ARCH_TYPE_NAME_EPSIG) ||
                            (_pOptions->copyFormat == ScanProgress::CF_FT_TYPE_NAME_EPSIG) || (_pOptions->copyFormat == ScanProgress::CF_ARCH_FT_TYPE_NAME_EPSIGREL) ||
                            (_pOptions->copyFormat == ScanProgress::CF_FT_ARCH_TYPE_NAME_EPSIGREL) || (_pOptions->copyFormat == ScanProgress::CF_FT_TYPE_NAME_EPSIGREL)) {
                            QFile _file;
                            _file.setFileName(sFileName);

                            if (_file.open(QIODevice::ReadOnly)) {
                                XBinary::_MEMORY_MAP memoryMap = XFormats::getMemoryMap(ss.id.fileType, XBinary::MAPMODE_UNKNOWN, &_file, g_pPdStruct);
                                qint64 nEPAddress = XFormats::getEntryPointAddress(ss.id.fileType, &_file);
                                qint64 nEPOffset = XFormats::getEntryPointOffset(ss.id.fileType, &_file);

                                sEPBytes = XBinary::getSignature(&_file, nEPOffset, 20);
                                sEP = XCapstone::getSignature(&_file, &memoryMap, nEPAddress, XCapstone::ST_MASK, 10).replace(".", "_");
                                sEPREL = XCapstone::getSignature(&_file, &memoryMap, nEPAddress, XCapstone::ST_MASKREL, 10).replace(".", "_");

                                _file.close();
                            }
                        } else if (_pOptions->copyFormat == ScanProgress::CF_FT_ARCH_TYPE_NAME_IMPORT) {
                            sImport = "";

                            if ((_id.fileType == XBinary::FT_PE32) || (_id.fileType == XBinary::FT_PE64)) {
                                QFile _file;
                                _file.setFileName(sFileName);

                                if (_file.open(QIODevice::ReadOnly)) {
                                    XPE pe(&_file);

                                    if (pe.isValid()) {
                                        QList<quint32> listImportHashes = pe.getImportPositionHashes();

                                        if (listImportHashes.count()) {
                                            sImport += XBinary::valueToHex(listImportHashes.at(0));
                                            sImport += "_";
                                        }

                                        QList<XPE::IMPORT_RECORD> listImportRecords = pe.getImportRecords();

                                        sImport += XBinary::valueToHex(pe.getImportHash32(&listImportRecords));
                                        sImport += "_";
                                        sImport += XBinary::valueToHex(pe.getImportHash64(&listImportRecords));
                                    }

                                    _file.close();
                                }
                            }
                        }

                        if (((_pOptions->stTypes.contains(ss.sType.toLower())) || (_pOptions->bAllTypes)) && (!ss.bIsUnknown)) {
                            bIdentified = true;

                            if ((_pOptions->copyType == CT_IDENT) || (_pOptions->copyType == CT_IDENT_UNK)) {
                                QString sResult = XScanEngine::createResultStringEx(&scanOptions, &ss);

                                sResult = XBinary::convertFileNameSymbols(sResult);

                                quint32 nCRC = XBinary::getStringCustomCRC32(XBinary::fileTypeIdToString(ss.id.fileType) + ss.id.sArch + ss.sType + sResult);

                                bool bCopy = true;

                                int nCurrentCount = getFileCount(nCRC);

                                if (_pOptions->nCopyCount) {
                                    if (nCurrentCount >= _pOptions->nCopyCount) {
                                        bCopy = false;
                                    }
                                }

                                if (bCopy) {
                                    QString _sFileName = _pOptions->sResultDirectory + QDir::separator() + createPath(_pOptions->copyFormat, ss.id) + QDir::separator() +
                                                         ss.sType + QDir::separator() + sResult;

                                    if ((_pOptions->copyFormat == ScanProgress::CF_ARCH_FT_TYPE_NAME_EPBYTES) ||
                                        (_pOptions->copyFormat == ScanProgress::CF_FT_ARCH_TYPE_NAME_EPBYTES) ||
                                        (_pOptions->copyFormat == ScanProgress::CF_FT_TYPE_NAME_EPBYTES)) {
                                        _sFileName += QDir::separator() + sEPBytes;
                                    } else if ((_pOptions->copyFormat == ScanProgress::CF_ARCH_FT_TYPE_NAME_EPSIG) ||
                                               (_pOptions->copyFormat == ScanProgress::CF_FT_ARCH_TYPE_NAME_EPSIG) ||
                                               (_pOptions->copyFormat == ScanProgress::CF_FT_TYPE_NAME_EPSIG)) {
                                        _sFileName += QDir::separator() + sEP;
                                    } else if ((_pOptions->copyFormat == ScanProgress::CF_ARCH_FT_TYPE_NAME_EPSIGREL) ||
                                               (_pOptions->copyFormat == ScanProgress::CF_FT_ARCH_TYPE_NAME_EPSIGREL) ||
                                               (_pOptions->copyFormat == ScanProgress::CF_FT_TYPE_NAME_EPSIGREL)) {
                                        _sFileName += QDir::separator() + sEPREL;
                                    } else if (_pOptions->copyFormat == ScanProgress::CF_FT_ARCH_TYPE_NAME_IMPORT) {
                                        if (sImport != "") {
                                            _sFileName += QDir::separator() + sImport;
                                        }
                                    }

                                    XBinary::createDirectory(_sFileName);

                                    _sFileName += QDir::separator() + _sBaseFileName;

                                    if (XBinary::copyFile(scanResult.sFileName, _sFileName)) {
                                        bGlobalCopy = true;

                                        setFileCount(nCRC, nCurrentCount + 1);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if ((!bIdentified) && ((_pOptions->copyType == CT_IDENT_UNK) || (_pOptions->copyType == CT_UNK))) {
                if (_pOptions->stFileTypes.contains(_id.fileType)) {
                    quint32 nCRC = XBinary::getStringCustomCRC32(XBinary::fileTypeIdToString(_id.fileType) + _id.sArch + "__UNKNOWN");

                    bool bCopy = true;

                    int nCurrentCount = getFileCount(nCRC);

                    if (_pOptions->nCopyCount) {
                        if (nCurrentCount >= _pOptions->nCopyCount) {
                            bCopy = false;
                        }
                    }

                    if (bCopy) {
                        QString _sFileName = _pOptions->sResultDirectory + QDir::separator() + createPath(_pOptions->copyFormat, _id) + QDir::separator() + "__UNKNOWN";

                        if ((_pOptions->copyFormat == ScanProgress::CF_ARCH_FT_TYPE_NAME_EPBYTES) ||
                            (_pOptions->copyFormat == ScanProgress::CF_FT_ARCH_TYPE_NAME_EPBYTES) || (_pOptions->copyFormat == ScanProgress::CF_FT_TYPE_NAME_EPBYTES)) {
                            _sFileName += QDir::separator() + sEPBytes;
                        } else if ((_pOptions->copyFormat == ScanProgress::CF_ARCH_FT_TYPE_NAME_EPSIG) ||
                                   (_pOptions->copyFormat == ScanProgress::CF_FT_ARCH_TYPE_NAME_EPSIG) ||
                                   (_pOptions->copyFormat == ScanProgress::CF_FT_TYPE_NAME_EPSIG)) {
                            _sFileName += QDir::separator() + sEP;
                        } else if ((_pOptions->copyFormat == ScanProgress::CF_ARCH_FT_TYPE_NAME_EPSIGREL) ||
                                   (_pOptions->copyFormat == ScanProgress::CF_FT_ARCH_TYPE_NAME_EPSIGREL) ||
                                   (_pOptions->copyFormat == ScanProgress::CF_FT_TYPE_NAME_EPSIGREL)) {
                            _sFileName += QDir::separator() + sEPREL;
                        } else if (_pOptions->copyFormat == ScanProgress::CF_FT_ARCH_TYPE_NAME_IMPORT) {
                            if (sImport != "") {
                                _sFileName += QDir::separator() + sImport;
                            }
                        }

                        if (_pOptions->unknownPrefix != UP_NONE) {
                            QString sFolderName;
                            qint32 nUnknownCount = _pOptions->nUnknownCount;

                            QFile file;

                            file.setFileName(scanResult.sFileName);

                            if (file.open(QIODevice::ReadOnly)) {
                                if ((_id.fileType == XBinary::FT_PE32) || (_id.fileType == XBinary::FT_PE64)) {
                                    XPE pe(&file);

                                    if (_pOptions->unknownPrefix == UP_EP_BYTES) {
                                        sFolderName = pe.getSignature(pe._getEntryPointOffset(), nUnknownCount);
                                    } else if (_pOptions->unknownPrefix == UP_OVERLAY_BYTES) {
                                        if (bIsOverlayPresent) {
                                            sFolderName = pe.getSignature(pe.getOverlayOffset(), nUnknownCount);
                                        }
                                    } else if (_pOptions->unknownPrefix == UP_HEADER_BYTES) {
                                        sFolderName = pe.getSignature(0, nUnknownCount);
                                    }
                                } else if ((_id.fileType == XBinary::FT_ELF32) || (_id.fileType == XBinary::FT_ELF64)) {
                                    XELF elf(&file);

                                    if (_pOptions->unknownPrefix == UP_EP_BYTES) {
                                        sFolderName = elf.getSignature(elf._getEntryPointOffset(), nUnknownCount);
                                    } else if (_pOptions->unknownPrefix == UP_OVERLAY_BYTES) {
                                        if (bIsOverlayPresent) {
                                            sFolderName = elf.getSignature(elf.getOverlayOffset(), nUnknownCount);
                                        }
                                    } else if (_pOptions->unknownPrefix == UP_HEADER_BYTES) {
                                        sFolderName = elf.getSignature(0, nUnknownCount);
                                    }
                                } else if ((_id.fileType == XBinary::FT_MACHO32) || (_id.fileType == XBinary::FT_MACHO64)) {
                                    XMACH mach(&file);

                                    if (_pOptions->unknownPrefix == UP_EP_BYTES) {
                                        sFolderName = mach.getSignature(mach._getEntryPointOffset(), nUnknownCount);
                                    } else if (_pOptions->unknownPrefix == UP_OVERLAY_BYTES) {
                                        if (bIsOverlayPresent) {
                                            sFolderName = mach.getSignature(mach.getOverlayOffset(), nUnknownCount);
                                        }
                                    } else if (_pOptions->unknownPrefix == UP_HEADER_BYTES) {
                                        sFolderName = mach.getSignature(0, nUnknownCount);
                                    }
                                } else if ((_id.fileType == XBinary::FT_LE) || (_id.fileType == XBinary::FT_LX)) {
                                    XLE le(&file);

                                    if (_pOptions->unknownPrefix == UP_EP_BYTES) {
                                        sFolderName = le.getSignature(le._getEntryPointOffset(), nUnknownCount);
                                    } else if (_pOptions->unknownPrefix == UP_OVERLAY_BYTES) {
                                        if (bIsOverlayPresent) {
                                            sFolderName = le.getSignature(le.getOverlayOffset(), nUnknownCount);
                                        }
                                    } else if (_pOptions->unknownPrefix == UP_HEADER_BYTES) {
                                        sFolderName = le.getSignature(0, nUnknownCount);
                                    }
                                } else if (_id.fileType == XBinary::FT_NE) {
                                    XNE ne(&file);

                                    if (_pOptions->unknownPrefix == UP_EP_BYTES) {
                                        sFolderName = ne.getSignature(ne._getEntryPointOffset(), nUnknownCount);
                                    } else if (_pOptions->unknownPrefix == UP_OVERLAY_BYTES) {
                                        if (bIsOverlayPresent) {
                                            sFolderName = ne.getSignature(ne.getOverlayOffset(), nUnknownCount);
                                        }
                                    } else if (_pOptions->unknownPrefix == UP_HEADER_BYTES) {
                                        sFolderName = ne.getSignature(0, nUnknownCount);
                                    }
                                } else if (_id.fileType == XBinary::FT_MSDOS) {
                                    XMSDOS msdos(&file);

                                    if (_pOptions->unknownPrefix == UP_EP_BYTES) {
                                        sFolderName = msdos.getSignature(msdos._getEntryPointOffset(), nUnknownCount);
                                    } else if (_pOptions->unknownPrefix == UP_OVERLAY_BYTES) {
                                        if (bIsOverlayPresent) {
                                            sFolderName = msdos.getSignature(msdos.getOverlayOffset(), nUnknownCount);
                                        }
                                    } else if (_pOptions->unknownPrefix == UP_HEADER_BYTES) {
                                        sFolderName = msdos.getSignature(0, nUnknownCount);
                                    }
                                } else {
                                    XBinary binary(&file);

                                    if (_pOptions->unknownPrefix == UP_EP_BYTES) {
                                        sFolderName = binary.getSignature(binary._getEntryPointOffset(), nUnknownCount);
                                    } else if (_pOptions->unknownPrefix == UP_OVERLAY_BYTES) {
                                        if (bIsOverlayPresent) {
                                            sFolderName = binary.getSignature(binary.getOverlayOffset(), nUnknownCount);
                                        }
                                    } else if (_pOptions->unknownPrefix == UP_HEADER_BYTES) {
                                        sFolderName = binary.getSignature(0, nUnknownCount);
                                    }
                                }

                                if (_pOptions->unknownPrefix == UP_OPCODES) {
                                    XBinary::_MEMORY_MAP memoryMap = XFormats::getMemoryMap(_id.fileType, XBinary::MAPMODE_UNKNOWN, &file);
                                    qint64 nEntryPointAddress = XFormats::getEntryPointAddress(_id.fileType, &file);

                                    sFolderName = XCapstone::getSignature(&file, &memoryMap, nEntryPointAddress, XCapstone::ST_MASK, nUnknownCount);
                                } else if (_pOptions->unknownPrefix == UP_OPCODES_REL) {
                                    XBinary::_MEMORY_MAP memoryMap = XFormats::getMemoryMap(_id.fileType, XBinary::MAPMODE_UNKNOWN, &file);
                                    qint64 nEntryPointAddress = XFormats::getEntryPointAddress(_id.fileType, &file);

                                    sFolderName = XCapstone::getSignature(&file, &memoryMap, nEntryPointAddress, XCapstone::ST_MASKREL, nUnknownCount);
                                }

                                file.close();
                            }

                            sFolderName = sFolderName.replace(".", "_");

                            if (sFolderName != "") {
                                _sFileName += QDir::separator() + sFolderName;
                            }
                        }

                        XBinary::createDirectory(_sFileName);

                        _sFileName += QDir::separator() + _sBaseFileName;

                        if (XBinary::copyFile(scanResult.sFileName, _sFileName)) {
                            bGlobalCopy = true;

                            setFileCount(nCRC, nCurrentCount + 1);
                        } else {
#ifdef QT_DEBUG
                            qDebug("%s", _sFileName.toUtf8().data());
#endif
                        }
                    }
                }
            }

            if (scanResult.listErrors.count()) {
                QString _sFileName = _pOptions->sResultDirectory;
                _sFileName = XBinary::convertFileName(_sFileName);
                XBinary::createDirectory(_sFileName);
                _sFileName += QDir::separator() + QString("__ERRORS");
                _sFileName = XBinary::convertFileName(_sFileName);
                XBinary::createDirectory(_sFileName);
                _sFileName += QDir::separator() + QFileInfo(sFileName).fileName();

                XBinary::copyFile(scanResult.sFileName, _sFileName);
            }

            setFileStat(scanResult.sFileName, QString::number(scanResult.nScanTime), QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

            if (_pOptions->bDebug) {
                XBinary::removeFile(sTempFile);
            }

            if ((_pOptions->bRemoveCopied) && bGlobalCopy) {
                XBinary::removeFile(scanResult.sFileName);
            }
        }
    }

    if (_pOptions->nThreads > 1) {
        pSemaphore->release();
    }
}

QString ScanProgress::createPath(ScanProgress::CF copyFormat, XScanEngine::SCANID scanID)
{
    QString sResult;

    if ((copyFormat == ScanProgress::CF_FT_TYPE_NAME) || (copyFormat == ScanProgress::CF_FT_TYPE_NAME_EPSIG) || (copyFormat == ScanProgress::CF_FT_TYPE_NAME_EPSIGREL)) {
        sResult = XBinary::fileTypeIdToString(scanID.fileType);
    } else if ((copyFormat == ScanProgress::CF_FT_ARCH_TYPE_NAME) || (copyFormat == ScanProgress::CF_FT_ARCH_TYPE_NAME_EPSIG) ||
               (copyFormat == ScanProgress::CF_FT_ARCH_TYPE_NAME_EPSIGREL) || (copyFormat == ScanProgress::CF_FT_ARCH_TYPE_NAME_IMPORT)) {
        sResult = XBinary::fileTypeIdToString(scanID.fileType) + QDir::separator() + scanID.sArch;
    } else if ((copyFormat == ScanProgress::CF_ARCH_FT_TYPE_NAME) || (copyFormat == ScanProgress::CF_ARCH_FT_TYPE_NAME_EPSIG) ||
               (copyFormat == ScanProgress::CF_ARCH_FT_TYPE_NAME_EPSIGREL)) {
        sResult = scanID.sArch + QDir::separator() + XBinary::fileTypeIdToString(scanID.fileType);
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
    QElapsedTimer scanTimer;
    scanTimer.start();

    if (_pOptions->nThreads > 1) {
        pSemaphore = new QSemaphore(_pOptions->nThreads);
    }

    if (!(_pOptions->bContinue)) {
        createTables();
    }

    if (!(_pOptions->bContinue)) {
        startTransaction();

        g_nFreeIndex = XBinary::getFreeIndex(g_pPdStruct);
        XBinary::setPdStructInit(g_pPdStruct, g_nFreeIndex, 0);

        findFiles(_sDirectoryName);

        XBinary::setPdStructFinished(g_pPdStruct, g_nFreeIndex);

        endTransaction();
    }

    qint32 _nFreeIndex = XBinary::getFreeIndex(g_pPdStruct);
    XBinary::setPdStructInit(g_pPdStruct, _nFreeIndex, getNumberOfFile());

    dieScript.loadDatabase(_pOptions->sSignatures, DiE_ScriptEngine::DT_MAIN, g_pPdStruct);
    dieScript.loadDatabase(_pOptions->sSignaturesExtra, DiE_ScriptEngine::DT_EXTRA, g_pPdStruct);
    dieScript.loadDatabase(_pOptions->sSignaturesCustom, DiE_ScriptEngine::DT_CUSTOM, g_pPdStruct);

    qint32 _nCurrent = 0;

    while (!(g_pPdStruct->bIsStop)) {
        QString sFileName = getCurrentFileNameAndLock();
        XBinary::setPdStructStatus(g_pPdStruct, _nFreeIndex, sFileName);

        if (sFileName == "") {
            break;
        }

        if (_pOptions->nThreads > 1) {
            QFuture<void> future = QtConcurrent::run(this, &ScanProgress::_processFile, sFileName);

            QThread::msleep(50);

            while (true) {
                int nAvailable = pSemaphore->available();
                //            currentStats.nNumberOfThreads=(_pOptions->nThreads)-nAvailable;
                if (nAvailable) {
                    break;
                }

                QThread::msleep(500);
            }
        } else {
            _processFile(sFileName);
        }

        _nCurrent++;

        XBinary::setPdStructCurrent(g_pPdStruct, _nFreeIndex, _nCurrent);
    }

    if (_pOptions->nThreads > 1) {
        while (true) {
            int nAvailable = pSemaphore->available();
            //        currentStats.nNumberOfThreads=(_pOptions->nThreads)-nAvailable;

            if (nAvailable == (_pOptions->nThreads)) {
                break;
            }

            QThread::msleep(1000);
        }

        delete pSemaphore;
    }

    XBinary::setPdStructFinished(g_pPdStruct, _nFreeIndex);

    emit completed(scanTimer.elapsed());
}

bool ScanProgress::createDatabase(QSqlDatabase *pDb, QString sDatabaseName)
{
    bool bResult = false;

    *pDb = QSqlDatabase::addDatabase("QSQLITE", "sqllite");
    pDb->setDatabaseName(sDatabaseName);

    if (pDb->open()) {
        QSqlQuery query(*pDb);

        query.exec("PRAGMA journal_mode = WAL");
        query.exec("PRAGMA synchronous = NORMAL");

        bResult = true;
    }

    return bResult;
}
