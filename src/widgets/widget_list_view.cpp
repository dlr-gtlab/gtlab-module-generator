#include "widget_list_view.h"

#include <QVBoxLayout>
#include <QSpacerItem>

WidgetListView::WidgetListView(QWidget* parent) : QScrollArea(parent)
{
    auto* scrollWidget = new QWidget;
    auto* m_scrollLayoutSpacer = new QSpacerItem(1, 10,
                                                 QSizePolicy::Minimum,
                                                 QSizePolicy::Expanding);

    m_scrollLayout = new QVBoxLayout;
    m_scrollLayout->setSpacing(0);
    m_scrollLayout->setContentsMargins(0, 0, 0, 0);
    m_scrollLayout->addItem(m_scrollLayoutSpacer);

    scrollWidget->setLayout(m_scrollLayout);

    setWidgetResizable(true);
    setWidget(scrollWidget);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

QWidgetList
WidgetListView::widgets() const
{
    QWidgetList list;

    for (int i = 0; i < m_scrollLayout->count(); ++i)
    {
        QWidget* widget = m_scrollLayout->itemAt(i)->widget();

        if (!widget)
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

int
WidgetListView::count() const
{
    return widgets().count();
}

void
WidgetListView::insertWidget(int index, QWidget* widget)
{
    if (index == -1)
    {
        index = m_scrollLayout->count() - 1;
    }

    m_scrollLayout->insertWidget(index, widget);
}

void
WidgetListView::appendWidget(QWidget* widget)
{
    insertWidget(-1, widget);
}

void
WidgetListView::removeWidget(QWidget* widget)
{
    m_scrollLayout->removeWidget(widget);
    delete widget;

    emit widgetRemoved();
}

void
WidgetListView::clear()
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
