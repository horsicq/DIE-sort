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

#include "guimainwindow.h"

#include "ui_guimainwindow.h"

GuiMainWindow::GuiMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::GuiMainWindow)
{
    ui->setupUi(this);

    setWindowTitle(QString("%1 v%2").arg(X_APPLICATIONNAME, X_APPLICATIONVERSION));

    options = {0};

    QString sSettingsFile = QApplication::applicationDirPath() + QDir::separator();
#ifdef Q_OS_LINUX
    sSettingsFile += QString("Linux.");
#endif
    sSettingsFile += QString("%1.ini").arg(X_APPLICATIONNAME);

    QSettings settings(sSettingsFile, QSettings::IniFormat);

    connect(ui->checkBoxBinary, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxCOM, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxMSDOS, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxNE, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxLE, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxLX, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxPE32, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxPE64, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxELF32, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxELF64, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxMACHO32, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxMACHO64, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));

    connect(ui->checkBox_archive, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_audio, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_boot, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_compiler, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_converter, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_driver, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_emulator, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_extender, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_format, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_framework, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_image, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_immunizer, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_installer, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_joiner, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_keygen, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_library, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_linker, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_loader, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_other, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_overlay, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_packer, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_patcher, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_player, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_protection, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_protector, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_script, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_self_displayer, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_sfx, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_source, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_system, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_type, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_operation_system, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));

    ui->checkBoxAllFileTypes->setChecked(true);
    ui->checkBoxAllTypes->setChecked(true);

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

    ui->comboBoxFileFormat->addItem("Original");
    ui->comboBoxFileFormat->addItem("MD5");
    ui->comboBoxFileFormat->addItem("MD5+Original");

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

    ui->comboBoxCopyFormat->setCurrentIndex(settings.value("CopyFormat", 0).toInt());
    ui->comboBoxCopyType->setCurrentIndex(settings.value("CopyType", 0).toInt());
    ui->comboBoxFileFormat->setCurrentIndex(settings.value("FileFormat", 0).toInt());
    ui->comboBoxUnknownPrefix->setCurrentIndex(settings.value("UnknownPrefix", 0).toInt());
    ui->comboBoxOverlay->setCurrentIndex(settings.value("Overlay", 0).toInt());
    ui->comboBoxEntropy->setCurrentIndex(settings.value("Entropy", 0).toInt());

    ui->spinBoxUnknownCount->setValue(settings.value("UnknownCount", 10).toInt());
    ui->doubleSpinBoxEntropy->setValue(settings.value("EntropyValue", 6.5).toDouble());

    ui->lineEditDirectoryName->setText(settings.value("DirectoryName", QDir::currentPath()).toString());
    ui->lineEditSignatures->setText(settings.value("Signatures", "$app/db").toString());
    ui->lineEditSignaturesCustom->setText(settings.value("SignaturesCustom", "$app/db_custom").toString());
    ui->lineEditOut->setText(settings.value("ResultName", QDir::currentPath()).toString());

    ui->spinBoxCopyCount->setValue(settings.value("CopyCount", 0).toInt());

    ui->checkBoxRemoveCopied->setChecked(settings.value("RemoveCopied", false).toBool());
    ui->checkBoxCopyTheFirstOnly->setChecked(settings.value("CopyTheFirstOnly", false).toBool());

    ui->spinBoxThreads->setValue(settings.value("Threads", 1).toInt());

    options.bContinue = settings.value("Continue", false).toBool();
    options.bDebug = settings.value("Debug", false).toBool();

#ifdef QT_DEBUG
    options.bDebug = true;
#endif

    QString sDatabaseName = settings.value("DatabaseName", ":memory:").toString();

    if (!ScanProgress::createDatabase(&options.dbSQLLite, sDatabaseName)) {
        QMessageBox::critical(this, tr("Error"), tr("Cannot open SQLITE database"));
        exit(1);
    }
}

