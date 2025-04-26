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

    connect(ui->checkBoxBinary, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxCOM, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxMSDOS, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxDOS16M, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxDOS4G, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxNE, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxLE, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxLX, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxPE32, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxPE64, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxELF32, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxELF64, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxMACHO32, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxMACHO64, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxZIP, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxJAR, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxAPK, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxMACHOFAT, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxNPM, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxPDF, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxDEX, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxRAR, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxAmiga, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));
    connect(ui->checkBoxJavaClass, SIGNAL(toggled(bool)), this, SLOT(onFileTypeToggled(bool)));

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
    connect(ui->checkBox_cryptor, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_crypter, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_licensing, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_language, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_tool, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_dos_extender, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_sign_tool, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));
    connect(ui->checkBox_virtual_machine, SIGNAL(toggled(bool)), this, SLOT(onTypeToggled(bool)));

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
    saveSettings();
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
    if (ui->checkBoxDOS16M->isChecked()) options.stFileTypes.insert(XBinary::FT_DOS16M);
    if (ui->checkBoxDOS4G->isChecked()) options.stFileTypes.insert(XBinary::FT_DOS4G);
    if (ui->checkBoxNE->isChecked()) options.stFileTypes.insert(XBinary::FT_NE);
    if (ui->checkBoxLE->isChecked()) options.stFileTypes.insert(XBinary::FT_LE);
    if (ui->checkBoxLX->isChecked()) options.stFileTypes.insert(XBinary::FT_LX);
    if (ui->checkBoxPE32->isChecked()) options.stFileTypes.insert(XBinary::FT_PE32);
    if (ui->checkBoxPE64->isChecked()) options.stFileTypes.insert(XBinary::FT_PE64);
    if (ui->checkBoxELF32->isChecked()) options.stFileTypes.insert(XBinary::FT_ELF32);
    if (ui->checkBoxELF64->isChecked()) options.stFileTypes.insert(XBinary::FT_ELF64);
    if (ui->checkBoxMACHO32->isChecked()) options.stFileTypes.insert(XBinary::FT_MACHO32);
    if (ui->checkBoxMACHO64->isChecked()) options.stFileTypes.insert(XBinary::FT_MACHO64);
    if (ui->checkBoxZIP->isChecked()) options.stFileTypes.insert(XBinary::FT_ZIP);
    if (ui->checkBoxJAR->isChecked()) options.stFileTypes.insert(XBinary::FT_JAR);
    if (ui->checkBoxAPK->isChecked()) options.stFileTypes.insert(XBinary::FT_APK);
    if (ui->checkBoxMACHOFAT->isChecked()) options.stFileTypes.insert(XBinary::FT_MACHOFAT);
    if (ui->checkBoxNPM->isChecked()) options.stFileTypes.insert(XBinary::FT_NPM);
    if (ui->checkBoxPDF->isChecked()) options.stFileTypes.insert(XBinary::FT_PDF);
    if (ui->checkBoxDEX->isChecked()) options.stFileTypes.insert(XBinary::FT_DEX);
    if (ui->checkBoxRAR->isChecked()) options.stFileTypes.insert(XBinary::FT_RAR);
    if (ui->checkBoxAmiga->isChecked()) options.stFileTypes.insert(XBinary::FT_AMIGAHUNK);
    if (ui->checkBoxJavaClass->isChecked()) options.stFileTypes.insert(XBinary::FT_JAVACLASS);

    options.stTypes.clear();

    options.bAllTypes = ui->checkBoxAllTypes->isChecked();

    if (ui->checkBox_archive->isChecked()) _appendType("archive");
    if (ui->checkBox_audio->isChecked()) _appendType("audio");
    if (ui->checkBox_boot->isChecked()) _appendType("boot");
    if (ui->checkBox_compiler->isChecked()) _appendType("compiler");
    if (ui->checkBox_converter->isChecked()) _appendType("converter");
    if (ui->checkBox_driver->isChecked()) _appendType("driver");
    if (ui->checkBox_emulator->isChecked()) _appendType("emulator");
    if (ui->checkBox_extender->isChecked()) _appendType("extender");
    if (ui->checkBox_format->isChecked()) _appendType("format");
    if (ui->checkBox_framework->isChecked()) _appendType("framework");
    if (ui->checkBox_image->isChecked()) _appendType("image");
    if (ui->checkBox_immunizer->isChecked()) _appendType("immunizer");
    if (ui->checkBox_installer->isChecked()) _appendType("installer");
    if (ui->checkBox_joiner->isChecked()) _appendType("joiner");
    if (ui->checkBox_keygen->isChecked()) _appendType("keygen");
    if (ui->checkBox_library->isChecked()) _appendType("library");
    if (ui->checkBox_linker->isChecked()) _appendType("linker");
    if (ui->checkBox_loader->isChecked()) _appendType("loader");
    if (ui->checkBox_other->isChecked()) _appendType("other");
    if (ui->checkBox_overlay->isChecked()) _appendType("overlay");
    if (ui->checkBox_packer->isChecked()) _appendType("packer");
    if (ui->checkBox_patcher->isChecked()) _appendType("patcher");
    if (ui->checkBox_player->isChecked()) _appendType("player");
    if (ui->checkBox_protection->isChecked()) _appendType("protection");
    if (ui->checkBox_protector->isChecked()) _appendType("protector");
    if (ui->checkBox_script->isChecked()) _appendType("script");
    if (ui->checkBox_self_displayer->isChecked()) _appendType("self-displayer");
    if (ui->checkBox_sfx->isChecked()) _appendType("sfx");
    if (ui->checkBox_source->isChecked()) _appendType("source");
    if (ui->checkBox_system->isChecked()) _appendType("system");
    if (ui->checkBox_type->isChecked()) _appendType("type");
    if (ui->checkBox_operation_system->isChecked()) _appendType("operation system");
    if (ui->checkBox_cryptor->isChecked()) _appendType("cryptor");
    if (ui->checkBox_crypter->isChecked()) _appendType("crypter");
    if (ui->checkBox_licensing->isChecked()) _appendType("licensing");
    if (ui->checkBox_language->isChecked()) _appendType("language");
    if (ui->checkBox_tool->isChecked()) _appendType("tool");
    if (ui->checkBox_dos_extender->isChecked()) _appendType("dos extender");
    if (ui->checkBox_sign_tool->isChecked()) _appendType("sign tool");
    if (ui->checkBox_virtual_machine->isChecked()) _appendType("virtual machine");

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

