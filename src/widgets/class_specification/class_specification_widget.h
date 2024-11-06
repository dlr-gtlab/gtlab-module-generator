/*
 * GTlab Module Generator
 *
 * SPDX-License-Identifier: MPL-2.0+
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 */

#ifndef CLASSSPECIFICATIONWIDGET_H
#define CLASSSPECIFICATIONWIDGET_H

#include <QWidget>

#include "module_generator_structs.h"

class QGridLayout;
class QLineEdit;
class QCheckBox;
class FunctionSpecificationWidget;
class ModuleGeneratorSettings;
class QRegularExpressionValidator;
class ClassSpecificationWidget : public QWidget
{
    Q_OBJECT

    static const QString S_CLASS_NAME_LABEL;
    static const QString S_FILE_NAME_LABEL;
    static const QString S_OBJECT_NAME_LABEL;
    static const QString S_AUTO_COMPLETE_LABEL;

    static const QString S_AUTO_GENERATED_TOOLTIP;
    static const QString S_AUTO_GENERATED_ALT_TOOLTIP;

public:

    ClassSpecificationWidget(ClassData const& baseClass,
                             ModuleGeneratorSettings* settings,
                             QWidget* parent = nullptr);

    ClassData implementedClass();

private:

    ClassData m_implementedClass{};

    ModuleGeneratorSettings* m_settings{};

    QLineEdit* m_classNameEdit{};
    QLineEdit* m_objectNameEdit{};
    QLineEdit* m_fileNameEdit{};
    QCheckBox* m_autoEditCheckBox{};

    bool m_autoEditFileName{false};

    /// module name validator
    QRegularExpressionValidator* m_nameValidator{};
    QRegularExpressionValidator* m_classNameValidator{};
    QRegularExpressionValidator* m_fileNameValidator{};

    FunctionSpecificationWidget* m_functionWidget{};

    void autoComplete();

    void initFunctionWidget(QGridLayout& layout, int& row,
                            FunctionDataList const& functions);

protected:

    void closeEvent(QCloseEvent* event) override;

private slots:

    void onEditedObjectName(QString name);
    void onEditedClassName(QString name);
    void onEditedFileName();

    void onAutoCompleteChanged(int state);

signals:

    void hidden();
};

#endif // CLASSSPECIFICATIONWIDGET_H
