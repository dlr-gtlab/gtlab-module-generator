/*
 * GTlab Module Generator
 *
 * SPDX-License-Identifier: MPL-2.0+
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 */

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

    explicit InterfaceSelectionWidget(ClassData interface,
                                      QWidget* parent = nullptr);

    ClassData const& interface() const { return m_interface; }

    bool isChecked() const;

    void setChecked(bool value) const;

signals:

    void selectionChanged();

private:

    QCheckBox* m_nameCheckBox{};

    ClassData m_interface{};
};

#endif // INTERFACESELECTIONWIDGET_H
