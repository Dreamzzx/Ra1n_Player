#include "mainwidget.h"
#include "qss.h"
#include "ffmsg.h"
#include <QDebug>
#include <qtoolbar.h>
#include <QFileDialog>
#include <functional>
#include "UpwardComboBox.h"

MainWidget::MainWidget(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    InitPlayer();
    InitQss();
}

MainWidget::~MainWidget()
{}

void MainWidget::InitPlayer()
{
    QWidget* em = new QWidget(this);
    ui.titleDockWidget->setTitleBarWidget(em);
    ui.titleDockWidget->setWidget(&m_title);

    QWidget* list = new QWidget(this);
    ui.playListDockWidget->setTitleBarWidget(list);
    ui.playListDockWidget->setWidget(&m_playlist);

    //Title
    connect(&m_title, &Title::MinBtnClick,  this, &MainWidget::showMinimized);
    connect(&m_title, &Title::FullBtnClick, this, &MainWidget::Show_Full_Normal);
    connect(&m_title, &Title::CloseBtnClick,this, &MainWidget::close);
    connect(&m_title, &Title::MenuClick,    &m_title,&Title::ShowMenuBar);
    connect(&m_title, &Title::OpenFileClick,this, &MainWidget::OpenFile);
    
    //CtrlBar
    connect(ui.ctrlBarWind, &CtrlBar::Play_Or_PauseBtnClick, this, &MainWidget::PlayOrPause);
    connect(ui.ctrlBarWind, &CtrlBar::EndBtnClick, this, &MainWidget::OnStop);
    connect(ui.ctrlBarWind, &CtrlBar::VoiceBtnClick, this, &MainWidget::VoiceMuted);
    connect(ui.ctrlBarWind, &CtrlBar::FastForwardBtnClick, this, &MainWidget::FastSeek);
    connect(ui.ctrlBarWind, &CtrlBar::BackOffdBtnClick, this, &MainWidget::BackSeek);
    connect(ui.ctrlBarWind, &CtrlBar::Play_Seek, this, &MainWidget::PlaySeek);
    connect(ui.ctrlBarWind, &CtrlBar::Volume_Change, this, &MainWidget::VolumeChange);
    connect(ui.ctrlBarWind, &CtrlBar::Speed_Change, this, &MainWidget::SpeedChange);
    this->resize(800, 600);
}

void MainWidget::InitQss()
{
    setWindowFlag(Qt::FramelessWindowHint);

    //title background color
    QPalette palette = ui.titleDockWidget->palette();
    palette.setColor(QPalette::Window, QColor(169,169,169));
    ui.titleDockWidget->setPalette(palette);
    ui.titleDockWidget->setAutoFillBackground(true);

    ui.showWind->setStyleSheet(QString::fromStdString(show_widget_qss));
}

void MainWidget::OnPlayOrPause()
{
    int ret = 0;

    //检测mp是否创建
    if (mp_)
    {
        OnStop();
    }
    if(!mp_)
    {
        mp_ = new Ra1nPlayer();
        ret = mp_->ra1nmp_create(std::bind(&MainWidget::message_loop, this, std::placeholders::_1));
        if (ret < 0)
        {
            delete mp_;
            mp_ = nullptr;
            return;
        }
        //设置视频回调
        mp_->SetVideofreshCallback(std::bind(&MainWidget::OutputVideo, this, std::placeholders::_1));
        //设置Ctrl回调
        mp_->SetCtrlCallBack(std::bind(&CtrlBar::SetTime, ui.ctrlBarWind, std::placeholders::_1, std::placeholders::_2));
    }
        //设置url
        mp_->ra1nmp_set_data_source(FilePath_->toStdString().c_str());
        //设置视频标题
        QFileInfo fileInfo(*FilePath_);
        m_title.SetName(fileInfo.baseName());
        //准备工作
        ret = mp_->ra1nmp_prepare_async();
        //设置SDL句柄
        ui.showWind->setWinID(ui.showWind->winId());

        if (ret < 0)
        {
            delete mp_;
            mp_ = nullptr; 
            return;
        }
}