void GuiMainWindow::on_checkBoxAllFileTypes_toggled(bool checked)
{
    ui->checkBoxBinary->setChecked(checked);
    ui->checkBoxCOM->setChecked(checked);
    ui->checkBoxMSDOS->setChecked(checked);
    ui->checkBoxDOS16M->setChecked(checked);
    ui->checkBoxDOS4G->setChecked(checked);
    ui->checkBoxNE->setChecked(checked);
    ui->checkBoxLE->setChecked(checked);
    ui->checkBoxLX->setChecked(checked);
    ui->checkBoxPE32->setChecked(checked);
    ui->checkBoxPE64->setChecked(checked);
    ui->checkBoxELF32->setChecked(checked);
    ui->checkBoxELF64->setChecked(checked);
    ui->checkBoxMACHO32->setChecked(checked);
    ui->checkBoxMACHO64->setChecked(checked);
    ui->checkBoxZIP->setChecked(checked);
    ui->checkBoxJAR->setChecked(checked);
    ui->checkBoxAPK->setChecked(checked);
    ui->checkBoxMACHOFAT->setChecked(checked);
    ui->checkBoxNPM->setChecked(checked);
    ui->checkBoxPDF->setChecked(checked);
    ui->checkBoxDEX->setChecked(checked);
    ui->checkBoxRAR->setChecked(checked);
    ui->checkBoxAmiga->setChecked(checked);
    ui->checkBoxJavaClass->setChecked(checked);
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
    ui->checkBox_crypter->setChecked(checked);
    ui->checkBox_cryptor->setChecked(checked);
    ui->checkBox_licensing->setChecked(checked);
    ui->checkBox_language->setChecked(checked);
    ui->checkBox_tool->setChecked(checked);
    ui->checkBox_dos_extender->setChecked(checked);
    ui->checkBox_sign_tool->setChecked(checked);
    ui->checkBox_virtual_machine->setChecked(checked);
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
    Q_UNUSED(arg1)

    double dEntropy = ui->doubleSpinBoxEntropy->value();
    double dPercentage = (dEntropy / 8) * 100;
    ui->lineEditEntropy->setText(QString::number(dPercentage, 'f', 2));
}

