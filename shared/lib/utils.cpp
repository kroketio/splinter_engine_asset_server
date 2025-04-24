#include <QObject>
#include <QScreen>
#include <QJsonDocument>
#include <QClipboard>

#include "png.h"
#include "utils.h"
#include "lib/config.h"

void Utils::init() {
  globals::configRoot = QDir::homePath();
  globals::homeDir = QDir::homePath();
  globals::configDirectory = QString("%1/.config/%2/").arg(globals::configRoot, QCoreApplication::applicationName());
  globals::configDirectoryAutoMasker = QString("%1%2").arg(globals::configDirectory, "autoMasker");
  globals::configDirectoryAssets = QString("%1%2").arg(globals::configDirectory, "data");
  globals::configDirectoryAssetsTextures = QString("%1/%2").arg(globals::configDirectoryAssets, "textures");
  globals::configDirectoryAssetsModels = QString("%1/%2").arg(globals::configDirectoryAssets, "models");
  globals::configDirectoryAssetsU2net = QString("%1/%2").arg(globals::configDirectoryAssets, "u2net");
  globals::cacheDirectory = QString("%1/cache").arg(globals::configDirectory);
  globals::pathDatabase = QFileInfo(globals::configDirectory + QDir::separator() + "db.sqlite3");
  qDebug() << "path database:" << globals::pathDatabase;
  qDebug() << "cache database:" << globals::cacheDirectory;
}

bool Utils::readJsonFile(QIODevice &device, QSettings::SettingsMap &map) {
  QJsonDocument json = QJsonDocument::fromJson(device.readAll());
  map = json.object().toVariantMap();
  return true;
}

bool Utils::writeJsonFile(QIODevice &device, const QSettings::SettingsMap &map) {
  device.write(QJsonDocument(QJsonObject::fromVariantMap(map)).toJson(QJsonDocument::Indented));
  return true;
}

bool Utils::validateJSON(const QString &message) {
  return validateJSON(message.toUtf8());
}

bool Utils::validateJSON(const QByteArray &blob) {
    QJsonDocument doc = QJsonDocument::fromJson(blob);
    QString jsonString = doc.toJson(QJsonDocument::Indented);
    return !jsonString.isEmpty();
}

bool Utils::fileExists(const QString &path) {
  QFileInfo check_file(path);
  return check_file.exists() && check_file.isFile();
}

bool Utils::dirExists(const QString &path) {
  QDir pathDir(path);
  return pathDir.exists();
}

QByteArray Utils::fileTextOpen(const QString &path) {
  QFile file(path);
  if(!file.open(QFile::ReadOnly | QFile::Text)) {
    file.close();
    return QByteArray();
  }

  QByteArray data = file.readAll();
  file.close();
  return data;
}

QByteArray Utils::fileOpen(const QString &path) {
  QFile file(path);
  if(!file.open(QFile::ReadOnly)) {
    file.close();
    return QByteArray();
  }

  QByteArray data = file.readAll();
  file.close();
  return data;
}

QByteArray Utils::fileOpenQRC(const QString &path) {
  QFile file(path);
  if(!file.open(QIODevice::ReadOnly)) {
    qDebug() << "error: " << file.errorString();
  }

  QByteArray data = file.readAll();
  file.close();
  return data;
}

bool Utils::fileWrite(const QString &path, const QString &data) {
    QFile file(path);
    if(file.open(QIODevice::WriteOnly)){
        QTextStream out(&file); 
        out << data;
        file.close();
        return true;
    }
    file.close();
    return false;
}

QList<QFileInfo> Utils::fileFind(const QRegularExpression &pattern, const QString &baseDir, int level, int depth, const int maxPerDir) {
    // like `find /foo -name -maxdepth 2 "*.jpg"`
    QList<QFileInfo> rtn;
    QDir dir(baseDir);
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDot | QDir::NoDotDot);

    int fileCount = 0;
    for(const auto &fileInfo: dir.entryInfoList({"*"})) {
        fileCount += 1;
        if(fileCount > maxPerDir) return rtn;
        if(!fileInfo.isReadable())
            continue;

        const auto fn = fileInfo.fileName();
        const auto path = fileInfo.filePath();

        if (fileInfo.isDir()) {
            if (level + 1 <= depth)
                rtn << Utils::fileFind(pattern, path, level + 1, depth, maxPerDir);
        }
        else if (pattern.match(fn).hasMatch())
            rtn << QFileInfo(path);
    }
    return rtn;
}

