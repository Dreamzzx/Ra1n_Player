#include "UpwardComboBox.h"
#include <QListView>
#include <QVBoxLayout>
#include <QAction>

UpwardComboBox::UpwardComboBox(QWidget* parent)
    : QComboBox(parent) {
    // 创建自定义的QListView
    m_listView = new QListView(this);
    m_listView->addAction(new QAction("1.0", this));
    m_listView->setModel(this->model()); // 使用当前ComboBox的模型
    m_listView->setWindowFlags(Qt::Popup); // 设置为弹出窗口
    m_listView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void UpwardComboBox::showPopup() {
    // 获取下拉框的位置
    QRect rect = this->rect();
    QPoint pos = this->mapToGlobal(QPoint(0, rect.bottom()));

    // 显示自定义的列表视图
    m_listView->setGeometry(pos.x(), pos.y(), rect.width(), 200); // 设定下拉框的宽度和高度
    m_listView->show(); // 显示列表视图
}

void UpwardComboBox::hidePopup() {
    m_listView->hide(); // 隐藏列表视图
}
