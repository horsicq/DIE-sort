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

#include "guimainwindow.h"
#include "ui_guimainwindow.h"

GuiMainWindow::GuiMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GuiMainWindow)
{
    ui->setupUi(this);

    setWindowTitle(QString("%1 v%2").arg(X_APPLICATIONNAME).arg(X_APPLICATIONVERSION));

    options={0};

    QString sSettingsFile=QApplication::applicationDirPath()+QDir::separator()+QString("%1.ini").arg(X_APPLICATIONNAME);
    QSettings settings(sSettingsFile,QSettings::IniFormat);

    ui->checkBoxAllFileTypes->setChecked(true);
    ui->checkBoxAllTypes->setChecked(true);

    ui->lineEditDirectoryName->setText(settings.value("DirectoryName",QDir::currentPath()).toString());
    ui->lineEditSignatures->setText(settings.value("Signatures","$app/db").toString());
    ui->lineEditOut->setText(settings.value("ResultName",QDir::currentPath()).toString());

    options.bContinue=settings.value("Continue",false).toBool();
    QString sDatabaseName=settings.value("DatabaseName",":memory:").toString();

    if(!ScanProgress::createDatabase(&options.dbSQLLite,sDatabaseName))
    {
        QMessageBox::critical(this,tr("Error"),tr("Cannot open SQLITE database"));
        exit(1);
    }

    options.bDebug=true;
}

GuiMainWindow::~GuiMainWindow()
{
    QString sSettingsFile=QApplication::applicationDirPath()+QDir::separator()+QString("%1.ini").arg(X_APPLICATIONNAME);
    QSettings settings(sSettingsFile,QSettings::IniFormat);

    settings.setValue("DirectoryName",ui->lineEditDirectoryName->text());
    settings.setValue("ResultName",ui->lineEditOut->text());
    settings.setValue("Signatures",ui->lineEditSignatures->text());

    delete ui;
}

void GuiMainWindow::on_pushButtonExit_clicked()
{
    this->close();
}

void GuiMainWindow::on_pushButtonOpenDirectory_clicked()
{
    QString sInitDirectory=ui->lineEditDirectoryName->text();

    QString sDirectoryName=QFileDialog::getExistingDirectory(this,tr("Open directory..."),sInitDirectory,QFileDialog::ShowDirsOnly);

    if(!sDirectoryName.isEmpty())
    {
        ui->lineEditDirectoryName->setText(sDirectoryName);
    }
}

void GuiMainWindow::on_pushButtonOut_clicked()
{
    QString sInitDirectory=ui->lineEditOut->text();

    QString sDirectoryName=QFileDialog::getExistingDirectory(this,tr("Open directory..."),sInitDirectory,QFileDialog::ShowDirsOnly);

    if(!sDirectoryName.isEmpty())
    {
        ui->lineEditOut->setText(sDirectoryName);
    }
}

void GuiMainWindow::on_pushButtonScan_clicked()
{
    _scan();
}

