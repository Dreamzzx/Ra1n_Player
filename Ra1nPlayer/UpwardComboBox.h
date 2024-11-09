#ifndef _UPWARDCOMBOBOX_H
#define _UPWARDCOMBOBOX_H

#include <QComboBox>
#include <QListView>

class UpwardComboBox : public QComboBox {
    Q_OBJECT

public:
    explicit UpwardComboBox(QWidget* parent = nullptr);
    void showPopup() override; // 重载showPopup方法

protected:
    void hidePopup() override; // 可选：重载hidePopup方法以处理关闭

private:
    QListView* m_listView; // 自定义下拉列表视图
};

#endif // !_UPWARDCOMBOBOX_H