bool Utils::isCyrillic(const QString &inp) {
  QRegularExpression re("[А-Яа-яЁё]+");
  QRegularExpressionMatch match = re.match(inp);
  return match.hasMatch();
}

RawImageInfo Utils::jpgInfo(const QFileInfo &path) {
  auto _path = path.absoluteFilePath();
  if(!Utils::fileExists(_path)) {
    qWarning() << "does not exist:" << path;
    return {};
  }

  QImageReader reader(_path);
  if (!reader.canRead())
      return {};

  QSize isize = reader.size();

  auto info = RawImageInfo();
  info.channels = 3;
  info.width = isize.width();
  info.height = isize.height();
  info.success = true;
  return info;
}

RawImageInfo Utils::pngInfo(const QFileInfo &path) {
  auto _path = path.absoluteFilePath();
  if(!Utils::fileExists(_path)) {
    qWarning() << "does not exist:" << path;
    return {};
  }

  FILE *fp = fopen(_path.toStdString().c_str(), "rb");
  if (fp == nullptr) {
    qWarning() << "Could not open" << _path << "for reading";
    return {};
  }

  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  // png_set_option(png, , 2);  // @TODO: libpng warning: iCCP: known incorrect sRGB profile
  if(!png) {
    qWarning() << "error reading png #1";
    fclose(fp);
    return {};
  }

  png_infop info = png_create_info_struct(png);
  if(!info) {
    qWarning() << "error reading png #2";
    fclose(fp);
    return {};
  }

  if(setjmp(png_jmpbuf(png))) {
    qWarning() << "error reading png #3";
    png_destroy_read_struct(&png, &info, nullptr);
    fclose(fp);
    return {};
  }

  png_init_io(png, fp);
  png_read_info(png, info);

  auto png_info = RawImageInfo();
  png_info.channels = png_get_channels(png, info);
  png_info.width = png_get_image_width(png, info);
  png_info.height = png_get_image_height(png, info);
  png_info.success = true;
  png_destroy_read_struct(&png, &info, nullptr);
  fclose(fp);
  return png_info;
}

bool Utils::portOpen(const QString &hostname, quint16 port){
    QTcpSocket socket;
    socket.connectToHost(hostname, port);
    return socket.waitForConnected(500);
}

QFileInfo Utils::tempFile(QString suffix) {
  // TOCTOU galore :)
  if(!suffix.startsWith("."))
    suffix = QString(".%1").arg(suffix);

  QUuid uuid = QUuid::createUuid();
  QString path = QDir::toNativeSeparators(
    QDir::tempPath() + "/" + qApp->applicationName().replace(" ", "") + "_" + uuid.toString(QUuid::WithoutBraces) + suffix);

  return QFileInfo(path);
}

QString Utils::humanFileSize(double num_bytes) {
  QStringList lst;
  lst << "KB" << "MB" << "GB" << "TB";

  QStringListIterator i(lst);
  QString unit("bytes");

  while(num_bytes >= 1024.0 && i.hasNext()) {
    unit = i.next();
    num_bytes /= 1024.0;
  }
  return QString::number(num_bytes, 'G', 3) + " " + unit;
}

std::chrono::time_point<std::chrono::high_resolution_clock> Utils::timeStart() {
  return std::chrono::high_resolution_clock::now();
}

void Utils::timeEnd(std::string label, std::chrono::time_point<std::chrono::high_resolution_clock> start) {
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsedSeconds = end - start;
  double delta = elapsedSeconds.count();
  //qDebug() << "TIME:" << label << delta << "second(s)";
  std::cout << label << " " << std::fixed << std::setprecision(6) << elapsedSeconds.count() << "s" << std::endl;
}
