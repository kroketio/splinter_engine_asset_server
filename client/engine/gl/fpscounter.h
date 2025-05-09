#pragma once

#include <QObject>

#include "engine/core/common.h"

namespace engine {
  class FPSCounter: public QObject {
    Q_OBJECT

public:
    FPSCounter(QObject* parent = 0);

    public slots:
        void inc();

    signals:
        void fpsChanged(int fps);

  private:
    QTimer* timer;
    int fps;

    private slots:
        void timeout();
  };
}