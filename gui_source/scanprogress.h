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

#ifndef SCANPROGRESS_H
#define SCANPROGRESS_H

#include "die_script.h"
#include "xcapstone.h"

class ScanProgress : public QObject {
    Q_OBJECT

public:
    enum CF {
        CF_FT_TYPE_NAME = 0,
        CF_ARCH_FT_TYPE_NAME,
        CF_FT_ARCH_TYPE_NAME,
        CF_FT_TYPE_NAME_EPBYTES,
        CF_ARCH_FT_TYPE_NAME_EPBYTES,
        CF_FT_ARCH_TYPE_NAME_EPBYTES,
        CF_FT_TYPE_NAME_EPSIG,
        CF_ARCH_FT_TYPE_NAME_EPSIG,
        CF_FT_ARCH_TYPE_NAME_EPSIG,
        CF_FT_TYPE_NAME_EPSIGREL,
        CF_ARCH_FT_TYPE_NAME_EPSIGREL,
        CF_FT_ARCH_TYPE_NAME_EPSIGREL,
        CF_FT_ARCH_TYPE_NAME_IMPORT,
    };

    enum CT {
        CT_IDENT = 0,
        CT_IDENT_UNK,
        CT_UNK
    };

    enum UP {
        UP_NONE = 0,
        UP_EP_BYTES,
        UP_HEADER_BYTES,
        UP_OVERLAY_BYTES,
        UP_OPCODES,
        UP_OPCODES_REL,
    };

    enum FF {
        FF_ORIGINAL = 0,
        FF_MD5,
        FF_MD5_ORIGINAL,
        FF_ENTROPY,
        FF_ENTROPY_ORIGINAL,
        FF_ENTROPYPERCENTAGE,
        FF_ENTROPYPERCENTAGE_ORIGINAL,
    };

    enum OVERLAY {
        OVERLAY_NONE = 0,
        OVERLAY_PRESENT,
        OVERLAY_NOTPRESENT
    };

    enum ENTROPY {
        ENTROPY_NONE = 0,
        ENTROPY_MORETHAN,
        ENTROPY_LESSTHAN
    };

    struct SCAN_OPTIONS {
        bool bIsRecursive;
        bool bIsDeepScan;
        bool bIsHeuristicScan;
        bool bIsVerbose;
        bool bIsAggressive;
        bool bSubdirectories;
        bool bShowVersion;
        bool bShowInfo;
        bool bIsSort;
        bool bAllFileTypes;
        QSet<XBinary::FT> stFileTypes;
        bool bAllTypes;
        QSet<QString> stTypes;
        qint32 nCopyCount;
        QString sResultDirectory;
        QString sSignatures;
        QString sSignaturesExtra;
        QString sSignaturesCustom;
        bool bSignaturesExtraUse;
        bool bSignaturesCustomUse;
        bool bDebug;
        CF copyFormat;
        CT copyType;
        UP unknownPrefix;
        FF fileFormat;
        OVERLAY overlay;
        ENTROPY entropy;
        double dEntropyValue;
        qint64 nUnknownCount;
        bool bRemoveCopied;
        bool bCopyTheFirstOnly;
        bool bValidOnly;
        bool bCurrentIni;
    };

    struct STATS {
        qint32 nTotal;
        qint32 nCurrent;
        qint64 nElapsed;
        QString sStatus;
        qint32 nNumberOfThreads;
        QSet<QString> stFiles;
    };

    explicit ScanProgress(QObject *pParent = nullptr);

    void setData(QString sDirectoryName, ScanProgress::SCAN_OPTIONS *pOptions, XBinary::PDSTRUCT *pPdStruct);

    void findFiles(qint64 *pnNumberOfFiles, QString sDirectoryName);
    void scanFiles(qint64 *pnNumberOfFiles, QString sDirectoryName);

    void _processFile(QString sFileName);

    static QString createPath(CF copyFormat, XScanEngine::SCANID scanID);

signals:
    void completed(qint64 nElapsedTime);

public slots:
    void process();

private:
    // #ifdef QT_DEBUG
    //     const int N_MAXNUMBEROFTHREADS=8;
    // #else
    //     const int N_MAXNUMBEROFTHREADS=8;
    // #endif
    QString _sDirectoryName;
    SCAN_OPTIONS *_pOptions;
    DiE_Script dieScript;
    qint32 g_nFreeIndex;
    XBinary::PDSTRUCT *g_pPdStruct;
};

#endif  // SCANPROGRESS_H
