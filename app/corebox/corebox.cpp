﻿/*
CoreBox is combination of some common desktop apps.

CoreBox is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; version 2
of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see {http://www.gnu.org/licenses/}. */

#include "corebox.h"
#include "ui_corebox.h"

#include <QDateTime>
#include <QSettings>
#include <QPoint>

#include "start/start.h"
#include "about/about.h"
#include "help/help.h"
#include "search/search.h"
#include "dashboard/dashboard.h"
#include "corepad/corepad.h"
#include "corefm/corefm.h"
#include "corepaint/corepaint.h"
#include "coreimage/coreimage.h"
#include "coreplayer/coreplayer.h"
#include "coreaction/coreaction.h"
#include "coretime/coretime.h"
#include "corepdf/corepdf.h"
#include "coreterminal/coreterminal.h"
#include "corerenamer/corerenamer.h"

#include "../bookmarks/bookmarks.h"
#include "../bookmarks/bookmarkmanage.h"
#include "../settings/settings.h"
#include "globalfunctions.h"

CoreBox::CoreBox(QWidget *parent) : QMainWindow(parent), ui(new Ui::CoreBox)
{
    ui->setupUi(this);

    //setWindowOpacity(0.95);
    //setAttribute(Qt::WA_TranslucentBackground);

    //set a icon set for the whole app
    QIcon::setThemeName(sm.getThemeName());

    //setup framless window
    ui->windows->tabBar()->installEventFilter(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::SubWindow);
    ui->restoreButton->setVisible(false);
//    ui->windows->setCornerWidget(ui->winbutn, Qt::BottomRightCorner);

    //make the resizeable window button
//    QSizeGrip *sizeGrip = new QSizeGrip(this);
//    sizeGrip->setStyleSheet("QWidget{background-color: #EFEFEF ; width: 16px;height: 16px; background-image: url(:/icons/expand_b.svg); background-repeat: no-repeat ;background-position: center center ;}");
//    ui->resize->addWidget(sizeGrip);

    //setup window size and state
    if (sm.getBoxIsMaximize()) {
        on_maximizeButton_clicked();
    } else {
        int x = screensize().width()  * .8;
        int y = screensize().height() * .8;
        this->resize(x, y);
    }

    BookmarkManage bk;
    bk.checkBook();

    setupFolder();

}

CoreBox::~CoreBox()
{
    delete ui;
}

//===========================WindowBar========Start===============================================================

