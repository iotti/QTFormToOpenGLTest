#include "mainwidget.h"

#include <QMouseEvent>

#include <math.h>

const char *vshader = "attribute highp vec4 a_position;\n\
        attribute mediump vec2 a_texcoord;\n\
        uniform highp mat4 mvp_matrix;\n\
        varying mediump vec2 v_texcoord;\n\
        void main()\n\
        {\n\
            // Calculate vertex position in screen space\n\
            gl_Position = mvp_matrix * a_position;\n\
            // Pass texture coordinate to fragment shader\n\
            // Value will be automatically interpolated to fragments inside polygon faces\n\
            v_texcoord = a_texcoord;\n\
        }";
const char *fshader = "uniform sampler2D texture;\n\
        varying mediump vec2 v_texcoord;\n\
        void main()\n\
        {\n\
            // Set fragment color from texture\n\
            gl_FragColor = texture2D(texture, v_texcoord);\n\
        }\n\
";

MainWidget::MainWidget(QWidget *parent) :
    QGLWidget(parent),
    angularSpeed(0)
{
    qDebug()<<"INITIALIZED!!!!!";
}

MainWidget::~MainWidget()
{
    deleteTexture(texture);
}

void MainWidget::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
}

void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{
    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    angularSpeed += acc;
}

void MainWidget::timerEvent(QTimerEvent *)
{
    // Decrease angular speed (friction)
    angularSpeed *= 0.99;

    // Stop rotation when speed goes below threshold
    if (angularSpeed < 0.01) {
        angularSpeed = 0.0;
    } else {
        // Update rotation
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;

        // Update scene
        updateGL();
    }
}

void MainWidget::initializeGL()
{
    initializeGLFunctions();
    qglClearColor(Qt::black);
    initShaders();
    initTextures();

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    geometries.init();

    // Use QBasicTimer because its faster than QTimer
    timer.start(12, this);
}

void MainWidget::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceCode(QGLShader::Vertex, vshader))
    {
        qDebug()<<"FAILED TO LOAD SHADERS!!!!!";
        close();
    }

    // Compile fragment shader
    if (!program.addShaderFromSourceCode(QGLShader::Fragment, fshader))
    {
        qDebug()<<"FAILED TO LOAD SHADERS!!!!!";
        close();
    }

    // Link shader pipeline
    if (!program.link())
    {
        qDebug()<<"FAILED TO LOAD SHADERS!!!!!";
        close();
    }

    // Bind shader pipeline for use
    if (!program.bind())
    {
        qDebug()<<"FAILED TO LOAD SHADERS!!!!!";
        close();
    }
}

void MainWidget::initTextures()
{
    // Load cube.png image
    glEnable(GL_TEXTURE_2D);
    texture = bindTexture(QImage(":/cube.png"));

    // Set nearest filtering mode for texture minification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Set bilinear filtering mode for texture magnification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void MainWidget::resizeGL(int w, int h)
{
    // Set OpenGL viewport to cover whole widget
    glViewport(0, 0, w, h);

    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 3.0, zFar = 7.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}

void MainWidget::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, -5.0);
    matrix.rotate(rotation);

    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projection * matrix);

    // Use texture unit 0 which contains cube.png
    program.setUniformValue("texture", 0);

    // Draw cube geometry
    geometries.drawCubeGeometry(&program);
    qDebug()<<"PAINTED!";
}
