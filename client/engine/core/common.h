#pragma once

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <climits>
#include <cstdlib>
#include <cstdint>
#include <ctime>
#include <memory>

#include <QByteArray>
#include <QString>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFont>
#include <QMimeData>
#include <QMetaType>
#include <QTime>
#include <QTimer>
#include <QImage>
#include <QImageReader>
#include <QEvent>
#include <QKeyEvent>

#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFramebufferObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>

#include <QApplication>
#include <QSurfaceFormat>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QMainWindow>
#include <QPushButton>
#include <QTreeWidget>
#include <QSplitter>
#include <QSlider>
#include <QSpinBox>
#include <QLabel>
#include <QScrollArea>
#include <QGroupBox>
#include <QCheckBox>
#include <QLineEdit>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QCommonStyle>

#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkReply>
#include <QDesktopServices>

#include <engine/core/extmath.h>

namespace engine {
#define FILENAME (QFileInfo(__FILE__).fileName().toStdString().c_str())
#define FUNCNAME __FUNCTION__
#define LINENO __LINE__

#define dout ((qDebug().nospace() << FUNCNAME << "(" << FILENAME << ":" << LINENO << "):").space())
#define tab(n) (QByteArray((n) * 4, ' ').data())

#define NO_LOG 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_INFO 3

  extern int log_level;
}