GuiMainWindow::~GuiMainWindow()
{
    QString sSettingsFile = QApplication::applicationDirPath() + QDir::separator();
#ifdef Q_OS_LINUX
    sSettingsFile += QString("Linux.");
#endif
    sSettingsFile += QString("%1.ini").arg(X_APPLICATIONNAME);
    QSettings settings(sSettingsFile, QSettings::IniFormat);

    settings.setValue("DirectoryName", ui->lineEditDirectoryName->text());
    settings.setValue("ResultName", ui->lineEditOut->text());
    settings.setValue("Signatures", ui->lineEditSignatures->text());
    settings.setValue("SignaturesCustom", ui->lineEditSignaturesCustom->text());
    settings.setValue("CopyCount", ui->spinBoxCopyCount->value());
    settings.setValue("CopyFormat", ui->comboBoxCopyFormat->currentIndex());
    settings.setValue("CopyType", ui->comboBoxCopyType->currentIndex());
    settings.setValue("FileFormat", ui->comboBoxFileFormat->currentIndex());
    settings.setValue("RemoveCopied", ui->checkBoxRemoveCopied->isChecked());
    settings.setValue("CopyTheFirstOnly", ui->checkBoxCopyTheFirstOnly->isChecked());

    settings.setValue("UnknownPrefix", ui->comboBoxUnknownPrefix->currentIndex());
    settings.setValue("UnknownCount", ui->spinBoxUnknownCount->value());

    settings.setValue("Overlay", ui->comboBoxOverlay->currentIndex());
    settings.setValue("Entropy", ui->comboBoxEntropy->currentIndex());
    settings.setValue("EntropyValue", ui->doubleSpinBoxEntropy->value());
    settings.setValue("Threads", ui->spinBoxThreads->value());

    delete ui;
}

void GuiMainWindow::on_pushButtonExit_clicked()
{
    this->close();
}

void GuiMainWindow::on_pushButtonOpenDirectory_clicked()
{
    QString sInitDirectory = ui->lineEditDirectoryName->text();

    QString sDirectoryName = QFileDialog::getExistingDirectory(this, tr("Open directory..."), sInitDirectory, QFileDialog::ShowDirsOnly);

    if (!sDirectoryName.isEmpty()) {
        ui->lineEditDirectoryName->setText(sDirectoryName);
    }
}

void GuiMainWindow::on_pushButtonOut_clicked()
{
    QString sInitDirectory = ui->lineEditOut->text();

    QString sDirectoryName = QFileDialog::getExistingDirectory(this, tr("Open directory..."), sInitDirectory, QFileDialog::ShowDirsOnly);

    if (!sDirectoryName.isEmpty()) {
        ui->lineEditOut->setText(sDirectoryName);
    }
}

void GuiMainWindow::on_pushButtonScan_clicked()
{
    _scan();
}