void GuiMainWindow::on_pushButtonSignaturesCustom_clicked()
{
    QString sInitDirectory = XBinary::convertPathName(ui->lineEditSignaturesCustom->text());

    QString sDirectoryName = QFileDialog::getExistingDirectory(this, tr("Open directory..."), sInitDirectory, QFileDialog::ShowDirsOnly);

    if (!sDirectoryName.isEmpty()) {
        ui->lineEditSignaturesCustom->setText(sDirectoryName);
    }
}

void GuiMainWindow::on_pushButtonSignaturesExtra_clicked()
{
    QString sInitDirectory = XBinary::convertPathName(ui->lineEditSignaturesExtra->text());

    QString sDirectoryName = QFileDialog::getExistingDirectory(this, tr("Open directory..."), sInitDirectory, QFileDialog::ShowDirsOnly);

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
    ui->lineEditOut->setText(settings.value("ResultName", QDir::currentPath()).toString());

    ui->spinBoxCopyCount->setValue(settings.value("CopyCount", 0).toInt());

    ui->checkBoxRemoveCopied->setChecked(settings.value("RemoveCopied", false).toBool());
    ui->checkBoxCopyTheFirstOnly->setChecked(settings.value("CopyTheFirstOnly", false).toBool());

    ui->checkBoxAllFileTypes->setChecked(settings.value("AllFileTypes", true).toBool());
    ui->checkBoxBinary->setChecked(settings.value("FT_Binary", true).toBool());
    ui->checkBoxCOM->setChecked(settings.value("FT_COM", true).toBool());
    ui->checkBoxMSDOS->setChecked(settings.value("FT_MSDOS", true).toBool());
    ui->checkBoxDOS16M->setChecked(settings.value("FT_DOS16M", true).toBool());
    ui->checkBoxDOS4G->setChecked(settings.value("FT_DOS4G", true).toBool());
    ui->checkBoxNE->setChecked(settings.value("FT_NE", true).toBool());
    ui->checkBoxLE->setChecked(settings.value("FT_LE", true).toBool());
    ui->checkBoxLX->setChecked(settings.value("FT_LX", true).toBool());
    ui->checkBoxPE32->setChecked(settings.value("FT_PE32", true).toBool());
    ui->checkBoxPE64->setChecked(settings.value("FT_PE64", true).toBool());
    ui->checkBoxELF32->setChecked(settings.value("FT_ELF32", true).toBool());
    ui->checkBoxELF64->setChecked(settings.value("FT_ELF64", true).toBool());
    ui->checkBoxMACHO32->setChecked(settings.value("FT_MACHO32", true).toBool());
    ui->checkBoxMACHO64->setChecked(settings.value("FT_MACHO64", true).toBool());
    ui->checkBoxZIP->setChecked(settings.value("FT_ZIP", true).toBool());
    ui->checkBoxJAR->setChecked(settings.value("FT_JAR", true).toBool());
    ui->checkBoxAPK->setChecked(settings.value("FT_APK", true).toBool());
    ui->checkBoxMACHOFAT->setChecked(settings.value("FT_MACHOFAT", true).toBool());
    ui->checkBoxNPM->setChecked(settings.value("FT_NPM", true).toBool());
    ui->checkBoxPDF->setChecked(settings.value("FT_PDF", true).toBool());
    ui->checkBoxDEX->setChecked(settings.value("FT_DEX", true).toBool());
    ui->checkBoxRAR->setChecked(settings.value("FT_RAR", true).toBool());
    ui->checkBoxAmiga->setChecked(settings.value("FT_AMIGAHUNK", true).toBool());
    ui->checkBoxJavaClass->setChecked(settings.value("FT_JAVACLASS", true).toBool());

    ui->checkBoxAllTypes->setChecked(settings.value("AllTypes", true).toBool());
    ui->checkBox_archive->setChecked(settings.value("type_archive", true).toBool());
    ui->checkBox_audio->setChecked(settings.value("type_audio", true).toBool());
    ui->checkBox_boot->setChecked(settings.value("type_boot", true).toBool());
    ui->checkBox_compiler->setChecked(settings.value("type_compiler", true).toBool());
    ui->checkBox_converter->setChecked(settings.value("type_converter", true).toBool());
    ui->checkBox_driver->setChecked(settings.value("type_driver", true).toBool());
    ui->checkBox_emulator->setChecked(settings.value("type_emulator", true).toBool());
    ui->checkBox_extender->setChecked(settings.value("type_extender", true).toBool());
    ui->checkBox_format->setChecked(settings.value("type_format", true).toBool());
    ui->checkBox_framework->setChecked(settings.value("type_framework", true).toBool());
    ui->checkBox_image->setChecked(settings.value("type_image", true).toBool());
    ui->checkBox_immunizer->setChecked(settings.value("type_immunizer", true).toBool());
    ui->checkBox_installer->setChecked(settings.value("type_installer", true).toBool());
    ui->checkBox_joiner->setChecked(settings.value("type_joiner", true).toBool());
    ui->checkBox_keygen->setChecked(settings.value("type_keygen", true).toBool());
    ui->checkBox_library->setChecked(settings.value("type_library", true).toBool());
    ui->checkBox_linker->setChecked(settings.value("type_linker", true).toBool());
    ui->checkBox_loader->setChecked(settings.value("type_loader", true).toBool());
    ui->checkBox_other->setChecked(settings.value("type_other", true).toBool());
    ui->checkBox_overlay->setChecked(settings.value("type_overlay", true).toBool());
    ui->checkBox_packer->setChecked(settings.value("type_packer", true).toBool());
    ui->checkBox_patcher->setChecked(settings.value("type_patcher", true).toBool());
    ui->checkBox_player->setChecked(settings.value("type_player", true).toBool());
    ui->checkBox_protection->setChecked(settings.value("type_protection", true).toBool());
    ui->checkBox_protector->setChecked(settings.value("type_protector", true).toBool());
    ui->checkBox_script->setChecked(settings.value("type_script", true).toBool());
    ui->checkBox_self_displayer->setChecked(settings.value("type_self_displayer", true).toBool());
    ui->checkBox_sfx->setChecked(settings.value("type_sfx", true).toBool());
    ui->checkBox_source->setChecked(settings.value("type_source", true).toBool());
    ui->checkBox_system->setChecked(settings.value("type_system", true).toBool());
    ui->checkBox_type->setChecked(settings.value("type_type", true).toBool());
    ui->checkBox_operation_system->setChecked(settings.value("type_operation_system", true).toBool());
    ui->checkBox_cryptor->setChecked(settings.value("type_cryptor", true).toBool());
    ui->checkBox_crypter->setChecked(settings.value("type_crypter", true).toBool());
    ui->checkBox_licensing->setChecked(settings.value("type_licensing", true).toBool());
    ui->checkBox_language->setChecked(settings.value("type_language", true).toBool());
    ui->checkBox_tool->setChecked(settings.value("type_tool", true).toBool());
    ui->checkBox_dos_extender->setChecked(settings.value("type_dos_extender", true).toBool());
    ui->checkBox_sign_tool->setChecked(settings.value("type_sign_tool", true).toBool());
    ui->checkBox_virtual_machine->setChecked(settings.value("type_virtual_machine", true).toBool());

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

    settings.setValue("AllFileTypes", ui->checkBoxAllFileTypes->isChecked());
    settings.setValue("FT_Binary", ui->checkBoxBinary->isChecked());
    settings.setValue("FT_COM", ui->checkBoxCOM->isChecked());
    settings.setValue("FT_MSDOS", ui->checkBoxMSDOS->isChecked());
    settings.setValue("FT_DOS16M", ui->checkBoxDOS16M->isChecked());
    settings.setValue("FT_DOS4G", ui->checkBoxDOS4G->isChecked());
    settings.setValue("FT_NE", ui->checkBoxNE->isChecked());
    settings.setValue("FT_LE", ui->checkBoxLE->isChecked());
    settings.setValue("FT_LX", ui->checkBoxLX->isChecked());
    settings.setValue("FT_PE32", ui->checkBoxPE32->isChecked());
    settings.setValue("FT_PE64", ui->checkBoxPE64->isChecked());
    settings.setValue("FT_ELF32", ui->checkBoxELF32->isChecked());
    settings.setValue("FT_ELF64", ui->checkBoxELF64->isChecked());
    settings.setValue("FT_MACHO32", ui->checkBoxMACHO32->isChecked());
    settings.setValue("FT_MACHO64", ui->checkBoxMACHO64->isChecked());
    settings.setValue("FT_ZIP", ui->checkBoxZIP->isChecked());
    settings.setValue("FT_JAR", ui->checkBoxJAR->isChecked());
    settings.setValue("FT_APK", ui->checkBoxAPK->isChecked());
    settings.setValue("FT_MACHOFAT", ui->checkBoxMACHOFAT->isChecked());
    settings.setValue("FT_NPM", ui->checkBoxNPM->isChecked());
    settings.setValue("FT_PDF", ui->checkBoxPDF->isChecked());
    settings.setValue("FT_DEX", ui->checkBoxDEX->isChecked());
    settings.setValue("FT_RAR", ui->checkBoxRAR->isChecked());
    settings.setValue("FT_AMIGAHUNK", ui->checkBoxAmiga->isChecked());
    settings.setValue("FT_JAVACLASS", ui->checkBoxJavaClass->isChecked());

    settings.setValue("AllTypes", ui->checkBoxAllTypes->isChecked());
    settings.setValue("type_archive", ui->checkBox_archive->isChecked());
    settings.setValue("type_audio", ui->checkBox_audio->isChecked());
    settings.setValue("type_boot", ui->checkBox_boot->isChecked());
    settings.setValue("type_compiler", ui->checkBox_compiler->isChecked());
    settings.setValue("type_converter", ui->checkBox_converter->isChecked());
    settings.setValue("type_driver", ui->checkBox_driver->isChecked());
    settings.setValue("type_emulator", ui->checkBox_emulator->isChecked());
    settings.setValue("type_extender", ui->checkBox_extender->isChecked());
    settings.setValue("type_format", ui->checkBox_format->isChecked());
    settings.setValue("type_framework", ui->checkBox_framework->isChecked());
    settings.setValue("type_image", ui->checkBox_image->isChecked());
    settings.setValue("type_immunizer", ui->checkBox_immunizer->isChecked());
    settings.setValue("type_installer", ui->checkBox_installer->isChecked());
    settings.setValue("type_joiner", ui->checkBox_joiner->isChecked());
    settings.setValue("type_keygen", ui->checkBox_keygen->isChecked());
    settings.setValue("type_library", ui->checkBox_library->isChecked());
    settings.setValue("type_linker", ui->checkBox_linker->isChecked());
    settings.setValue("type_loader", ui->checkBox_loader->isChecked());
    settings.setValue("type_other", ui->checkBox_other->isChecked());
    settings.setValue("type_overlay", ui->checkBox_overlay->isChecked());
    settings.setValue("type_packer", ui->checkBox_packer->isChecked());
    settings.setValue("type_patcher", ui->checkBox_patcher->isChecked());
    settings.setValue("type_player", ui->checkBox_player->isChecked());
    settings.setValue("type_protection", ui->checkBox_protection->isChecked());
    settings.setValue("type_protector", ui->checkBox_protector->isChecked());
    settings.setValue("type_script", ui->checkBox_script->isChecked());
    settings.setValue("type_self_displayer", ui->checkBox_self_displayer->isChecked());
    settings.setValue("type_sfx", ui->checkBox_sfx->isChecked());
    settings.setValue("type_source", ui->checkBox_source->isChecked());
    settings.setValue("type_system", ui->checkBox_system->isChecked());
    settings.setValue("type_type", ui->checkBox_type->isChecked());
    settings.setValue("type_operation_system", ui->checkBox_operation_system->isChecked());
    settings.setValue("type_cryptor", ui->checkBox_cryptor->isChecked());
    settings.setValue("type_crypter", ui->checkBox_crypter->isChecked());
    settings.setValue("type_licensing", ui->checkBox_licensing->isChecked());
    settings.setValue("type_language", ui->checkBox_language->isChecked());
    settings.setValue("type_tool", ui->checkBox_tool->isChecked());
    settings.setValue("type_dos_extender", ui->checkBox_dos_extender->isChecked());
    settings.setValue("type_sign_tool", ui->checkBox_sign_tool->isChecked());
    settings.setValue("type_virtual_machine", ui->checkBox_virtual_machine->isChecked());

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