void GuiMainWindow::_scan()
{
    options.nCopyCount=ui->spinBoxCopyCount->value();
    options.sResultDirectory=ui->lineEditOut->text();

    options.stFileTypes.clear();

    if(ui->checkBoxBinary->isChecked())             options.stFileTypes.insert(XBinary::FT_BINARY);
    if(ui->checkBoxText->isChecked())               options.stFileTypes.insert(XBinary::FT_TEXT);
    if(ui->checkBoxMSDOS->isChecked())              options.stFileTypes.insert(XBinary::FT_MSDOS);
    if(ui->checkBoxPE32->isChecked())               options.stFileTypes.insert(XBinary::FT_PE32);
    if(ui->checkBoxPE64->isChecked())               options.stFileTypes.insert(XBinary::FT_PE64);
    if(ui->checkBoxELF32->isChecked())              options.stFileTypes.insert(XBinary::FT_ELF32);
    if(ui->checkBoxELF64->isChecked())              options.stFileTypes.insert(XBinary::FT_ELF64);
    if(ui->checkBoxMACHO32->isChecked())            options.stFileTypes.insert(XBinary::FT_MACH32);
    if(ui->checkBoxMACHO64->isChecked())            options.stFileTypes.insert(XBinary::FT_MACH64);

    options.stTypes.clear();

    options.bAllTypes=ui->checkBoxAllTypes->isChecked();

//    if(ui->checkBoxArchive->isChecked())            options.stTypes.insert(SpecAbstract::RECORD_TYPE_ARCHIVE);
//    if(ui->checkBoxCertificate->isChecked())        options.stTypes.insert(SpecAbstract::RECORD_TYPE_CERTIFICATE);
//    if(ui->checkBoxCompiler->isChecked())           options.stTypes.insert(SpecAbstract::RECORD_TYPE_COMPILER);
//    if(ui->checkBoxConverter->isChecked())          options.stTypes.insert(SpecAbstract::RECORD_TYPE_CONVERTER);
//    if(ui->checkBoxDatabase->isChecked())           options.stTypes.insert(SpecAbstract::RECORD_TYPE_DATABASE);
//    if(ui->checkBoxDebugData->isChecked())          options.stTypes.insert(SpecAbstract::RECORD_TYPE_DEBUGDATA);
//    if(ui->checkBoxDongleProtection->isChecked())   options.stTypes.insert(SpecAbstract::RECORD_TYPE_DONGLEPROTECTION);
//    if(ui->checkBoxDOSExtender->isChecked())        options.stTypes.insert(SpecAbstract::RECORD_TYPE_DOSEXTENDER);
//    if(ui->checkBoxFormat->isChecked())             options.stTypes.insert(SpecAbstract::RECORD_TYPE_FORMAT);
//    if(ui->checkBoxGeneric->isChecked())            options.stTypes.insert(SpecAbstract::RECORD_TYPE_GENERIC);
//    if(ui->checkBoxImage->isChecked())              options.stTypes.insert(SpecAbstract::RECORD_TYPE_IMAGE);
//    if(ui->checkBoxInstaller->isChecked())          options.stTypes.insert(SpecAbstract::RECORD_TYPE_INSTALLER);
//    if(ui->checkBoxInstallerData->isChecked())      options.stTypes.insert(SpecAbstract::RECORD_TYPE_INSTALLERDATA);
//    if(ui->checkBoxLibrary->isChecked())            options.stTypes.insert(SpecAbstract::RECORD_TYPE_LIBRARY);
//    if(ui->checkBoxLinker->isChecked())             options.stTypes.insert(SpecAbstract::RECORD_TYPE_LINKER);
//    if(ui->checkBoxNETObfuscator->isChecked())      options.stTypes.insert(SpecAbstract::RECORD_TYPE_NETOBFUSCATOR);
//    if(ui->checkBoxPacker->isChecked())             options.stTypes.insert(SpecAbstract::RECORD_TYPE_PACKER);
//    if(ui->checkBoxPETool->isChecked())             options.stTypes.insert(SpecAbstract::RECORD_TYPE_PETOOL);
//    if(ui->checkBoxProtector->isChecked())          options.stTypes.insert(SpecAbstract::RECORD_TYPE_PROTECTOR);
//    if(ui->checkBoxProtectorData->isChecked())      options.stTypes.insert(SpecAbstract::RECORD_TYPE_PROTECTORDATA);
//    if(ui->checkBoxSFX->isChecked())                options.stTypes.insert(SpecAbstract::RECORD_TYPE_SFX);
//    if(ui->checkBoxSFXData->isChecked())            options.stTypes.insert(SpecAbstract::RECORD_TYPE_SFXDATA);
//    if(ui->checkBoxSignTool->isChecked())           options.stTypes.insert(SpecAbstract::RECORD_TYPE_SIGNTOOL);
//    if(ui->checkBoxSourceCode->isChecked())         options.stTypes.insert(SpecAbstract::RECORD_TYPE_SOURCECODE);
//    if(ui->checkBoxStub->isChecked())               options.stTypes.insert(SpecAbstract::RECORD_TYPE_STUB);
//    if(ui->checkBoxTool->isChecked())               options.stTypes.insert(SpecAbstract::RECORD_TYPE_TOOL);

    options.bDeepScan=ui->checkBoxDeepScan->isChecked();
    options.bShowVersion=ui->checkBoxShowVersion->isChecked();
    options.bShowOptions=ui->checkBoxShowOptions->isChecked();
    options.bSubdirectories=ui->checkBoxScanSubdirectories->isChecked();
    options.sSignatures=ui->lineEditSignatures->text();

    DialogScanProgress ds(this);

    ds.setData(ui->lineEditDirectoryName->text(),&options);

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
    ui->checkBoxText->setChecked(checked);
    ui->checkBoxMSDOS->setChecked(checked);
    ui->checkBoxPE32->setChecked(checked);
    ui->checkBoxPE64->setChecked(checked);
    ui->checkBoxELF32->setChecked(checked);
    ui->checkBoxELF64->setChecked(checked);
    ui->checkBoxMACHO32->setChecked(checked);
    ui->checkBoxMACHO64->setChecked(checked);
}