void GuiMainWindow::_scan()
{
    options.nCopyCount = ui->spinBoxCopyCount->value();
    options.sResultDirectory = ui->lineEditOut->text();

    options.stFileTypes.clear();

    if (ui->checkBoxBinary->isChecked()) options.stFileTypes.insert(XBinary::FT_BINARY);
    if (ui->checkBoxCOM->isChecked()) options.stFileTypes.insert(XBinary::FT_COM);
    if (ui->checkBoxMSDOS->isChecked()) options.stFileTypes.insert(XBinary::FT_MSDOS);
    if (ui->checkBoxNE->isChecked()) options.stFileTypes.insert(XBinary::FT_NE);
    if (ui->checkBoxLE->isChecked()) options.stFileTypes.insert(XBinary::FT_LE);
    if (ui->checkBoxLX->isChecked()) options.stFileTypes.insert(XBinary::FT_LX);
    if (ui->checkBoxPE32->isChecked()) options.stFileTypes.insert(XBinary::FT_PE32);
    if (ui->checkBoxPE64->isChecked()) options.stFileTypes.insert(XBinary::FT_PE64);
    if (ui->checkBoxELF32->isChecked()) options.stFileTypes.insert(XBinary::FT_ELF32);
    if (ui->checkBoxELF64->isChecked()) options.stFileTypes.insert(XBinary::FT_ELF64);
    if (ui->checkBoxMACHO32->isChecked()) options.stFileTypes.insert(XBinary::FT_MACHO32);
    if (ui->checkBoxMACHO64->isChecked()) options.stFileTypes.insert(XBinary::FT_MACHO64);

    options.stTypes.clear();

    options.bAllTypes = ui->checkBoxAllTypes->isChecked();

    if (ui->checkBox_archive->isChecked()) options.stTypes.insert("archive");
    if (ui->checkBox_audio->isChecked()) options.stTypes.insert("audio");
    if (ui->checkBox_boot->isChecked()) options.stTypes.insert("boot");
    if (ui->checkBox_compiler->isChecked()) options.stTypes.insert("compiler");
    if (ui->checkBox_converter->isChecked()) options.stTypes.insert("converter");
    if (ui->checkBox_driver->isChecked()) options.stTypes.insert("driver");
    if (ui->checkBox_emulator->isChecked()) options.stTypes.insert("emulator");
    if (ui->checkBox_extender->isChecked()) options.stTypes.insert("extender");
    if (ui->checkBox_format->isChecked()) options.stTypes.insert("format");
    if (ui->checkBox_framework->isChecked()) options.stTypes.insert("framework");
    if (ui->checkBox_image->isChecked()) options.stTypes.insert("image");
    if (ui->checkBox_immunizer->isChecked()) options.stTypes.insert("immunizer");
    if (ui->checkBox_installer->isChecked()) options.stTypes.insert("installer");
    if (ui->checkBox_joiner->isChecked()) options.stTypes.insert("joiner");
    if (ui->checkBox_keygen->isChecked()) options.stTypes.insert("keygen");
    if (ui->checkBox_library->isChecked()) options.stTypes.insert("library");
    if (ui->checkBox_linker->isChecked()) options.stTypes.insert("linker");
    if (ui->checkBox_loader->isChecked()) options.stTypes.insert("loader");
    if (ui->checkBox_other->isChecked()) options.stTypes.insert("other");
    if (ui->checkBox_overlay->isChecked()) options.stTypes.insert("overlay");
    if (ui->checkBox_packer->isChecked()) options.stTypes.insert("packer");
    if (ui->checkBox_patcher->isChecked()) options.stTypes.insert("patcher");
    if (ui->checkBox_player->isChecked()) options.stTypes.insert("player");
    if (ui->checkBox_protection->isChecked()) options.stTypes.insert("protection");
    if (ui->checkBox_protector->isChecked()) options.stTypes.insert("protector");
    if (ui->checkBox_script->isChecked()) options.stTypes.insert("script");
    if (ui->checkBox_self_displayer->isChecked()) options.stTypes.insert("self-displayer");
    if (ui->checkBox_sfx->isChecked()) options.stTypes.insert("sfx");
    if (ui->checkBox_source->isChecked()) options.stTypes.insert("source");
    if (ui->checkBox_system->isChecked()) options.stTypes.insert("system");
    if (ui->checkBox_type->isChecked()) options.stTypes.insert("type");
    if (ui->checkBox_operation_system->isChecked()) options.stTypes.insert("operation system");

    options.bIsRecursive = ui->checkBoxRecursive->isChecked();
    options.bIsDeepScan = ui->checkBoxDeepScan->isChecked();
    options.bIsVerbose = ui->checkBoxVerbose->isChecked();
    options.bIsHeuristicScan = ui->checkBoxHeuristicScan->isChecked();
    options.bShowVersion = ui->checkBoxShowVersion->isChecked();
    options.bShowOptions = ui->checkBoxShowOptions->isChecked();
    options.bSubdirectories = ui->checkBoxScanSubdirectories->isChecked();
    options.sSignatures = ui->lineEditSignatures->text();
    options.sSignaturesCustom = ui->lineEditSignaturesCustom->text();

    options.copyFormat = (ScanProgress::CF)ui->comboBoxCopyFormat->currentIndex();
    options.copyType = (ScanProgress::CT)ui->comboBoxCopyType->currentIndex();
    options.bRemoveCopied = ui->checkBoxRemoveCopied->isChecked();
    options.bCopyTheFirstOnly = ui->checkBoxCopyTheFirstOnly->isChecked();

    options.unknownPrefix = (ScanProgress::UP)ui->comboBoxUnknownPrefix->currentIndex();
    options.nUnknownCount = ui->spinBoxUnknownCount->value();

    options.fileFormat = (ScanProgress::FF)ui->comboBoxFileFormat->currentIndex();
    options.overlay = (ScanProgress::OVERLAY)ui->comboBoxOverlay->currentIndex();
    options.entropy = (ScanProgress::ENTROPY)ui->comboBoxEntropy->currentIndex();
    options.dEntropyValue = ui->doubleSpinBoxEntropy->value();
    options.nThreads = ui->spinBoxThreads->value();

    DialogScanProgress ds(this);

    ds.setData(ui->lineEditDirectoryName->text(), &options);

    ds.exec();

    //    DialogStaticScan ds(this);
    //    connect(&ds, SIGNAL(scanFileStarted(QString)),this,SLOT(scanFileStarted(QString)),Qt::DirectConnection);
    //    connect(&ds, SIGNAL(scanResult(SpecAbstract::SCAN_RESULT)),this,SLOT(scanResult(SpecAbstract::SCAN_RESULT)),Qt::DirectConnection);
    //    ds.setData(ui->lineEditDirectoryName->text(),&options);
    //    ds.exec();
}

