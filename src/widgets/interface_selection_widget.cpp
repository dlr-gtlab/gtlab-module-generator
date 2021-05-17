#include "interface_selection_widget.h"

#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QPixmap>

#include "module_generator_structs.h"

//const int I_BTN_WIDTH = 40;

const QString S_BTN_TEXT = QStringLiteral("...");
const QString S_SETTINGS_TOOLTIP = QStringLiteral("Define implementations for functions");


InterfaceSelectionWidget::InterfaceSelectionWidget(ClassStruct& interface, QWidget* parent)
    : QWidget(parent), m_interfaceStruct(interface)
{
    m_nameCheckBox = new QCheckBox();
    m_warningLabel = new QLabel();
    m_baseLayout = new QHBoxLayout();

    m_nameCheckBox->setText(m_interfaceStruct.className);

    m_baseLayout->setSpacing(0);
    m_baseLayout->setContentsMargins(5, 2, 5, 2);

    m_baseLayout->addWidget(m_nameCheckBox);

    setLayout(m_baseLayout);
}

bool
InterfaceSelectionWidget::isChecked() const
{
    return m_nameCheckBox->isChecked();
}

void
InterfaceSelectionWidget::setChecked(const bool value) const
{
    m_nameCheckBox->setChecked(value);
}
