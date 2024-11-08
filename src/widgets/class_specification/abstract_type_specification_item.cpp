/*
 * GTlab Module Generator
 *
 * SPDX-License-Identifier: MPL-2.0+
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 */

#include "abstract_type_specification_item.h"

#include "basic_type_specification_items.h"
#include "class_specification_item.h"
#include "class_specification_list.h"
#include "module_generator_settings.h"


const QString
AbstractTypeSpecificationItem::S_ID_VALUE =
        QStringLiteral("$$VALUE$$");
const QString
AbstractTypeSpecificationItem::S_ID_LINKED_VALUE =
        QStringLiteral("$$LINKED_VALUE$$");
const QString
AbstractTypeSpecificationItem::S_ID_VARIABLE_NAME =
        QStringLiteral("$$VARIABLE_NAME$$");
const QString
AbstractTypeSpecificationItem::S_ID_RETURN_TYPE =
        QStringLiteral("$$RETURN_TYPE$$");

AbstractTypeSpecificationItem::AbstractTypeSpecificationItem(
        ImplementationData implementation, TypeInfo typeInfo) :
    m_implementation(std::move(implementation)),
    m_typeInfo(std::move(typeInfo))
{

}

ImplementationData
AbstractTypeSpecificationItem::functionImplementation()
{
    m_implementation.lines = this->codeImplementation();
    m_implementation.includes += this->additionalIncludes();
    m_implementation.derivedClasses = this->derivedClasses();
    m_implementation.linkedClasses  = this->linkedClasses();

    // TODO: investigate: a lot of duplicates in includes
    m_implementation.includes.removeDuplicates();

    return m_implementation;
}

AbstractTypeSpecificationItem*
generateFunctionSpecification(ModuleGeneratorSettings* settings,
                              FunctionData& function,
                              QString const& inputType)
{
    auto typeInfo = settings->typeInfo(inputType);
    auto const& widgetType = typeInfo.widgetType;

    if (!typeInfo.isValid())
    {
        LOG_INDENT_WARN("No type info found for '" + inputType + "'!");
        function.implementation.lines = QStringList{"{}"};
        return {};
    }

    // simple return types
    if (widgetType.isEmpty() ||
        function.isHidden ||
        !function.version.isEnabled(settings->gtlabVersion()))
    {
        // use brace initalization by default or in case of list input
        // as we may not now the return type
        if (typeInfo.implementation.isEmpty() ||
            typeInfo.widgetType == S_WIDGET_TYPE_MULTI)
        {
            typeInfo.implementation = QStringList{"{}"};
        }
        function.implementation.lines = typeInfo.implementation;
        function.implementation.includes += typeInfo.includes;
        // no input widget required
        return {};
    }

    // function.widgetType should be handled by now
    if (typeInfo.widgetType == S_WIDGET_TYPE_MULTI)
    {
        auto innerType = typeInfo.innerType(inputType);
        auto innerTypeInfo = settings->typeInfo(innerType);

        if (!innerTypeInfo.isValid())
        {
            LOG_INDENT_WARN("No captured type info found for '" +
                            innerType + "/" + typeInfo.returnType + "'!");

            function.implementation.lines = QStringList{"{}"};
            return {};
        }

        return new ClassSpecificationList(function, typeInfo, settings);
    }

    // checkbox input
    if (widgetType == S_WIDGET_TYPE_BOOL)
    {
        return new BoolSpecificationItem(function.implementation, typeInfo);
    }

    // lineedit input
    if (widgetType == S_WIDGET_TYPE_STRING)
    {
        return new StringSpecificationItem(function.implementation, typeInfo);
    }

    // combobox input
    if (widgetType == S_WIDGET_TYPE_COMBOBOX)
    {
        return new ComboboxSpecificationItem(function.implementation, typeInfo);
    }

    // new class
    if (widgetType == S_WIDGET_TYPE_CLASS ||
        widgetType == S_WIDGET_TYPE_CLASSLINKED)
    {
        if (!function.baseClass.isValid())
        {
            LOG_INDENT_ERR("null base class!");
            return {};
        }
        return new ClassSpecificationItem(function, typeInfo, settings);
    }
    // existing class
    if (widgetType == S_WIDGET_TYPE_CLASSNAME)
    {
        return new ClassSpecificationItem(function, typeInfo, settings);
    }

    LOG_INDENT_ERR("No specification widget found for type info '" +
                   typeInfo.returnType + "/" + typeInfo.widgetType +
                   "'! Function input type: '" + widgetType + "'");

    function.implementation.lines = QStringList{"{}"};
    return {};
}