void MainWidget::PlayOrPause()
{
    if (mp_->is_paused()) 
        mp_->ra1nmp_play();
    else
        mp_->ra1nmp_pause();
}

//中止播放
void MainWidget::OnStop()
{
    if (mp_)
    {
        mp_->ra1nmp_destroy();
        ui.showWind->destory();

        delete mp_;
        mp_ = nullptr;

        ui.ctrlBarWind->SetTime(MP_INIT_TIME, 0);
        m_title.InitName();
    }
}

void MainWidget::FastSeek()
{
    //mp_->rainmp_seek();
}

void MainWidget::BackSeek()
{

}

int MainWidget::message_loop(void*arg)
{
    Ra1nPlayer* mp = (Ra1nPlayer*)arg;

    //线程循环
    while (1)
    {
        AVMessage msg;
        //取消息队列的消息，如果没有消息就阻塞，直到有消息被发到信息队列
        int retval = mp->ra1nmp_get_msg(&msg, 1);

        if (retval < 0)
            break;
        switch (msg.what)
        {
        case RA1NP_MSG_OPEN_INPUT:
            qDebug() << "FFP_MSG_OPEN_INPUT";
                break;
        case RA1NP_MSG_PREPARED:
            qDebug() << "FFP_MSG_PREPARDE";
            break;
        case RA1NP_MSG_STOP:
            qDebug() << "FFP_MSG_STOP";
            break; 
        }
    }
    return 0;
}

int MainWidget::OutputVideo(const Frame* frame)
{
    return ui.showWind->Draw(frame);
}

void MainWidget::PlaySeek(double position)
{
    if(mp_)
    mp_->rainmp_seek(position);
}

void MainWidget::VolumeChange(float position)
{
    if(mp_)
    mp_->ra1nmp_set_volume(position);
}

void MainWidget::SpeedChange(const QString str)
{
    if (!mp_)
        return;

    bool flag = false;
    float speed = str.toFloat(&flag);
    if (flag)
    {
        mp_->ra1nmp_set_speed(speed);
    }
    else
    {
        printf("倍速转换失败");
    }
}

void MainWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        isleftpress_ = true;
        QPoint temp = event->globalPos();
        plast_ = temp;
        
    }
}

void MainWidget::mouseReleaseEvent(QMouseEvent* event)
{
    QApplication::restoreOverrideCursor(); //恢复鼠标的指针状态
    if (isleftpress_)
        isleftpress_ = false;
}

//鼠标移动
void MainWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (this->isFullScreen())
        return;

    if (isleftpress_)
    {
        QPoint ptemp = event->globalPos();
        ptemp = ptemp - plast_;
        ptemp = ptemp + pos();
        move(ptemp);
        plast_ = event->globalPos();
    }
}

void MainWidget::Title_Menu()
{
    
}

void MainWidget::OpenFile()
{
    FilePath_ = new QString();
    *FilePath_ =  QFileDialog::getOpenFileName(this, u8"打开文件", QDir::homePath(),
        "(*.mp4 *.avi *.flv)");
    if(FilePath_->size() != 0)
    emit OnPlayOrPause();
}

void MainWidget::VoiceMuted()
{
    mp_->ra1nmp_set_volum_muted();
}

void MainWidget::Show_Full_Normal()
{
    QPushButton* fullBtn = nullptr;
    if (isFullScreen())
    {
        QIcon icon(":/title/sources/title/maximize-2.svg");
        fullBtn = m_title.GetFullButton();
        fullBtn->setIcon(icon);
        this->showNormal();
    }
    else
    {
        QIcon icon(":/title/sources/title/minimize-3.svg");
        fullBtn =  m_title.GetFullButton();
        fullBtn->setIcon(icon);
        this->showFullScreen();
    }
}
