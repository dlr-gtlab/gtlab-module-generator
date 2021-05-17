#ifndef WIDGETLISTVIEW_H
#define WIDGETLISTVIEW_H

#include <QScrollArea>

class QScrollArea;
class QVBoxLayout;
class QSpacerItem;

class WidgetListView : public QScrollArea
{
    Q_OBJECT

public:

    explicit WidgetListView(QWidget* parent = nullptr);

    QWidget* at(int index) const;

    void insertWidget(int index, QWidget* widget);

    void removeWidget(QWidget* widget);

    void clearWidgets();

    QLayout* layout();

    QWidgetList widgets() const;

private:

    QVBoxLayout* m_scrollLayout;
    QWidget* m_scrollWidget;
    QSpacerItem* m_scrollLayoutSpacer;

signals:

    void widgetRemoved();

};

#endif // WIDGETLISTVIEW_H
