#ifndef INTERFACESELECTIONWIDGET_H
#define INTERFACESELECTIONWIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <module_generator_structs.h>

class QCheckBox;
class QLabel;
class QPushButton;
class QHBoxLayout;

class InterfaceSelectionWidget : public QWidget
{
    Q_OBJECT

public:

    explicit InterfaceSelectionWidget(ClassStruct const& interface,
                                      QWidget* parent = nullptr);

    ClassStruct const& interfaceStruct() const { return m_interfaceStruct; }

    bool isChecked() const;
    void setChecked(const bool value) const;

signals:

    void selectionChanged();

private:

    QLabel* m_warningLabel;
    QCheckBox* m_nameCheckBox;
    QHBoxLayout* m_baseLayout;

    ClassStruct m_interfaceStruct;
};

#endif // INTERFACESELECTIONWIDGET_H