void GuiMainWindow::on_checkBoxAllFileTypes_toggled(bool checked)
{
    ui->checkBoxBinary->setChecked(checked);
    ui->checkBoxCOM->setChecked(checked);
    ui->checkBoxMSDOS->setChecked(checked);
    ui->checkBoxNE->setChecked(checked);
    ui->checkBoxLE->setChecked(checked);
    ui->checkBoxLX->setChecked(checked);
    ui->checkBoxPE32->setChecked(checked);
    ui->checkBoxPE64->setChecked(checked);
    ui->checkBoxELF32->setChecked(checked);
    ui->checkBoxELF64->setChecked(checked);
    ui->checkBoxMACHO32->setChecked(checked);
    ui->checkBoxMACHO64->setChecked(checked);
}

// void GuiMainWindow::on_checkBoxBinary_toggled(bool checked)
//{
//     QSignalBlocker blocker(ui->checkBoxAllFileTypes);

//    if(!checked)
//    {
//        ui->checkBoxAllFileTypes->setChecked(false);
//    }
//}

void GuiMainWindow::on_checkBoxAllTypes_toggled(bool checked)
{
    ui->checkBox_archive->setChecked(checked);
    ui->checkBox_audio->setChecked(checked);
    ui->checkBox_boot->setChecked(checked);
    ui->checkBox_compiler->setChecked(checked);
    ui->checkBox_converter->setChecked(checked);
    ui->checkBox_driver->setChecked(checked);
    ui->checkBox_emulator->setChecked(checked);
    ui->checkBox_extender->setChecked(checked);
    ui->checkBox_format->setChecked(checked);
    ui->checkBox_framework->setChecked(checked);
    ui->checkBox_image->setChecked(checked);
    ui->checkBox_immunizer->setChecked(checked);
    ui->checkBox_installer->setChecked(checked);
    ui->checkBox_joiner->setChecked(checked);
    ui->checkBox_keygen->setChecked(checked);
    ui->checkBox_library->setChecked(checked);
    ui->checkBox_linker->setChecked(checked);
    ui->checkBox_loader->setChecked(checked);
    ui->checkBox_other->setChecked(checked);
    ui->checkBox_overlay->setChecked(checked);
    ui->checkBox_packer->setChecked(checked);
    ui->checkBox_patcher->setChecked(checked);
    ui->checkBox_player->setChecked(checked);
    ui->checkBox_protection->setChecked(checked);
    ui->checkBox_protector->setChecked(checked);
    ui->checkBox_script->setChecked(checked);
    ui->checkBox_self_displayer->setChecked(checked);
    ui->checkBox_sfx->setChecked(checked);
    ui->checkBox_source->setChecked(checked);
    ui->checkBox_system->setChecked(checked);
    ui->checkBox_type->setChecked(checked);
    ui->checkBox_operation_system->setChecked(checked);
}

void GuiMainWindow::on_pushButtonInfo_clicked()
{
    QMessageBox::information(this, tr("Info"), tr("Bugreports: horsicq@gmail.com"));
}

void GuiMainWindow::onFileTypeToggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllFileTypes);

    if (!checked) {
        ui->checkBoxAllFileTypes->setChecked(false);
    }
}

void GuiMainWindow::onTypeToggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if (!checked) {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_pushButtonSignatures_clicked()
{
    QString sInitDirectory = XBinary::convertPathName(ui->lineEditSignatures->text());

    QString sDirectoryName = QFileDialog::getExistingDirectory(this, tr("Open directory..."), sInitDirectory, QFileDialog::ShowDirsOnly);

    if (!sDirectoryName.isEmpty()) {
        ui->lineEditSignatures->setText(sDirectoryName);
    }
}

void GuiMainWindow::on_doubleSpinBoxEntropy_valueChanged(double arg1)
{
    double dEntropy = ui->doubleSpinBoxEntropy->value();
    double dProcent = (dEntropy / 8) * 100;
    ui->lineEditEntropy->setText(QString::number(dProcent));
}

void GuiMainWindow::on_pushButtonSignaturesCustom_clicked()
{
    QString sInitDirectory = XBinary::convertPathName(ui->lineEditSignaturesCustom->text());

    QString sDirectoryName = QFileDialog::getExistingDirectory(this, tr("Open directory..."), sInitDirectory, QFileDialog::ShowDirsOnly);

    if (!sDirectoryName.isEmpty()) {
        ui->lineEditSignaturesCustom->setText(sDirectoryName);
    }
}