void CoreBox::tabEngine(AppsName i,const QString arg) // engine to open app in window
{
    int n = ui->windows->count();

    switch (i) {
    case CoreFM: {
        corefm *cFM = new corefm();

        QString str = checkIsValidDir(arg);
        if (!str.isEmpty() || !str.isNull()) cFM->goTo(str);

        ui->windows->insertTab(n, cFM, QIcon(":/icons/CoreFM.svg"), "CoreFM");
        ui->windows->setCurrentIndex(n);
        break;
    }
    case CoreImage: {
        coreimage *cIMG = new coreimage();

        QString str = checkIsValidFile(arg);
        if (!str.isEmpty() || !str.isNull()) cIMG->loadFile(str);

        ui->windows->insertTab(n, cIMG, QIcon(":/icons/CoreImage.svg"), "CoreImage");
        ui->windows->setCurrentIndex(n);
        break;
    }
    case CorePad: {
        int nn = filterEngine("CorePad");

        if (nn != 404) {
            ui->windows->setCurrentIndex(nn);
            corepad *cPad = static_cast<corepad*>(ui->windows->currentWidget());
            cPad->initializeNewTab(checkIsValidFile(arg));
        } else {
            corepad *cPAD = new corepad();
            cPAD->openText(checkIsValidFile(arg));

            ui->windows->insertTab(n, cPAD, QIcon(":/icons/CorePad.svg"), "CorePad");
            ui->windows->setCurrentIndex(n);
        }
        break;
    }
    case CorePaint: {
        corepaint *cPAINT = new corepaint();

        const QString str = checkIsValidFile(arg);
        if (!str.isEmpty() || !str.isNull()) cPAINT->initializeNewTab(true, str);
        else cPAINT->initializeNewTab();

        ui->windows->insertTab(n, cPAINT, QIcon(":/icons/CorePaint.svg"), "CorePaint");
        ui->windows->setCurrentIndex(n);
        break;
    }
    case CorePlayer: {
        coreplayer *cPLAYER = new coreplayer();

        const QString str = checkIsValidFile(arg);
        if (!str.isEmpty() || !str.isNull()) cPLAYER->openPlayer(str);

        ui->windows->insertTab(n, cPLAYER, QIcon(":/icons/CorePlayer.svg"), "CorePlayer");
        ui->windows->setCurrentIndex(n);
        break;
    }
    case Dashboard: {
        int nn = filterEngine("DashBoard");

        if (nn != 404) ui->windows->setCurrentIndex(nn);
        else {
            ui->windows->insertTab(n, new dashboard(), QIcon(":/icons/DashBoard.svg"), "DashBoard");
            ui->windows->setCurrentIndex(n);
        }
        break;
    }
    case Bookmarks: {
        int nn = filterEngine("Bookmarks");

        if (nn != 404) ui->windows->setCurrentIndex(nn);
        else {
            ui->windows->insertTab(n, new bookmarks(), QIcon(":/icons/Bookmarks.svg"), "Bookmarks");
            ui->windows->setCurrentIndex(n);
        }
        break;
    }
    case About: {
        int nn = filterEngine("About");

        if (nn != 404) ui->windows->setCurrentIndex(nn);
        else {
            ui->windows->insertTab(n, new about(), QIcon(":/icons/About.svg"), "About");
            ui->windows->setCurrentIndex(n);
        }
        break;
    }
    case StartView: {
        int nn = filterEngine("Start");

        if (nn != 404) ui->windows->setCurrentIndex(nn);
        else {
            ui->windows->insertTab(n, new Start(), QIcon(":/icons/Start.svg"), "Start");
            ui->windows->setCurrentIndex(n);
        }
        break;
    }
    case Help: {
        int nn = filterEngine("Help");

        if (nn != 404) ui->windows->setCurrentIndex(nn);
        else {
            ui->windows->insertTab(n, new help(), QIcon(":/icons/Help.svg"), "Help");
            ui->windows->setCurrentIndex(n);
        }
        break;
    }
    case Settings: {
        int nn = filterEngine("Settings");

        if (nn != 404) ui->windows->setCurrentIndex(nn);
        else {
            ui->windows->insertTab(n, new settings(), QIcon(":/icons/Settings.svg"), "Settings");
            ui->windows->setCurrentIndex(n);
        }
        break;
    }
    case Search: {
        search *ser = new search();

        const QString str = checkIsValidDir(arg);
        if (!str.isEmpty() || !str.isNull()) ser->setPath(str);

        ui->windows->insertTab(n, ser, QIcon(":/icons/Search.svg"), "Search");
        ui->windows->setCurrentIndex(n);
        break;
    }
    case CoreTime: {
        int nn = filterEngine("CoreTime");

        if (nn != 404) ui->windows->setCurrentIndex(nn);
        else {
            ui->windows->insertTab(n, new coretime, QIcon(":/icons/CoreTime.svg"), "CoreTime");
            ui->windows->setCurrentIndex(n);
        }
        break;
    }
    case Corebox: {
        CoreBox *cBox = new CoreBox();
        cBox->show();
        cBox->tabEngine(StartView);
        break;
    }
    case CorePDF: {
        corepdf *cPDF = new corepdf();

        const QString str = checkIsValidFile(arg);
        if (!str.isEmpty() || !str.isNull()) cPDF->openPdfFile(str);

        ui->windows->insertTab(n, cPDF, QIcon(":/icons/CorePDF.svg"), "CorePDF");
        ui->windows->setCurrentIndex(n);
        break;
    }
    case CoreTerminal: {
        QString workDir = arg;
        if (!arg.count())
            workDir = QDir::homePath();

        if (QFileInfo(workDir).isFile())
            workDir = QFileInfo(arg).path();

        coreterminal *trm = new coreterminal(workDir, "");
        ui->windows->insertTab(n, trm, QIcon(":/icons/CoreTerminal.svg"), "CoreTerminal");
        ui->windows->setCurrentIndex(n);
        break;
    }
    case CoreRenamer: {
        corerenamer *cFM = new corerenamer();

        const QString str = checkIsValidDir(arg);
        if (!str.isEmpty() || !str.isNull()) cFM->addPath(str);

        ui->windows->insertTab(n, cFM, QIcon(":/icons/CoreRenemer.svg"), "CoreRenamer");
        ui->windows->setCurrentIndex(n);

        break;
    }
    default:
        break;
    }
}

