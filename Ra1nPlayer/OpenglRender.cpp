#include "OpenglRender.h"
#include <QDebug>
OpenglRender::~OpenglRender()
{
	makeCurrent(); // 手动激活上下文
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteTextures(1, &yTexture);
	glDeleteTextures(1, &uTexture);
	glDeleteTextures(1, &vTexture);

	shaderProgram.release();

	doneCurrent();

}

void OpenglRender::setYUVData(AVFrame* frame)
{
	// 通过信号槽机制确保在主线程中执行OpenGL操作
	emit frameReady(frame);
}

void OpenglRender::onFrameReady(AVFrame* frame)
{
	// 这个方法在主线程中执行
	if (!frame) {
		return;
	}
	
	// 检查OpenGL上下文是否有效
	if (!context() || !context()->isValid()) {
		qDebug() << "OpenGL context is not valid";
		return;
	}
	
	makeCurrent();

	int width = frame->width;
	int height = frame->height;

	// 检查帧数据是否有效
	if (!frame->data[0] || !frame->data[1] || !frame->data[2]) {
		qDebug() << "Invalid frame data";
		doneCurrent();
		return;
	}
	
	// 上传Y数据
	glBindTexture(GL_TEXTURE_2D, yTexture);
	// 设置行对齐
	glPixelStorei(GL_UNPACK_ROW_LENGTH, frame->linesize[0] / sizeof(uint8_t));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, frame->data[0]);
	// 恢复默认行对齐
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

	// 上传UV数据
	int uvWidth = width / 2;
	int uvHeight = height / 2;
	
	// 上传U数据
	glBindTexture(GL_TEXTURE_2D, uTexture);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, frame->linesize[1] / sizeof(uint8_t));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, uvWidth, uvHeight, 0, GL_RED, GL_UNSIGNED_BYTE, frame->data[1]);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

	// 上传V数据
	glBindTexture(GL_TEXTURE_2D, vTexture);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, frame->linesize[2] / sizeof(uint8_t));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, uvWidth, uvHeight, 0, GL_RED, GL_UNSIGNED_BYTE, frame->data[2]);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

	update();

	doneCurrent();
}

void OpenglRender::initializeGL()
{
	initializeOpenGLFunctions();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// 初始化顶点数组
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	// 绑定顶点数组
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),vertices.data(),GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(),GL_STATIC_DRAW);

	//设置顶点属性指针 0和1 指向locationPos
    // x y 
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	// u v 坐标
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2*sizeof(float)));
	glEnableVertexAttribArray(1);

	// 解绑vao
	glBindVertexArray(0);

	// 初始化纹理
	glGenTextures(1,&yTexture);
	glGenTextures(1, &uTexture);
	glGenTextures(1, &vTexture);

	// 初始化绑定纹理
	uint8_t yData = 0;
	uint8_t uData = 128;
	uint8_t vData = 128;

	// 初始化Y纹理
	glBindTexture(GL_TEXTURE_2D, yTexture);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RED,1,1,0,GL_RED,GL_UNSIGNED_BYTE,&yData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  
	// 初始化U纹理
    glBindTexture(GL_TEXTURE_2D, uTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 1, 1, 0, GL_RED, GL_UNSIGNED_BYTE, &uData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // 初始化V纹理
    glBindTexture(GL_TEXTURE_2D, vTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 1, 1, 0, GL_RED, GL_UNSIGNED_BYTE, &vData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// 编译着色器
	if (!shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaderSource/source.vert")) {
		qDebug() << "Vertex shader compilation failed:" << shaderProgram->log();
	}
	if (!shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaderSource/source.frag")) {
		qDebug() << "Fragment shader compilation failed:" << shaderProgram->log();
	}
	if (!shaderProgram->link()) {
		qDebug() << "Shader program linking failed:" << shaderProgram->log();
	}

}

void OpenglRender::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);

	shaderProgram->bind();

	// 绑定纹理


	// 绑定纹理
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, yTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, uTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, vTexture);

	shaderProgram->setUniformValue("yTexture", 0);
	shaderProgram->setUniformValue("uTexture", 1);
	shaderProgram->setUniformValue("vTexture", 2);

	// 绘制三角形
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	shaderProgram->release();
}

void OpenglRender::resizeGL(int w, int h)
{
	printf("resizeGL\n");
}


