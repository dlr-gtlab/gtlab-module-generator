/*
 * GTlab Module Generator
 *
 * SPDX-License-Identifier: MPL-2.0+
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 */

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

    int count() const;

    void insertWidget(int index, QWidget* widget);

    void appendWidget(QWidget* widget);

    void removeWidget(QWidget* widget);

    void clear();

    QLayout* layout();

    QWidgetList widgets() const;

private:

    QVBoxLayout* m_scrollLayout{};

signals:

    void widgetRemoved();

};

#endif // WIDGETLISTVIEW_H
