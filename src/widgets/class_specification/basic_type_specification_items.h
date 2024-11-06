/*
 * GTlab Module Generator
 *
 * SPDX-License-Identifier: MPL-2.0+
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 */

#ifndef BASICTYPESPECIFICATIONITEMS_H
#define BASICTYPESPECIFICATIONITEMS_H

#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>

#include "abstract_type_specification_item.h"
#include "module_generator_structs.h"

// BOOLEAN
class BoolSpecificationItem : public QCheckBox,
                              public AbstractTypeSpecificationItem
{
    Q_OBJECT

public:

    BoolSpecificationItem(ImplementationData implementation,
                          TypeInfo typeInfo,
                          QWidget* parent = nullptr);

    QStringList codeImplementation() override;

    QString stateToString();

private slots:

    void onStateChanged();
};


// STRINGS
class StringSpecificationItem : public QLineEdit,
                                public AbstractTypeSpecificationItem
{
    Q_OBJECT

public:

    StringSpecificationItem(ImplementationData implementation,
                            TypeInfo typeInfo,
                            QWidget* parent = nullptr);

    QStringList codeImplementation() override;
};


// DOCKWIDGET AREA
class ComboboxSpecificationItem : public QComboBox,
                                  public AbstractTypeSpecificationItem
{
    Q_OBJECT

public:

    ComboboxSpecificationItem(ImplementationData implementation,
                              TypeInfo tinfo,
                              QWidget* parent = nullptr);

    QStringList codeImplementation() override;
};

#endif // BASICTYPESPECIFICATIONITEMS_H
