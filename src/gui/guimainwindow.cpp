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

#include <QDir>

GuiMainWindow::GuiMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::GuiMainWindow)
{
    ui->setupUi(this);

#ifdef USE_XSIMD
    xsimd_init();
#endif

    setWindowTitle(XOptions::getTitle(X_APPLICATIONDISPLAYNAME, X_APPLICATIONVERSION));

    m_xOptions.setName(X_OPTIONSFILE);

    m_xOptions.addID(XOptions::ID_VIEW_STYLE, "Fusion");
    m_xOptions.addID(XOptions::ID_VIEW_LANG, "System");
    m_xOptions.addID(XOptions::ID_VIEW_FONT_CONTROLS, XOptions::getDefaultFont().toString());
    m_xOptions.addID(XOptions::ID_FEATURE_READBUFFERSIZE, 4 * 1024);
    m_xOptions.addID(XOptions::ID_FEATURE_FILEBUFFERSIZE, 64 * 1024 * 1024);
    m_xOptions.addID(XOptions::ID_FEATURE_SSE2, true);
    m_xOptions.addID(XOptions::ID_FEATURE_AVX2, true);
    m_xOptions.addID(XOptions::ID_VIEW_SIZES, "");
    m_xOptions.load();

    m_xShortcuts.setName(X_SHORTCUTSFILE);
    m_xShortcuts.setNative(m_xOptions.isNative(), m_xOptions.getApplicationDataPath());
    m_xShortcuts.load();

    // setGlobal() must run before setEngine(): setEngine() reads the global options to
    // seed its per-engine defaults, and XDialogProcess is handed the same pair on Scan.
    ui->widgetSort->setGlobal(&m_xShortcuts, &m_xOptions);
    ui->widgetSort->setEngine(&m_dieScript);

    QByteArray baGeometry = m_xOptions.getSizeRecord("MainWindow");

    if (!baGeometry.isEmpty()) {
        restoreGeometry(baGeometry);
    }
}

GuiMainWindow::~GuiMainWindow()
{
    m_xOptions.setSizeRecord("MainWindow", saveGeometry());
    m_xOptions.save();
    m_xShortcuts.save();

    delete ui;
}

void GuiMainWindow::on_pushButtonExit_clicked()
{
    this->close();
}

void GuiMainWindow::on_pushButtonInfo_clicked()
{
    QString sInfo = QString("%1 v%2\n\n%3\n%4")
                        .arg(X_APPLICATIONDISPLAYNAME, X_APPLICATIONVERSION, tr("Bugreports: horsicq@gmail.com"),
                             tr("Settings") + ": " + QDir::toNativeSeparators(m_xOptions.getApplicationDataPath()));

    QMessageBox::information(this, tr("Info"), sInfo);
}