void CoreBox::on_windows_tabCloseRequested(int index)
{
    const QString appName = ui->windows->tabBar()->tabText(index);

    if (appName == "Bookmarks") {
        bookmarks *cbook = static_cast<bookmarks*>(ui->windows->widget(index));
        if (cbook->close()){
            cbook->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "About") {
        about *cabout = static_cast<about*>(ui->windows->widget(index));
        if (cabout->close()){
            cabout->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "DashBoard") {
        dashboard *cdash = static_cast<dashboard*>(ui->windows->widget(index));
        if (cdash->close()){
            cdash->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "Start") {
        Start *cstart = static_cast<Start*>(ui->windows->widget(index));
        if (cstart->close()){
            cstart->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "Search") {
        search *csearch = static_cast<search*>(ui->windows->widget(index));
        if (csearch->close()){
            csearch->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "Help") {
        help *chelp = static_cast<help*>(ui->windows->widget(index));
        if (chelp->close()){
            chelp->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "Settings") {
        settings *csettings = static_cast<settings*>(ui->windows->widget(index));
        if (csettings->close()){
            csettings->deleteLater();
            ui->windows->removeTab(index);
        }
    }
    else if (appName == "CorePaint") {
        corepaint *cpaint = static_cast<corepaint*>(ui->windows->widget(index));
        if (cpaint->close()){
            cpaint->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "CorePlayer") {
        coreplayer *cplay = static_cast<coreplayer*>(ui->windows->widget(index));
        if (cplay->close()){
            cplay->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "CoreFM") {
        corefm *cfm = static_cast<corefm*>(ui->windows->widget(index));
        if (cfm->close()){
            cfm->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "CorePad") {
        corepad *cpad = static_cast<corepad*>(ui->windows->widget(index));
        if (cpad->close()){
            cpad->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "CoreImage") {
        coreimage *cimg = static_cast<coreimage*>(ui->windows->widget(index));
        if (cimg->close()){
            cimg->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "CoreTime") {
        coretime *ctim = static_cast<coretime*>(ui->windows->widget(index));
        if (ctim->close()){
            ctim->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "CorePDF") {
        corepdf *cpdf = static_cast<corepdf*>(ui->windows->widget(index));
//        cpdf->eclose();
        if (cpdf->close()){
            cpdf->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "CoreTerminal") {
        coreterminal *ctrm = static_cast<coreterminal*>(ui->windows->widget(index));
        if (ctrm->close()){
            ctrm->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "CoreRenamer") {
        corerenamer *cren = static_cast<corerenamer*>(ui->windows->widget(index));
        if (cren->close()){
            cren->deleteLater();
            ui->windows->removeTab(index);
        }
    }

    if(ui->windows->count() == 0){
        on_start_clicked();
    }
}

int CoreBox::filterEngine(QString name) // engine for find if a app is opened
{
    for(int i= 0; i < ui->windows->count(); ++i) {
        if(ui->windows->tabText(i) == name) {
            return i ;
        }
    }
    return 404;
}

void CoreBox::on_windows_currentChanged(int index) // set window title related to current selected app
{
    QString title = ui->windows->tabText(index);
    this->setWindowTitle(title);
    this->setWindowIcon(appsIcon(title));
}

void CoreBox::on_windows_tabBarClicked(int index) // reload the apps if related app is clicked
{
    QString appName = ui->windows->tabBar()->tabText(index);

    if (appName == "Bookmarks") {
        bookmarks *cbook = ui->windows->findChild<bookmarks*>("bookmarks");
        cbook->reload();
    } else if (appName == "DashBoard") {
        dashboard *cdash = ui->windows->findChild<dashboard*>("dashboard");
        cdash->reload();
    } else if (appName == "Start") {
        Start *cstart = ui->windows->findChild<Start*>("Start");
        cstart->reload();
    }
}

void CoreBox::closeCurrentTab()
{
    on_windows_tabCloseRequested(ui->windows->currentIndex());
}

void CoreBox::on_maximizeButton_clicked()
{
    ui->restoreButton->setVisible(true);
    ui->maximizeButton->setVisible(false);
    this->setWindowState(Qt::WindowMaximized);
}

void CoreBox::on_restoreButton_clicked()
{
    ui->restoreButton->setVisible(false);
    ui->maximizeButton->setVisible(true);
    this->setWindowState(Qt::WindowNoState);
}

void CoreBox::on_minimizeButton_clicked()
{
    this->setWindowState(Qt::WindowMinimized);
}

void CoreBox::on_closeButton_clicked()
{
    this->close();
}

void CoreBox::doubleClicked()
{
  if (windowState().testFlag(Qt::WindowNoState)) {
    on_maximizeButton_clicked();
  } else if (windowState().testFlag(Qt::WindowMaximized)) {
    on_restoreButton_clicked();
  }
}

void CoreBox::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::WindowStateChange) {
    if (windowState().testFlag(Qt::WindowNoState)) {
      on_restoreButton_clicked();
      event->ignore();
    } else if (windowState().testFlag(Qt::WindowMaximized)) {
      on_maximizeButton_clicked();
      event->ignore();
    }
  }
  event->accept();
}

void CoreBox::mousePressEvent(QMouseEvent *event)
{
    if(ui->windows->tabBar()->underMouse() || ui->sidebar->underMouse() && event->buttons()== Qt::LeftButton){
        isMouseDown = true;
        isLeftDown = true;
        mousePos = event->globalPos();
        wndPos = this->pos();
        qDebug() << "1st Left mouse clicked";
    }
//    else if (event->buttons()== Qt::RightButton){
//        isMouseDown = true;
//        isRightDown = true;
//        mousePos = event->globalPos();
//        wndPos = this->pos();
//        qDebug() << "1st Right mouse clicked";
//    }
}

void CoreBox::mouseMoveEvent(QMouseEvent *event)
{
    if(isMouseDown==true){
//        int mousePointx = wndPos.x() + (event->globalX() - mousePos.x());
//        int mousePointy = wndPos.y() + (event->globalY() - mousePos.y());

        if ((ui->windows->tabBar()->underMouse() || ui->sidebar->underMouse()) && isLeftDown == true){
            move(wndPos + (event->globalPos() - mousePos));
            qDebug() << "2nd Left mouse clicked";
        }
//        else if (isRightDown == true){
//            resize((mousePointx),(mousePointy));
//            qDebug() << "2nd Right mouse clicked";
//        }
    }
}

void CoreBox::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    isMouseDown = false;
    isLeftDown = false;
    isRightDown = false;
}

bool CoreBox::eventFilter(QObject *obj, QEvent *evt)
{
    if (obj == ui->windows && evt->type() == QEvent::MouseButtonDblClick){
        doubleClicked();
    }

    return QMainWindow::eventFilter(obj,evt);
}

//===========================WindowBar========End=================================================================

void CoreBox::closeEvent(QCloseEvent *event)
{
    event->ignore();
    if (ui->windows->count() > 1) {
        for (int i = ui->windows->count() - 1; i >= 1; --i) {
            on_windows_tabCloseRequested(i);
        }
    }
    event->accept();
}

void CoreBox::paintEvent(QPaintEvent *event)
{
//    QRgb _blend(qRgba(0,0,0,0xff));
//    QColor color(_blend);
//    color.setAlphaF(0.75);
//    _blend = color.rgba();

//    QPainter paint(this);
//    paint.setOpacity(0.8);

//    const auto rects = (event->region() & contentsRect()).rects();
//    //qDebug() << rects;
//    for (const QRect &rect : rects)
//    {
//        QColor col(QColor::fromRgb(255,255,255));
//        col.setAlpha(qAlpha(_blend));
//        paint.save();
//        paint.setCompositionMode(QPainter::CompositionMode_Source);
//        paint.fillRect(rect, col);
//        paint.restore();
//    }
}

void CoreBox::on_bookmarks_clicked()
{
    tabEngine(Bookmarks);
}

void CoreBox::on_dashboard_clicked()
{
    tabEngine(Dashboard);
}

void CoreBox::on_settings_clicked()
{
    tabEngine(Settings);
}

void CoreBox::on_start_clicked()
{
    tabEngine(StartView);
}

void CoreBox::on_search_clicked()
{
    tabEngine(Search);
}

void CoreBox::on_corepaint_clicked()
{
    tabEngine(CorePaint);
}

void CoreBox::on_corefm_clicked()
{
    tabEngine(CoreFM);
}

void CoreBox::on_corepad_clicked()
{
    tabEngine(CorePad);
}

void CoreBox::on_box_clicked()
{
    messageEngine("Thanks for using CoreBox\nVersion 2.2", MessageType::Info);
}

void CoreBox::on_bookAll_clicked()
{
    BookmarkManage bk;
    if (bk.getBookSections().contains("LastUsed"))
        bk.delSection("LastUsed");

    int count = 0;
    bk.addSection("LastUsed");
    for (int i = 0; i < ui->windows->count(); i++) {
        const QString path = ui->windows->widget(i)->windowFilePath();
        if (path.count()) {
            bk.addBookmark("LastUsed", QFileInfo(path).fileName(), path);
            count++;
        }
    }

    if (!count) bk.delSection("LastUsed");
}
