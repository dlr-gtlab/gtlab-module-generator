/*
 * GTlab Module Generator
 *
 * SPDX-License-Identifier: MPL-2.0+
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 */

#include "function_specification_widget.h"

#include "widget_list_view.h"

#include "basic_type_specification_items.h"
#include "class_specification_item.h"
#include "class_specification_list.h"
#include "module_generator.h"
#include "module_generator_logger.h"
#include "module_generator_settings.h"

#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QSpacerItem>


FunctionSpecificationWidget::FunctionSpecificationWidget(
        FunctionDataList functions,
        ModuleGeneratorSettings* settings,
        QWidget* parent)
    : QScrollArea(parent),
      m_functions(std::move(functions)),
      m_settings(settings)
{
    auto* scrollWidget = new QWidget;
    m_baseLayout = new QGridLayout;

    setContent();

    scrollWidget->setLayout(m_baseLayout);

    setFrameShape(QFrame::NoFrame);

    setWidgetResizable(true);
    setWidget(scrollWidget);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

int
FunctionSpecificationWidget::count() const
{
    return m_specificationWidgets.count();
}

bool
FunctionSpecificationWidget::isEmpty() const
{
    return this->count() == 0;
}

FunctionDataList
FunctionSpecificationWidget::implementedFunctions()
{
    QMapIterator<QString, AbstractTypeSpecificationItem*> iterator{
        m_specificationWidgets
    };



    while (iterator.hasNext())
    {
        auto item = iterator.next();

        if (!item.value())
        {
            LOG_INDENT_ERR("Null specification");
            continue;
        }

        for (auto& f : m_functions)
        {
            if (f.name == item.key() &&
                f.version.isEnabled(m_settings->gtlabVersion()))
            {
                f.implementation = item.value()->functionImplementation();
                break;
            }
        }
    }

    return m_functions;
}

void
FunctionSpecificationWidget::setContent()
{
    LOG_INDENT("setting content...");

    for (auto& function : m_functions)
    {
        LOG_INDENT("setting specifcation widget for '" +
                   function.returnType + ' ' +
                   function.name + "'...");

        auto* widget = generateFunctionSpecification(m_settings, function,
                                                     function.inputType);
        if (!widget)
        {
            LOG_INFO << "skipping null specification!";
            continue;
        }
        auto* inputWidget = dynamic_cast<QWidget*>(widget);
        if (!inputWidget)
        {
            LOG_ERR << "Null input widget!";
            continue;
        }

        inputWidget->setToolTip(function.tooltip);

        // shorten return type:
        //  QMap<A, B>
        // to
        //  QMap<A,
        //       B>
        auto returnType = function.returnType;
        auto tIdx = returnType.indexOf('<');
        auto cIdx = returnType.indexOf(',');
        if (cIdx > tIdx && tIdx > 0)
        {
            returnType.replace(',', ",\n" + QString{' '}.repeated(tIdx));
        }

        auto* returnTypeLabel = new QLabel{returnType};
        auto* functionLabel   = new QLabel{function.name};

        QPalette palette;
        palette.setColor(QPalette::WindowText, Qt::darkBlue);

        returnTypeLabel->setMinimumHeight(20);
        returnTypeLabel->setFont(ModuleGeneratorSettings::F_MONO_FONT);
        returnTypeLabel->setPalette(palette);

        functionLabel->setFont(ModuleGeneratorSettings::F_MONO_FONT);
        functionLabel->setMinimumHeight(20);

        returnTypeLabel->setSizePolicy(QSizePolicy::Minimum,
                                       QSizePolicy::Minimum);
        functionLabel->setSizePolicy(QSizePolicy::Minimum,
                                     QSizePolicy::Minimum);

        int widgetCount = this->count();

        m_baseLayout->addWidget(functionLabel, widgetCount, 0, 1, 1,
                                Qt::AlignTop);
        m_baseLayout->addWidget(returnTypeLabel, widgetCount, 1, 1, 1,
                                Qt::AlignTop);
        m_baseLayout->addWidget(inputWidget, widgetCount, 2, 1, 1,
                                Qt::AlignTop);

        m_specificationWidgets.insert(function.name, widget);

        LOG_INFO << "done!";
    }

    auto* spacer = new QSpacerItem{1, 1, QSizePolicy::Minimum,
                                   QSizePolicy::MinimumExpanding};

    m_baseLayout->addItem(spacer, this->count() + 1, 0);

    LOG_INFO << "done!";
}
