
#ifndef QT_RESOURCEPACK_H
#define QT_RESOURCEPACK_H

#include <map>
#include <QDir>
#include <QIcon>
#include <QUrl>

/**
 * @brief 管理資源檔的class
 * @details
 * 1. icon 路徑；< QT_ResourcePack::absolutePath >/images    (*.png)
 * 2. sound 路徑：< QT_ResourcePack::absolutePath >/sounds    (*.mp3 ; *.wav)
 */
class QT_ResourcePack
{
    //! resource directory
    QDir dir;
    //! map between filename and QIcon
    std::map<QString, QIcon> iconMap;
    //! map between filename and url to sound effect
    std::map<QString, QUrl> soundMap;

public:
    /**
     * @brief constructor
     * @param _dir - 包含多媒體資源的資料夾， QT_ResourcePack 會從這讀取資源檔
     */
    QT_ResourcePack(const QDir& _dir);

    /**
     * @brief destructor
     * @details 清掉載入的資源
     */
    ~QT_ResourcePack();

    /**
     * @brief absolutePath
     * @return 目前這個 QT_ResourcePack 是從哪讀取資源檔
     */
    QString absolutePath() const { return dir.absolutePath(); }

    /**
     * @brief 從檔案_name取得QIcon
     * @param _name - 檔案名稱（相對於 QT_ResourcePack::absolutePath + "/images"）
     * @return 你要的QIcon，或是"unknown"如果找不到的話
     */
    const QIcon& getIcon(const QString& _name) const;

    /**
     * @brief 取得音檔的Url
     * @param _name - 檔案名稱（相對於 QT_ResourcePack::absolutePath + "/sounds"）
     * @return 你要的QSoundEffect的QUrl，或是"unknown"的如果找不到的話
     */
    QUrl getSoundUrl(const QString& _name) const;
};

#endif // QT_RESOURCEPACK_H
