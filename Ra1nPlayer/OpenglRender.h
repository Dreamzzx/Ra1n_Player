#pragma once
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLFunctions_4_5_Core>
#include <vector>

extern "C" {
#include<libavformat/avformat.h>
}

class OpenglRender :public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
	Q_OBJECT
public:
	explicit OpenglRender(QWidget* parent = nullptr):QOpenGLWidget(parent){
		shaderProgram.reset(new QOpenGLShaderProgram(this));
	};
	~OpenglRender();

	void setYUVData(AVFrame* frame);
	
signals:
	void frameReady(AVFrame* frame);
	
public slots:
	void onFrameReady(AVFrame* frame);

protected:
	void initializeGL()override;

	void paintGL() override;
	void resizeGL(int w, int h)override;
private:
	
	const std::vector<float> vertices = {
		-1.0f,1.0f,0.0f,0.0f, // 左上
		-1.0f,-1.0f,0.0f,1.0f, // 左下
		1.0f,-1.0f,1.0f,1.0f, // 右下
		1.0f,1.0f,1.0f,0.0f//  右上
	};

	// 缓冲对象
	const std::vector<int> indices = { 0,1,2,3 };

	GLuint vao,vbo,ebo;

	GLuint yTexture, uTexture, vTexture;

	std::unique_ptr<QOpenGLShaderProgram> shaderProgram;
};

