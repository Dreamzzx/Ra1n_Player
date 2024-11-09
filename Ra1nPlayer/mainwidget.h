#pragma once

#include <QtWidgets/QMainWindow>
#include <QMouseEvent>
#include "ui_mainwidget.h"
#include "title.h"
#include "playlist.h"
#include "ra1nplayer.h"

class MainWidget : public QMainWindow
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();


private:
    void InitPlayer();
    void InitQss();

    void OnPlayOrPause();
    //视频暂停或继续播放

    void PlayOrPause();
    void OnStop();
    void FastSeek();
    void BackSeek();
    int message_loop(void*arg);
    int OutputVideo(const Frame* frame);
    void PlaySeek(double position);
    void VolumeChange(float position);
    void SpeedChange(const QString str);

    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent *event);



private slots:
    void Show_Full_Normal();
    void Title_Menu();
    void OpenFile();
    void VoiceMuted();
private:
    Ui::MainWidgetClass ui;

    Title m_title ;
    PlayList m_playlist;
    QString* FilePath_ = nullptr;

    Ra1nPlayer *mp_ = nullptr;

    bool isleftpress_ = false; //判断是否是鼠标左键按下
    QPoint plast_; //鼠标左键按下的鼠标位置
};
