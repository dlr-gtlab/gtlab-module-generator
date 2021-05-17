#ifndef INTERFACESELECTIONWIDGET_H
#define INTERFACESELECTIONWIDGET_H

#include <QWidget>
#include <QCheckBox>

class QCheckBox;
class QLabel;
class QPushButton;
class QHBoxLayout;
struct ClassStruct;

class InterfaceSelectionWidget : public QWidget
{
    Q_OBJECT

public:

    explicit InterfaceSelectionWidget(ClassStruct& interface, QWidget* parent = nullptr);

    ClassStruct& interfaceStruct() const { return m_interfaceStruct; }

    bool isChecked() const;
    void setChecked(const bool value) const;

signals:

    void selectionChanged();

private:

    QLabel* m_warningLabel;
    QCheckBox* m_nameCheckBox;
    QHBoxLayout* m_baseLayout;

    ClassStruct& m_interfaceStruct;


};

#endif // INTERFACESELECTIONWIDGET_H
