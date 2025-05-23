#pragma once

#include <QObject>
#include <QOpenGLVersionFunctionsFactory>

#include "engine/core/mesh.h"
#include "engine/core/vertex.h"
#include "gl_material.h"

namespace engine {
  class OpenGLMesh: public QObject {
    Q_OBJECT

public:
    OpenGLMesh(Mesh* mesh, QObject* parent = 0);
    ~OpenGLMesh();

    Mesh* host() const;

    void create();
    void commit();
    void render(bool pickingPass = false);
    void destroy();

    void setSizeFixed(bool sizeFixed);
    void setPickingID(uint id);

    OpenGLMaterial *m_openGLMaterial;

  protected:
    void childEvent(QChildEvent *event) override;

  private:
    Mesh* m_host;
    bool m_sizeFixed;
    uint m_pickingID;

    QOpenGLVertexArrayObject * m_vao;
    QOpenGLBuffer * m_vbo, *m_ebo;
    QOpenGLFunctions_3_3_Core * glFuncs;

    static OpenGLUniformBufferObject *m_modelInfo;

    private slots:
      void materialChanged(const QSharedPointer<Material> &material);
      void geometryChanged(const QVector<Vertex>& vertices, const QVector<uint32_t>& indices);
      void hostDestroyed(QObject* host);
  };
}