#include "UpwardComboBox.h"
#include <QListView>
#include <QVBoxLayout>
#include <QAction>

UpwardComboBox::UpwardComboBox(QWidget* parent)
    : QComboBox(parent) {
    // �����Զ����QListView
    m_listView = new QListView(this);
    m_listView->addAction(new QAction("1.0", this));
    m_listView->setModel(this->model()); // ʹ�õ�ǰComboBox��ģ��
    m_listView->setWindowFlags(Qt::Popup); // ����Ϊ��������
    m_listView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void UpwardComboBox::showPopup() {
    // ��ȡ�������λ��
    QRect rect = this->rect();
    QPoint pos = this->mapToGlobal(QPoint(0, rect.bottom()));

    // ��ʾ�Զ�����б���ͼ
    m_listView->setGeometry(pos.x(), pos.y(), rect.width(), 200); // �趨������Ŀ�Ⱥ͸߶�
    m_listView->show(); // ��ʾ�б���ͼ
}

void UpwardComboBox::hidePopup() {
    m_listView->hide(); // �����б���ͼ
}
