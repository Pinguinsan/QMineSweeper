/***********************************************************************
*    aboutqmswidget.cpp:                                               *
*    Custom QWidget for showing information about QMineSweeper         *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a source file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    The source code is released under the GNU LGPL                    *
*    This file holds the implementation of an AboutQmsWidget class     *
*    The AboutQmsWidget class is a window showing information about    *
*    QMineSweeper, using the same format used by the program GHex      *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "AboutQmsWidget.h"
#include "ui_AboutQmsWidget.h"

#include "GlobalDefinitions.h"
#include "QmsApplicationSettings.h"
#include "QmsStrings.h"

#include <QDesktopWidget>
#include <QRect>
#include <QDateTime>
#include <QPushButton>
#include <QFile>

#include <memory>

AboutQmsWidget::AboutQmsWidget(QWidget *parent) :
    MouseMoveableQWidget{parent},
    m_ui{new Ui::AboutQmsWidget{}},
    m_licenseHiddenHeight{-1}
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

    this->connect(this->m_ui->btnLicense, &QPushButton::clicked, this, &AboutQmsWidget::onLicenseButtonClicked);
    this->connect(this->m_ui->btnCloseDialog, &QPushButton::clicked, this, &AboutQmsWidget::onCloseButtonClicked);
}

AboutQmsWidget::~AboutQmsWidget()
{

}

/* onAboutQmsWindowLicenseButtonClicked() : called when the license button on the
 * AboutQmsWindow is clicked. This method displays the licence for QMineSweeper*/
void AboutQmsWidget::onLicenseButtonClicked(bool checked)
{
    (void)checked;
    if (this->m_ui->tbAboutQmsLicense->isVisible()) {
        this->clearLicenseText();
        /*
        if (this->m_licenseHiddenHeight != -1) {
            QRect thisGeometry{this->geometry()};
            thisGeometry.setHeight(this->m_licenseHiddenHeight);
            this->setGeometry(thisGeometry);
        }
        */
    } else {
        /*
        if (this->m_licenseHiddenHeight == -1) {
            this->m_licenseHiddenHeight = this->height();
        }
        */
        this->populateLicenseText();
    }
    this->m_ui->tbAboutQmsLicense->setVisible(!this->m_ui->tbAboutQmsLicense->isVisible());
    //this->setFixedSize(this->minimumSize());
}

void AboutQmsWidget::populateLicenseText()
{
    QFile licenseFile{QmsStrings::QMINESWEEPER_LICENSE_PATH};
    licenseFile.open(QIODevice::OpenModeFlag::ReadOnly);
    this->m_ui->tbAboutQmsLicense->append(licenseFile.readAll());
    licenseFile.close();
    this->m_ui->tbAboutQmsLicense->moveCursor(QTextCursor::Start);
    this->m_ui->tbAboutQmsLicense->ensureCursorVisible();
}

void AboutQmsWidget::clearLicenseText()
{
    this->m_ui->tbAboutQmsLicense->clear();
}

/* onAboutQmsWindowCloseButtonClicked() : called when the close button on the
 * AboutQmsWindow is clicked. This method closes the window */
void AboutQmsWidget::onCloseButtonClicked(bool checked)
{
    (void)checked;
    this->close();
}

void AboutQmsWidget::closeEvent(QCloseEvent *ce)
{
    emit (aboutToClose());
    MouseMoveableQWidget::closeEvent(ce);
}

