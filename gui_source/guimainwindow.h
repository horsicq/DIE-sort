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

#ifndef GUIMAINWINDOW_H
#define GUIMAINWINDOW_H

#include "../global.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include "dialogscanprogress.h"

namespace Ui {
class GuiMainWindow;
}

class GuiMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GuiMainWindow(QWidget *parent=nullptr);
    ~GuiMainWindow();

private slots:
    void on_pushButtonExit_clicked();
    void on_pushButtonOpenDirectory_clicked();
    void on_pushButtonOut_clicked();
    void on_pushButtonScan_clicked();
    void _scan();

    void on_checkBoxAllFileTypes_toggled(bool checked);
    void on_checkBoxBinary_toggled(bool checked);
    void on_checkBoxText_toggled(bool checked);
    void on_checkBoxMSDOS_toggled(bool checked);
    void on_checkBoxPE32_toggled(bool checked);
    void on_checkBoxPE64_toggled(bool checked);
    void on_checkBoxELF32_toggled(bool checked);
    void on_checkBoxELF64_toggled(bool checked);
    void on_checkBoxMACHO32_toggled(bool checked);
    void on_checkBoxMACHO64_toggled(bool checked);
    void on_checkBoxAllTypes_toggled(bool checked);
    void on_checkBoxArchive_toggled(bool checked);
    void on_checkBoxCertificate_toggled(bool checked);
    void on_checkBoxCompiler_toggled(bool checked);
    void on_checkBoxConverter_toggled(bool checked);
    void on_checkBoxDatabase_toggled(bool checked);
    void on_checkBoxDebugData_toggled(bool checked);
    void on_checkBoxDongleProtection_toggled(bool checked);
    void on_checkBoxDOSExtender_toggled(bool checked);
    void on_checkBoxFormat_toggled(bool checked);
    void on_checkBoxGeneric_toggled(bool checked);
    void on_checkBoxImage_toggled(bool checked);
    void on_checkBoxInstaller_toggled(bool checked);
    void on_checkBoxInstallerData_toggled(bool checked);
    void on_checkBoxLibrary_toggled(bool checked);
    void on_checkBoxLinker_toggled(bool checked);
    void on_checkBoxNETObfuscator_toggled(bool checked);
    void on_checkBoxPacker_toggled(bool checked);
    void on_checkBoxProtector_toggled(bool checked);
    void on_checkBoxProtectorData_toggled(bool checked);
    void on_checkBoxSFX_toggled(bool checked);
    void on_checkBoxSFXData_toggled(bool checked);
    void on_checkBoxSignTool_toggled(bool checked);
    void on_checkBoxSourceCode_toggled(bool checked);
    void on_checkBoxStub_toggled(bool checked);
    void on_checkBoxTool_toggled(bool checked);
    void on_pushButtonInfo_clicked();
    void on_checkBoxPETool_toggled(bool checked);
    void on_pushButtonSignatures_clicked();

private:
    Ui::GuiMainWindow *ui;

    ScanProgress::SCAN_OPTIONS options;
};

#endif // GUIMAINWINDOW_H
