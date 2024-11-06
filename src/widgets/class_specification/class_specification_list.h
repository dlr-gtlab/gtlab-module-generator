/*
 * GTlab Module Generator
 *
 * SPDX-License-Identifier: MPL-2.0+
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 */

#ifndef CLASSSPECIFICATIONLIST_H
#define CLASSSPECIFICATIONLIST_H

#include "abstract_type_specification_item.h"

#include <QWidget>

class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class WidgetListView;
class ModuleGeneratorSettings;
class ClassSpecificationList : public QWidget,
                               public AbstractTypeSpecificationItem
{
    Q_OBJECT

    static const int I_LIST_HEIGTH;

    static const QString S_ADD_BTN;

public:

    ClassSpecificationList(FunctionData const& function,
                           TypeInfo typeInfo,
                           ModuleGeneratorSettings* settings,
                           QWidget* parent = nullptr);

    QStringList codeImplementation() override;
    QStringList additionalIncludes() override;
    ClassDataList derivedClasses() override;
    ClassDataList linkedClasses() override;

private:

    FunctionData m_functionData;
    ModuleGeneratorSettings* m_settings{};

    QPushButton* m_addButton{};
    WidgetListView* m_widgetListView{};

private slots:

    void onAddButtonPressed();
};

class DeletableSpecificationItem : public QWidget
{
    Q_OBJECT

    static const QString S_DELETE_BTN_ICON_PATH;

public:

    explicit DeletableSpecificationItem(QWidget* inputWidget,
                                        QWidget* parent = {});

    QWidget* widget() const { return m_inputWidget; }

    QWidget* m_inputWidget{};
};

#endif // CLASSSPECIFICATIONLIST_H
