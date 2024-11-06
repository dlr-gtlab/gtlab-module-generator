/*
 * GTlab Module Generator
 *
 * SPDX-License-Identifier: MPL-2.0+
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 */

#ifndef CLASSSPECIFICATIONITEM_H
#define CLASSSPECIFICATIONITEM_H

#include "abstract_type_specification_item.h"

#include "class_specification_widget.h"

#include <QWidget>
#include <memory>

class QPushButton;
class ModuleGeneratorSettings;
class ClassSpecificationItem : public QWidget,
                               public AbstractTypeSpecificationItem
{
    Q_OBJECT

    static const QString S_DERIVED_CLASS_BTN_TEXT;
    static const QString S_LINKED_CLASS_BTN_TEXT;

    static const QString S_CLASS_BTN_TOOLTIP;

public:

    ClassSpecificationItem(FunctionData function,
                           TypeInfo typeInfo,
                           ModuleGeneratorSettings* settings,
                           QWidget* parent = nullptr);

    QStringList codeImplementation() override;
    QStringList additionalIncludes() override;
    ClassDataList derivedClasses() override;
    ClassDataList linkedClasses() override;

private:

    FunctionData m_functionData{};

    ModuleGeneratorSettings* m_settings{};
    QPushButton* m_derivedClassButton{};
    QPushButton* m_linkedClassButton{};

    std::unique_ptr<ClassSpecificationWidget> m_derivedClassWidget{};
    std::unique_ptr<ClassSpecificationWidget> m_linkedClassWidget{};

    void setupSpecificationsWidget(ClassSpecificationWidget& widget);

    void updateSpecificationButton(QPushButton& button,
                                   ClassData const& implClass,
                                   bool isLinkedBtn);

private slots:

    void onDerivedClassBtnPressed();
    void onHideDerivedClassWidget();

    void onLinkedClassBtnPressed();
    void onHideLinkedClassWidget();
};

#endif // CLASSSPECIFICATIONITEM_H
