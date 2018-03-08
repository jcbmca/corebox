#ifndef SETTINGSMANAGE_H
#define SETTINGSMANAGE_H

#include <QSettings>
#include <QVariant>


class SettingsManage
{
public:
    SettingsManage();
    QSettings *cSetting;
    void createDefaultSettings();

    bool setSpecificValue(QString groupName, QString keyName, QVariant value);
    QVariant getSpecificValue(QString groupName, QString keyName);

    QString getTerminal();
    bool setTerminal(QString termName);

    QString getStartupPath();
    bool setStartupPath(QString path);

    QString getThemeName();
    bool setThemeName(QString themeName);

    QString getMimeFilePath();
    bool setMimeFilePath(QString mimeFilePath);

    bool getIsRealMimeType();
    bool setIsRealMimeType(bool isRealMime);

    int getZoomValue();
    bool setZoomValue(int value);

    int getZoomTreeValue();
    bool setZoomTreeValue(int value);

    int getZoomListValue();
    bool setZoomListValue(int value);

    int getZoomDetailValue();
    bool setZoomDetailValue(int value);

    bool getIsShowThumb();
    bool setIsShowThumb(bool isShowThumb);

    bool getShowToolbox();
    bool setShowToolbox(bool showTool);

    bool getViewMode();
    bool setViewMode(bool mode);

    int getSortColumn();
    bool setSortColumn(int value);

    int getSortOrder();
    bool setSortOrder(Qt::SortOrder order);

    QString getSCSaveLocation();
    bool setSCSaveLocation(QString path);

    bool getBoxIsMaximize();
    bool setBoxIsMaximize(bool isMaximize);

    bool getShowBattery();
    bool setSHowBattery(bool showBattery);

    bool getDisableRecent();
    bool setDisableRecent(bool showRecent);

    QString getTimeFormat();
    bool setTimeFormat(QString AmPm);

};

#endif // SETTINGSMANAGE_H