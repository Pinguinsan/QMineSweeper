/***********************************************************************
*    aboutqmsdialog.cpp:                                               *
*    Custom QDialog for showing information about QMineSweeper         *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a source file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    The source code is released under the GNU LGPL                    *
*    This file holds the implementation of an AboutQmsDialog class     *
*    The AboutQmsDialog class is a window showing information about    *
*    QMineSweeper, using the same format used by the program GHex      *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "aboutqmsdialog.h"
#include "ui_aboutqmsdialog.h"

#include "globaldefinitions.h"
#include "qmsapplicationsettings.h"
#include "qmsstrings.h"

#include <QDesktopWidget>
#include <QRect>
#include <QDateTime>
#include <QPushButton>
#include <QFile>

#include <memory>

AboutQmsDialog::AboutQmsDialog() :
    m_ui{new Ui::AboutQmsDialog{}},
    m_licenseHiddenHeight{-1},
    m_xPlacement{0},
    m_yPlacement{0}
{
    using namespace QmsGlobalSettings;
    using namespace QmsStrings;

    this->m_ui->setupUi(this);

    /* initialize all strings and stuff for the AboutQmsWindow and connect slots */
    QString currentText{this->m_ui->lblProgramCopyright->text()};
    QString currentYear{QS_NUMBER(QDateTime{}.currentDateTime().date().year())};
    this->m_ui->lblProgramCopyright->setText(QString{"%1 %2 %3"}.arg(currentText, currentYear, AUTHOR_NAME));
    this->m_ui->lblProgramVersion->setText(QString{"v%1.%2.%3"}.arg(QS_NUMBER(SOFTWARE_MAJOR_VERSION), QS_NUMBER(SOFTWARE_MINOR_VERSION), QS_NUMBER(SOFTWARE_PATCH_VERSION)));
    this->m_ui->lblProgramWebsite->setText(QString{"<html><head/><body><p><a href=\"%1\"><span style=\" text-decoration: underline; color:#007af4;\">QMineSweeper website</span></a></p></body></html>"}.arg(REMOTE_URL));
    this->m_ui->lblProgramWebsite->setOpenExternalLinks(true);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->m_ui->tbAboutQmsLicense->setVisible(false);

    this->connect(this->m_ui->btnLicense, &QPushButton::clicked, this, &AboutQmsDialog::onLicenseButtonClicked);
    this->connect(this->m_ui->btnCloseDialog, &QPushButton::clicked, this, &AboutQmsDialog::onCloseButtonClicked);
}

AboutQmsDialog::~AboutQmsDialog()
{

}

/* onAboutQmsWindowLicenseButtonClicked() : called when the license button on the
 * AboutQmsWindow is clicked. This method displays the licence for QMineSweeper*/
void AboutQmsDialog::onLicenseButtonClicked(bool checked)
{
    (void)checked;
    if (this->m_ui->tbAboutQmsLicense->isVisible()) {
        this->clearLicenseText();
        if (this->m_licenseHiddenHeight != -1) {
            QRect thisGeometry{this->geometry()};
            thisGeometry.setHeight(this->m_licenseHiddenHeight);
            this->setGeometry(thisGeometry);
        }
    } else {
        if (this->m_licenseHiddenHeight == -1) {
            this->m_licenseHiddenHeight = this->height();
        }
        this->populateLicenseText();
    }
    this->m_ui->tbAboutQmsLicense->setVisible(!this->m_ui->tbAboutQmsLicense->isVisible());
    //this->setFixedSize(this->minimumSize());
}

void AboutQmsDialog::populateLicenseText()
{
    QFile licenseFile{QmsStrings::QMINESWEEPER_LICENSE_PATH};
    licenseFile.open(QIODevice::OpenModeFlag::ReadOnly);
    this->m_ui->tbAboutQmsLicense->append(licenseFile.readAll());
    licenseFile.close();
    this->m_ui->tbAboutQmsLicense->moveCursor(QTextCursor::Start);
    this->m_ui->tbAboutQmsLicense->ensureCursorVisible();
}

void AboutQmsDialog::clearLicenseText()
{
    this->m_ui->tbAboutQmsLicense->clear();
}

/* onAboutQmsWindowCloseButtonClicked() : called when the close button on the
 * AboutQmsWindow is clicked. This method closes the window */
void AboutQmsDialog::onCloseButtonClicked(bool checked)
{
    (void)checked;
    this->close();
}

/* centerAndFitWindow() : Called whenever code requests the main window to be centered
 * Typically called after resizing the UI on this widget / window */
void AboutQmsDialog::centerAndFitWindow(QDesktopWidget *desktopWidget)
{
    this->setFixedSize(this->minimumSize());
    this->calculateXYPlacement(desktopWidget);
    this->move(this->m_xPlacement, this->m_yPlacement);
}

/* calculateXYPlacement() : Checks the currently available screen geometry, and calulates
 * where the widget must be moved to appear on at the center of the users screen */
void AboutQmsDialog::calculateXYPlacement(QDesktopWidget *desktopWidget)
{
    std::unique_ptr<QRect> avail{new QRect{desktopWidget->availableGeometry()}};
    this->m_xPlacement = (avail->width()/2)-(this->width()/2);
#if defined(__ANDROID__)
    this->m_yPlacement = avail->height() - this->height();
#else
    this->m_yPlacement = (avail->height()/2)-(this->height()/2);
#endif
}


void AboutQmsDialog::closeEvent(QCloseEvent *ce)
{
    emit (aboutToClose());
    QDialog::closeEvent(ce);
}

