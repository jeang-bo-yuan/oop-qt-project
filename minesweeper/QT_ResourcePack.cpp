/**
 * \file QT_ResourcePack.cpp
 * \details implement QT_ResourcePack class
 */
#include "QT_ResourcePack.h"

QT_ResourcePack::QT_ResourcePack(const QDir& _dir)
    : dir(_dir)
{
    QDir iconPath(dir.absolutePath() + "/images");
    // 將所有icon載入
    for(QString& fileName : iconPath.entryList({"*.png"})) {
        iconMap.insert(std::make_pair(fileName, QIcon(iconPath.absoluteFilePath(fileName))));
    }
    // 若有人索取未知的Icon，回傳它
    iconMap.insert(std::make_pair("unknown", QIcon(iconPath.absoluteFilePath("unknown.png"))));

    QDir soundPath(dir.absolutePath() + "/sounds");
    // 將所有sound載入
    for (QString& fileName : soundPath.entryList({"*.wav", "*.mp3"})) {
        soundMap.insert(std::make_pair(fileName, QUrl::fromLocalFile(soundPath.absoluteFilePath(fileName))));
    }
    // "unknown" sound (null url)
    soundMap.insert(std::make_pair("unknown", QUrl()));
}

QT_ResourcePack::~QT_ResourcePack() {}

// getter /////////////////////////////////////////////////////////////////////////////////////////////////////////

const QIcon& QT_ResourcePack::getIcon(const QString& _name) const {
    auto it = iconMap.find(_name);
    if (it == iconMap.end())
        return iconMap.at("unknown");
    else
        return it->second;
}

QUrl QT_ResourcePack::getSoundUrl(const QString& _name) const {
    auto it = soundMap.find(_name);
    if (it == soundMap.cend())
        return soundMap.at("unknown");
    else
        return it->second;
}
