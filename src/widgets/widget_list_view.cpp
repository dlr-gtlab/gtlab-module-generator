#include "widget_list_view.h"

#include <QVBoxLayout>
#include <QSpacerItem>

WidgetListView::WidgetListView(QWidget* parent) : QScrollArea(parent)
{
    m_scrollWidget = new QWidget();
    m_scrollLayout = new QVBoxLayout();
    m_scrollLayoutSpacer = new QSpacerItem(1, 10,
                                QSizePolicy::Minimum,
                                QSizePolicy::Expanding);

    m_scrollLayout->setSpacing(0);
    m_scrollLayout->setContentsMargins(0, 0, 0, 0);
    m_scrollLayout->addItem(m_scrollLayoutSpacer);

    m_scrollWidget->setLayout(m_scrollLayout);

    setWidgetResizable(true);
    setWidget(m_scrollWidget);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

QWidgetList
WidgetListView::widgets() const
{
    QWidgetList list;

    for (int i = 0; i < m_scrollLayout->count(); ++i)
    {
        QWidget* widget = m_scrollLayout->itemAt(i)->widget();

        if (widget == Q_NULLPTR)
        {
            continue;
        }

        list << widget;
    }

    return list;
}


QWidget*
WidgetListView::at(int index) const
{
    return widgets().at(index);
}

void WidgetListView::insertWidget(int index, QWidget* widget)
{
    if (index == -1)
    {
        index = m_scrollLayout->count()-1;
    }

    m_scrollLayout->insertWidget(index, widget);
}

void
WidgetListView::removeWidget(QWidget* widget)
{
    m_scrollLayout->removeWidget(widget);

    emit widgetRemoved();
}

void
WidgetListView::clearWidgets()
{
    for (auto widget : widgets())
    {
        removeWidget(widget);
    }
}

QLayout*
WidgetListView::layout()
{
    return m_scrollLayout;
}
