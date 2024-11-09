#ifndef _UPWARDCOMBOBOX_H
#define _UPWARDCOMBOBOX_H

#include <QComboBox>
#include <QListView>

class UpwardComboBox : public QComboBox {
    Q_OBJECT

public:
    explicit UpwardComboBox(QWidget* parent = nullptr);
    void showPopup() override; // ����showPopup����

protected:
    void hidePopup() override; // ��ѡ������hidePopup�����Դ���ر�

private:
    QListView* m_listView; // �Զ��������б���ͼ
};

#endif // !_UPWARDCOMBOBOX_H