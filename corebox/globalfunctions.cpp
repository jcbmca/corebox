#include "globalfunctions.h"
#include "corebox.h"

#include <QProcess>
#include <QDebug>
#include <corefm/mimeutils.h>

#include "corepad/corepad.h"


bool moveToTrash(QString fileName)
{
    if (getfilesize(fileName) >= 1073741824) {
        QMessageBox::StandardButton replyC;
        replyC = QMessageBox::warning(qApp->activeWindow(), "Warning!", "File size is about 1 GB or larger.\nPlease delete it instead of moveing to trash.\nDo you want to delete it?", QMessageBox::Yes | QMessageBox::No);
        if (replyC == QMessageBox::No) {
            return false;
        } else {
            QFile::remove(fileName);
        }
    }
    else {
        QDir trash = QDir::home();
        trash.cd(".local/share/");
        trash.mkdir("Trash");
        trash.cd("Trash");
        trash.mkdir("files");
        trash.mkdir("info");

        QFile directorySizes(trash.path() + "/directorysizes");
        directorySizes.open(QFile::Append);

        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(qApp->activeWindow(), "Warning!", "Do you want to Trash the '" + fileName + "' ?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            QString fileLocation = fileName;
            if (QFile(fileLocation).exists()) {
                QFile(fileLocation).rename(trash.path() + "/files/" + QFileInfo(fileName).fileName());
            } else {
                QDir(QFileInfo(fileName).path()).rename(QFileInfo(fileName).fileName(), trash.path() + "/files/ " + QFileInfo(fileName).fileName());
            }
            QFile trashinfo(trash.path() + "/info/" + QFileInfo(fileName).fileName() + ".trashinfo");
            trashinfo.open(QFile::WriteOnly);
            trashinfo.write(QString("[Trash Info]\n").toUtf8());
            trashinfo.write(QString("Path=" + fileLocation + "\n").toUtf8());
            trashinfo.write(QString("DeletionDate=" + QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss") + "\n").toUtf8());trashinfo.close();

            messageEngine("File Moved to Trash", "Info");
        }
    return true;
    }
    return false;
}

bool saveToRecent(QString appName, QString pathName)
{
    SettingsManage sm;
    if (sm.getDisableRecent() == true) {
        if (!pathName.isEmpty()) {
            sm.cSetting->beginGroup("Recent");
            int keysCount = sm.cSetting->childKeys().count();
            sm.cSetting->endGroup();
            sm.setSpecificValue("Recent", QString::number(keysCount), appName + "$$$" + pathName + "$$$" + QDateTime::currentDateTime().toString());
            return true;
        }
    }
    return false;
}

void messageEngine(QString message, QString messageType)
{
    QLabel *l = new QLabel(message);
    QFont f ("Arial", 14, QFont::Bold);
    QWidget *mbox = new QWidget();
    QVBoxLayout *bi = new QVBoxLayout();
    mbox->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::ToolTip);
    mbox->setMinimumSize(230,50);
    mbox->setLayout(bi);
    l->setFont(f);
    bi->addWidget(l);
    bi->setContentsMargins(6, 6, 6, 6);
    if (messageType == "Info") {
        mbox->setStyleSheet("QWidget{background-color: #2A2A2A;color: #ffffff;border: 1px #2A2A2A; border-radius: 10px; padding: 5px 5px 5px 5px;}");
    } else if (messageType == "Warning") {
        mbox->setStyleSheet("QWidget{background-color: red;color: #ffffff;border: 1px #2A2A2A; border-radius: 10px; padding: 5px 5px 5px 5px;}");
    } else if (messageType == "Tips") {
        mbox->setStyleSheet("QWidget{background-color: blue;color: #ffffff;border: 1px #2A2A2A; border-radius: 10px; padding: 5px 5px 5px 5px;}");
    } else {
        return;
    }
    mbox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    mbox->show();

    int x = screensize().width() - (mbox->width() + 10);
    int y = screensize().height() - (mbox->height() + 10);

    mbox->move(x,y);
    QTimer::singleShot(3000, mbox, SLOT(close()));

}

AppsName nameToInt(QString appName)
{
    if (appName == "CoreFM" || appName == "corefm") {
        return CoreFM;
    } else if (appName == "CoreImage" || appName == "coreimage") {
        return CoreImage;
    } else if (appName == "CorePad" || appName == "corepad") {
        return CorePad;
    } else if (appName == "CorePaint" || appName == "corepaint") {
        return CorePaint;
    } else if (appName == "CorePlayer" || appName == "coreplayer") {
        return CorePlayer;
    } else if (appName == "DashBoard" || appName == "dashboard") {
        return Dashboard;
    } else if (appName == "Bookmarks" || appName == "bookmarks") {
        return Bookmarks;
    } else if (appName == "About" || appName == "about") {
        return About;
    } else if (appName == "Start" || appName == "start") {
        return StartView;
    } else if (appName == "Help" || appName == "help") {
        return Help;
    } else if (appName == "Settings" || appName == "settings") {
        return Settings;
    } else if (appName == "Search" || appName == "search") {
        return Search;
    } else if (appName == "CoreTime" || appName == "coretime") {
        return CoreTime;
    } else if (appName == "CoreBox" || appName == "corebox") {
        return Corebox;
    } else if (appName == "CoreRenamer" || appName == "corerenamer") {
        return CoreRenamer;
    } else if (appName == "CorePDF" || appName == "corepdf") {
        return CorePDF;
    } else {
        return damn;
    }
}

QString appsIconPath(QString appName)
{
    QString str = ":/icons/";
    if (appName == "CoreFM" || appName == "corefm") {
        return str + "CoreFM.svg";
    } else if (appName == "CoreImage" || appName == "coreimage") {
        return str + "CoreImage.svg";
    } else if (appName == "CorePad" || appName == "corepad") {
        return str + "CorePad.svg";
    } else if (appName == "CorePaint" || appName == "corepaint") {
        return str + "CorePaint.svg";
    } else if (appName == "CorePlayer" || appName == "coreplayer") {
        return str + "CorePlayer.svg";
    } else if (appName == "CoreTime" || appName == "coretime") {
        return str + "CoreTime.svg";
    } else if (appName == "DashBoard" || appName == "dashboard") {
        return str + "DashBoard.svg";
    } else if (appName == "Bookmarks" || appName == "bookmarks") {
        return str + "Bookmarks.svg";
    } else if (appName == "About" || appName == "about") {
        return str + "About.svg";
    } else if (appName == "Start" || appName == "start") {
        return str + "Start.svg";
    } else if (appName == "Search" || appName == "search") {
        return str + "Search.svg";
    } else if (appName == "Help" || appName == "help") {
        return str + "Help.svg";
    } else if (appName == "Settings" || appName == "settings") {
        return str + "Settings.svg";
    } else {
        return NULL;
    }
}

QString formatSize(qint64 num)
{
    QString total;
    const qint64 kb = 1024;
    const qint64 mb = 1024 * kb;
    const qint64 gb = 1024 * mb;
    const qint64 tb = 1024 * gb;

    if (num >= tb) total = QString("%1TB").arg(QString::number(qreal(num) / tb, 'f', 2));
    else if(num >= gb) total = QString("%1GB").arg(QString::number(qreal(num) / gb, 'f', 2));
    else if(num >= mb) total = QString("%1MB").arg(QString::number(qreal(num) / mb, 'f', 1));
    else if(num >= kb) total = QString("%1KB").arg(QString::number(qreal(num) / kb,'f', 1));
    else total = QString("%1 bytes").arg(num);

    return total;
}

qint64 getfilesize(QString path)
{
    QProcess p;
    QString commd = "du -sb --total \"" + path + "\"";
    p.start(commd.toLatin1());
    p.waitForFinished();
    QString output(p.readAllStandardOutput());
    return output.split("\n").at(1).split("	").at(0).toLongLong();
}

QString getFileSize(QString path)
{
    return formatSize(getfilesize(path));
}

qint64 getmultiplefilesize(QStringList paths)
{
    QString pathNames;
    for (int i = 0; i < paths.count(); i++) {
        pathNames = pathNames + " \"" + paths.at(i) + "\"";
    }
    QProcess p;
    QString commd = "du -sb --total " + pathNames;
    p.start(commd.toLatin1());
    p.waitForFinished();
    QString output(p.readAllStandardOutput());
    QStringList l = output.split("\n");
    return l.at(l.count() - 2).split("\t").at(0).toLongLong();
}

QString getMultipleFileSize(QStringList paths)
{
    return formatSize(getmultiplefilesize(paths));
}

void openAppEngine(QString path){

    CoreBox *cBox = qobject_cast<CoreBox*>(qApp->activeWindow());
    QFileInfo info(path);
    if(!info.exists() && !path.isEmpty()){
        messageEngine("File not exists","Warning");
        return;
    }

    QStringList image, txts;
    image << "jpg" << "jpeg" << "png" << "bmp" << "ico" << "svg" << "gif";
    txts << "txt" << "pro" << "";

    QString suffix = QFileInfo(path).suffix();

    //CoreFM
    if (info.isDir()) {
        cBox->tabEngine(CoreFM, info.absoluteFilePath());
        return;
    }

    //CoreImage
    else if (image.contains(suffix, Qt::CaseInsensitive)) {
        cBox->tabEngine(CoreImage, info.absoluteFilePath());
        return;
    }

    //CorePad
    else if (txts.contains(suffix, Qt::CaseInsensitive)) {
        cBox->tabEngine(CorePad, info.absoluteFilePath());
        return;
    }

    //sendtomimeutils
    else {
        QFileInfo p(path);
        MimeUtils *m = new MimeUtils(cBox);
        m->openInApp(p,cBox);
    }
}


QString checkIsValidDir(QString str) {
    if (str.isEmpty() || str.isNull()) {
        return NULL;
    } else {
        QFileInfo dir(str);
        if (dir.isDir()) {
            if (dir.isRoot()) return str;
            else {
                if (str.endsWith('/')) return str.remove(str.length() - 1, 1);
                else return str;
            }
        }
    }
    return NULL;
}

QString checkIsValidFile(QString str) {
    if (str.isEmpty() || str.isNull()) {
        return NULL;
    } else {
        QFileInfo fi(str);
        if (fi.isFile()) {
            return str;
        }
    }
    return NULL;
}

QRect screensize(){
    QScreen * screen = QGuiApplication::primaryScreen();
    return screen->availableGeometry();
}

QIcon geticon(const QString &filePath) {

    QIcon icon;
    QFileInfo info(filePath);

    QMimeDatabase mime;
    QMimeType mType;

    mType = mime.mimeTypeForFile(filePath);
    icon = QIcon::fromTheme(mType.iconName());

    if (icon.isNull())
      return QApplication::style()->standardIcon(QStyle::SP_FileIcon);
    else
      return icon;
}



QStringList fStringList(QStringList left, QStringList right, QFont font) {

    QFontMetrics *fm = new QFontMetrics(font);
    int large = 0;
    for (int i = 0; i < left.count(); i++) {
        int index;
        if (large < fm->width(left.at(i))) {
            large = fm->width(left.at(i));
            index = i;
        }
    }

    large = large + fm->width('\t');

    for (int i = 0; i < left.count(); i++) {
        while (large >= fm->width(left.at(i))) {
             left.replace(i, QString(left.at(i) + QString('\t')));
        }
    }

    for (int i = 0; i < left.count(); i++) {
        left.replace(i, left.at(i) + ": " + right.at(i));
    }

    return left;
}


QString getMountPathByName(QString displayName){

    if(displayName.isNull() || displayName.isEmpty()) return NULL;

    else {
        const auto allMounted = QStorageInfo::mountedVolumes();
        for(auto& singleMounted : allMounted){
            if (singleMounted.displayName() == displayName) return singleMounted.rootPath();
        }
    }
    return NULL;
}
