/* Copyright (c) 2019-2026 hors<horsicq@gmail.com>
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

#include "guimainwindow.h"

#include "ui_guimainwindow.h"

GuiMainWindow::GuiMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::GuiMainWindow)
{
    ui->setupUi(this);

    setWindowTitle(QString("%1 v%2").arg(X_APPLICATIONNAME, X_APPLICATIONVERSION));

    options = {0};

    ui->comboBoxCopyFormat->addItem("FileType/Type/Name");
    ui->comboBoxCopyFormat->addItem("Arch/FileType/Type/Name");
    ui->comboBoxCopyFormat->addItem("FileType/Arch/Type/Name");
    ui->comboBoxCopyFormat->addItem("FileType/Type/Name/EP bytes");
    ui->comboBoxCopyFormat->addItem("Arch/FileType/Type/Name/EP bytes");
    ui->comboBoxCopyFormat->addItem("FileType/Arch/Type/Name/EP bytes");
    ui->comboBoxCopyFormat->addItem("FileType/Type/Name/EP signature");
    ui->comboBoxCopyFormat->addItem("Arch/FileType/Type/Name/EP signature");
    ui->comboBoxCopyFormat->addItem("FileType/Arch/Type/Name/EP signature");
    ui->comboBoxCopyFormat->addItem("FileType/Type/Name/EP signature(rel)");
    ui->comboBoxCopyFormat->addItem("Arch/FileType/Type/Name/EP signature(rel)");
    ui->comboBoxCopyFormat->addItem("FileType/Arch/Type/Name/EP signature(rel)");
    ui->comboBoxCopyFormat->addItem("FileType/Arch/Type/Name/Import");

    ui->comboBoxCopyType->addItem("Identified");
    ui->comboBoxCopyType->addItem("Identified/Unknown");
    ui->comboBoxCopyType->addItem("Unknown");

    ui->comboBoxFileNameFormat->addItem("Original");
    ui->comboBoxFileNameFormat->addItem("MD5");
    ui->comboBoxFileNameFormat->addItem("MD5+Original");
    ui->comboBoxFileNameFormat->addItem("Entropy");
    ui->comboBoxFileNameFormat->addItem("Entropy+Original");
    ui->comboBoxFileNameFormat->addItem("EntropyPercentage");
    ui->comboBoxFileNameFormat->addItem("EntropyPercentage+Original");

    ui->comboBoxUnknownPrefix->addItem("NONE");
    ui->comboBoxUnknownPrefix->addItem("EP Bytes");
    ui->comboBoxUnknownPrefix->addItem("Header Bytes");
    ui->comboBoxUnknownPrefix->addItem("Overlay Bytes");
    ui->comboBoxUnknownPrefix->addItem("Opcodes");
    ui->comboBoxUnknownPrefix->addItem("Rel Opcodes");

    ui->comboBoxOverlay->addItem("");
    ui->comboBoxOverlay->addItem("Present");
    ui->comboBoxOverlay->addItem("Not present");

    ui->comboBoxEntropy->addItem("");
    ui->comboBoxEntropy->addItem("More than");
    ui->comboBoxEntropy->addItem("Less than");

    loadSettings();
}

GuiMainWindow::~GuiMainWindow()
{
    delete ui;
}

void GuiMainWindow::on_pushButtonExit_clicked()
{
    this->close();
}

void GuiMainWindow::on_pushButtonOpenDirectory_clicked()
{
    QString sInitDirectory = ui->lineEditDirectoryName->text();

    QString sDirectoryName = QFileDialog::getExistingDirectory(this, tr("Open directory") + QString("..."), sInitDirectory, QFileDialog::ShowDirsOnly);

    if (!sDirectoryName.isEmpty()) {
        ui->lineEditDirectoryName->setText(sDirectoryName);
    }
}

void GuiMainWindow::on_pushButtonOut_clicked()
{
    QString sInitDirectory = ui->lineEditOut->text();

    QString sDirectoryName = QFileDialog::getExistingDirectory(this, tr("Open directory") + QString("..."), sInitDirectory, QFileDialog::ShowDirsOnly);

    if (!sDirectoryName.isEmpty()) {
        ui->lineEditOut->setText(sDirectoryName);
    }
}

void GuiMainWindow::on_pushButtonScan_clicked()
{
    saveSettings();
    _scan();
}

void GuiMainWindow::_scan()
{
    options.nCopyCount = ui->spinBoxCopyCount->value();
    options.sCopyDirectory = ui->lineEditOut->text();
    options.sLogDirectory = ui->lineEditLog->text();

    options.stFileTypes.clear();

    options.stTypes.clear();

    options.bIsCopyEnable = ui->groupBoxCopy->isChecked();
    options.bIsLogEnable = ui->groupBoxLog->isChecked();
    options.bIsRecursive = ui->checkBoxRecursive->isChecked();
    options.bIsDeepScan = ui->checkBoxDeepScan->isChecked();
    options.bIsVerbose = ui->checkBoxVerbose->isChecked();
    options.bIsHeuristicScan = ui->checkBoxHeuristicScan->isChecked();
    options.bIsAggressive = ui->checkBoxAggressive->isChecked();
    options.bShowVersion = ui->checkBoxShowVersion->isChecked();
    options.bShowInfo = ui->checkBoxShowInfo->isChecked();
    options.bIsSort = ui->checkBoxSort->isChecked();
    options.bSubdirectories = ui->checkBoxScanSubdirectories->isChecked();
    options.sSignatures = ui->lineEditSignatures->text();
    options.sSignaturesExtra = ui->lineEditSignaturesExtra->text();
    options.sSignaturesCustom = ui->lineEditSignaturesCustom->text();
    options.bSignaturesExtraUse = ui->groupBoxSignaturesExtra->isChecked();
    options.bSignaturesCustomUse = ui->groupBoxSignaturesCustom->isChecked();

    options.copyFormat = (ScanProgress::CF)ui->comboBoxCopyFormat->currentIndex();
    options.copyType = (ScanProgress::CT)ui->comboBoxCopyType->currentIndex();
    options.bRemoveCopied = ui->checkBoxRemoveCopied->isChecked();
    options.bCopyTheFirstOnly = ui->checkBoxCopyTheFirstOnly->isChecked();
    options.bValidOnly = ui->checkBoxValidOnly->isChecked();

    options.unknownPrefix = (ScanProgress::UP)ui->comboBoxUnknownPrefix->currentIndex();
    options.nUnknownCount = ui->spinBoxUnknownCount->value();

    options.fileFormat = (ScanProgress::FF)ui->comboBoxFileNameFormat->currentIndex();
    options.overlay = (ScanProgress::OVERLAY)ui->comboBoxOverlay->currentIndex();
    options.entropy = (ScanProgress::ENTROPY)ui->comboBoxEntropy->currentIndex();
    options.dEntropyValue = ui->doubleSpinBoxEntropy->value();
    // options.nThreads = ui->spinBoxThreads->value();

    DialogScanProgress ds(this);

    ds.setData(ui->lineEditDirectoryName->text(), &options);

    ds.exec();

    //    DialogStaticScan ds(this);
    //    connect(&ds, SIGNAL(scanFileStarted(QString)),this,SLOT(scanFileStarted(QString)),Qt::DirectConnection);
    //    connect(&ds, SIGNAL(scanResult(SpecAbstract::SCAN_RESULT)),this,SLOT(scanResult(SpecAbstract::SCAN_RESULT)),Qt::DirectConnection);
    //    ds.setData(ui->lineEditDirectoryName->text(),&options);
    //    ds.exec();
}

void GuiMainWindow::_appendType(QString sType)
{
    options.stTypes.insert(sType);
    options.stTypes.insert("(heur)" + sType);
}

// void GuiMainWindow::on_checkBoxBinary_toggled(bool checked)
//{
//     QSignalBlocker blocker(ui->checkBoxAllFileTypes);

//    if(!checked)
//    {
//        ui->checkBoxAllFileTypes->setChecked(false);
//    }
//}

void GuiMainWindow::on_pushButtonInfo_clicked()
{
    QMessageBox::information(this, tr("Info"), tr("Bugreports: horsicq@gmail.com"));
}

void GuiMainWindow::on_pushButtonSignatures_clicked()
{
    QString sInitDirectory = XBinary::convertPathName(ui->lineEditSignatures->text());

    QString sDirectoryName = QFileDialog::getExistingDirectory(this, tr("Open directory") + QString("..."), sInitDirectory, QFileDialog::ShowDirsOnly);

    if (!sDirectoryName.isEmpty()) {
        ui->lineEditSignatures->setText(sDirectoryName);
    }
}

void GuiMainWindow::on_doubleSpinBoxEntropy_valueChanged(double arg1)
{
    Q_UNUSED(arg1)

    double dEntropy = ui->doubleSpinBoxEntropy->value();
    double dPercentage = (dEntropy / 8) * 100;
    ui->lineEditEntropy->setText(QString::number(dPercentage, 'f', 2));
}

void GuiMainWindow::on_pushButtonSignaturesCustom_clicked()
{
    QString sInitDirectory = XBinary::convertPathName(ui->lineEditSignaturesCustom->text());

    QString sDirectoryName = QFileDialog::getExistingDirectory(this, tr("Open directory") + QString("..."), sInitDirectory, QFileDialog::ShowDirsOnly);

    if (!sDirectoryName.isEmpty()) {
        ui->lineEditSignaturesCustom->setText(sDirectoryName);
    }
}

void GuiMainWindow::on_pushButtonSignaturesExtra_clicked()
{
    QString sInitDirectory = XBinary::convertPathName(ui->lineEditSignaturesExtra->text());

    QString sDirectoryName = QFileDialog::getExistingDirectory(this, tr("Open directory") + QString("..."), sInitDirectory, QFileDialog::ShowDirsOnly);

    if (!sDirectoryName.isEmpty()) {
        ui->lineEditSignaturesExtra->setText(sDirectoryName);
    }
}

void GuiMainWindow::loadSettings()
{
    QString sSettingsFile = QApplication::applicationDirPath() + QDir::separator();
#ifdef Q_OS_LINUX
    sSettingsFile += QString("Linux.");
#endif
    sSettingsFile += QString("%1.ini").arg(X_APPLICATIONNAME);

    QSettings settings(sSettingsFile, QSettings::IniFormat);

    ui->comboBoxCopyFormat->setCurrentIndex(settings.value("CopyFormat", 0).toInt());
    ui->comboBoxCopyType->setCurrentIndex(settings.value("CopyType", 0).toInt());
    ui->comboBoxFileNameFormat->setCurrentIndex(settings.value("FileFormat", 0).toInt());
    ui->comboBoxUnknownPrefix->setCurrentIndex(settings.value("UnknownPrefix", 0).toInt());
    ui->comboBoxOverlay->setCurrentIndex(settings.value("Overlay", 0).toInt());
    ui->comboBoxEntropy->setCurrentIndex(settings.value("Entropy", 0).toInt());

    ui->spinBoxUnknownCount->setValue(settings.value("UnknownCount", 10).toInt());
    ui->doubleSpinBoxEntropy->setValue(settings.value("EntropyValue", 6.5).toDouble());

    ui->lineEditDirectoryName->setText(settings.value("DirectoryName", QDir::currentPath()).toString());
    ui->lineEditSignatures->setText(settings.value("Signatures", "$app/db").toString());
    ui->lineEditSignaturesExtra->setText(settings.value("SignaturesExtra", "$app/db_extra").toString());
    ui->lineEditSignaturesCustom->setText(settings.value("SignaturesCustom", "$app/db_custom").toString());
    ui->groupBoxSignaturesExtra->setChecked(settings.value("SignaturesExtraEnable", true).toBool());
    ui->groupBoxSignaturesCustom->setChecked(settings.value("SignaturesCustomEnable", true).toBool());
    ui->lineEditOut->setText(settings.value("ResultName", QDir::currentPath() + QDir::separator() + "out").toString());
    ui->lineEditLog->setText(settings.value("LogName", QDir::currentPath() + QDir::separator() + "log").toString());

    ui->spinBoxCopyCount->setValue(settings.value("CopyCount", 0).toInt());

    ui->checkBoxRemoveCopied->setChecked(settings.value("RemoveCopied", false).toBool());
    ui->checkBoxCopyTheFirstOnly->setChecked(settings.value("CopyTheFirstOnly", false).toBool());

    ui->checkBoxRecursive->setChecked(settings.value("Recursive", true).toBool());
    ui->checkBoxDeepScan->setChecked(settings.value("DeepScan", true).toBool());
    ui->checkBoxVerbose->setChecked(settings.value("Verbose", true).toBool());
    ui->checkBoxHeuristicScan->setChecked(settings.value("HeuristicScan", true).toBool());
    ui->checkBoxAggressive->setChecked(settings.value("Aggressive", true).toBool());
    ui->checkBoxShowVersion->setChecked(settings.value("ShowVersion", true).toBool());
    ui->checkBoxShowInfo->setChecked(settings.value("ShowInfo", true).toBool());
    ui->checkBoxSort->setChecked(settings.value("Sort", true).toBool());
    ui->checkBoxScanSubdirectories->setChecked(settings.value("ScanSubdirectories", true).toBool());
    ui->checkBoxValidOnly->setChecked(settings.value("ValidOnly", true).toBool());

    // ui->spinBoxThreads->setValue(settings.value("Threads", 1).toInt());

    options.bDebug = settings.value("Debug", false).toBool();

#ifdef QT_DEBUG
    options.bDebug = true;
#endif
}

void GuiMainWindow::saveSettings()
{
    QString sSettingsFile = QApplication::applicationDirPath() + QDir::separator();
#ifdef Q_OS_LINUX
    sSettingsFile += QString("Linux.");
#endif
    sSettingsFile += QString("%1.ini").arg(X_APPLICATIONNAME);
    QSettings settings(sSettingsFile, QSettings::IniFormat);

    settings.setValue("DirectoryName", ui->lineEditDirectoryName->text());
    settings.setValue("ResultName", ui->lineEditOut->text());
    settings.setValue("LogName", ui->lineEditLog->text());
    settings.setValue("Signatures", ui->lineEditSignatures->text());
    settings.setValue("SignaturesExtra", ui->lineEditSignaturesExtra->text());
    settings.setValue("SignaturesCustom", ui->lineEditSignaturesCustom->text());
    settings.setValue("SignaturesExtraEnable", ui->groupBoxSignaturesExtra->isChecked());
    settings.setValue("SignaturesCustomEnable", ui->groupBoxSignaturesCustom->isChecked());
    settings.setValue("CopyCount", ui->spinBoxCopyCount->value());
    settings.setValue("CopyFormat", ui->comboBoxCopyFormat->currentIndex());
    settings.setValue("CopyType", ui->comboBoxCopyType->currentIndex());
    settings.setValue("FileFormat", ui->comboBoxFileNameFormat->currentIndex());
    settings.setValue("RemoveCopied", ui->checkBoxRemoveCopied->isChecked());
    settings.setValue("CopyTheFirstOnly", ui->checkBoxCopyTheFirstOnly->isChecked());
    settings.setValue("Copy", ui->groupBoxCopy->isChecked());
    settings.setValue("Log", ui->groupBoxLog->isChecked());

    settings.setValue("Recursive", ui->checkBoxRecursive->isChecked());
    settings.setValue("DeepScan", ui->checkBoxDeepScan->isChecked());
    settings.setValue("Verbose", ui->checkBoxVerbose->isChecked());
    settings.setValue("HeuristicScan", ui->checkBoxHeuristicScan->isChecked());
    settings.setValue("Aggressive", ui->checkBoxAggressive->isChecked());
    settings.setValue("ShowVersion", ui->checkBoxShowVersion->isChecked());
    settings.setValue("ShowInfo", ui->checkBoxShowInfo->isChecked());
    settings.setValue("Sort", ui->checkBoxSort->isChecked());
    settings.setValue("ScanSubdirectories", ui->checkBoxScanSubdirectories->isChecked());
    settings.setValue("ValidOnly", ui->checkBoxValidOnly->isChecked());

    settings.setValue("UnknownPrefix", ui->comboBoxUnknownPrefix->currentIndex());
    settings.setValue("UnknownCount", ui->spinBoxUnknownCount->value());

    settings.setValue("Overlay", ui->comboBoxOverlay->currentIndex());
    settings.setValue("Entropy", ui->comboBoxEntropy->currentIndex());
    settings.setValue("EntropyValue", ui->doubleSpinBoxEntropy->value());
    // settings.setValue("Threads", ui->spinBoxThreads->value());
}

void GuiMainWindow::on_pushButtonLog_clicked()
{
    QString sInitDirectory = ui->lineEditLog->text();

    QString sDirectoryName = QFileDialog::getExistingDirectory(this, tr("Open directory") + QString("..."), sInitDirectory, QFileDialog::ShowDirsOnly);

    if (!sDirectoryName.isEmpty()) {
        ui->lineEditLog->setText(sDirectoryName);
    }
}