void GuiMainWindow::on_checkBoxBinary_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllFileTypes);

    if(!checked)
    {
        ui->checkBoxAllFileTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxText_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllFileTypes);

    if(!checked)
    {
        ui->checkBoxAllFileTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxMSDOS_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllFileTypes);

    if(!checked)
    {
        ui->checkBoxAllFileTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxPE32_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllFileTypes);

    if(!checked)
    {
        ui->checkBoxAllFileTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxPE64_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllFileTypes);

    if(!checked)
    {
        ui->checkBoxAllFileTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxELF32_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllFileTypes);

    if(!checked)
    {
        ui->checkBoxAllFileTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxELF64_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllFileTypes);

    if(!checked)
    {
        ui->checkBoxAllFileTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxMACHO32_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllFileTypes);

    if(!checked)
    {
        ui->checkBoxAllFileTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxMACHO64_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllFileTypes);

    if(!checked)
    {
        ui->checkBoxAllFileTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxAllTypes_toggled(bool checked)
{
    ui->checkBoxArchive->setChecked(checked);
    ui->checkBoxCertificate->setChecked(checked);
    ui->checkBoxCompiler->setChecked(checked);
    ui->checkBoxConverter->setChecked(checked);
    ui->checkBoxDatabase->setChecked(checked);
    ui->checkBoxDebugData->setChecked(checked);
    ui->checkBoxDongleProtection->setChecked(checked);
    ui->checkBoxDOSExtender->setChecked(checked);
    ui->checkBoxFormat->setChecked(checked);
    ui->checkBoxGeneric->setChecked(checked);
    ui->checkBoxImage->setChecked(checked);
    ui->checkBoxInstaller->setChecked(checked);
    ui->checkBoxInstallerData->setChecked(checked);
    ui->checkBoxLibrary->setChecked(checked);
    ui->checkBoxLinker->setChecked(checked);
    ui->checkBoxNETObfuscator->setChecked(checked);
    ui->checkBoxPacker->setChecked(checked);
    ui->checkBoxPETool->setChecked(checked);
    ui->checkBoxProtector->setChecked(checked);
    ui->checkBoxProtectorData->setChecked(checked);
    ui->checkBoxSFX->setChecked(checked);
    ui->checkBoxSFXData->setChecked(checked);
    ui->checkBoxSignTool->setChecked(checked);
    ui->checkBoxSourceCode->setChecked(checked);
    ui->checkBoxStub->setChecked(checked);
    ui->checkBoxTool->setChecked(checked);
}

void GuiMainWindow::on_checkBoxArchive_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxCertificate_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxCompiler_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxConverter_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxDatabase_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxDebugData_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxDongleProtection_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxDOSExtender_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxFormat_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxGeneric_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxImage_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxInstaller_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxInstallerData_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxLibrary_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxLinker_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxNETObfuscator_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxPacker_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxProtector_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxProtectorData_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxSFX_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxSFXData_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxSignTool_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxSourceCode_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxStub_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxTool_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_pushButtonInfo_clicked()
{
    QMessageBox::information(this,tr("Info"),tr("Bugreports: horsicq@gmail.com"));
}


void GuiMainWindow::on_checkBoxPETool_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}


void GuiMainWindow::on_pushButtonSignatures_clicked()
{
    QString sInitDirectory=ui->lineEditSignatures->text();

    QString sDirectoryName=QFileDialog::getExistingDirectory(this,tr("Open signatures directory..."),sInitDirectory,QFileDialog::ShowDirsOnly);

    if(!sDirectoryName.isEmpty())
    {
        ui->lineEditSignatures->setText(sDirectoryName);
    }
}
