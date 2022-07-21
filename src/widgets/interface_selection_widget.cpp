#include "interface_selection_widget.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QPixmap>

#include "module_generator_structs.h"

InterfaceSelectionWidget::InterfaceSelectionWidget(ClassData interface,
                                                   QWidget* parent)
    : QWidget(parent), m_interface(std::move(interface))
{
    auto* baseLayout = new QHBoxLayout;
    m_nameCheckBox = new QCheckBox;

    m_nameCheckBox->setText(m_interface.className);

    baseLayout->setSpacing(0);
    baseLayout->setContentsMargins(5, 2, 5, 2);

    baseLayout->addWidget(m_nameCheckBox);

    setToolTip(m_interface.tooltip);
    setLayout(baseLayout);
}

bool
InterfaceSelectionWidget::isChecked() const
{
    return m_nameCheckBox->isChecked();
}

void
InterfaceSelectionWidget::setChecked(bool value) const
{
    m_nameCheckBox->setChecked(value);
}
