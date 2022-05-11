#ifndef TYPESPECIFICATIONWIDGET_H
#define TYPESPECIFICATIONWIDGET_H

#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>

#include "abstract_type_specifier.h"
#include "module_generator_structs.h"

// BOOLEAN
class BoolSpecificationWidget : public QCheckBox,
                                public AbstractTypeSpecifier
{
    Q_OBJECT

public:

    BoolSpecificationWidget(ImplementationData implementation,
                            QWidget* parent = nullptr);

    QStringList implementationCode() override;

    QString stateToString();

private slots:

    void onStateChanged();
};


// STRINGS
class StringSpecificationWidget : public QLineEdit,
                                  public AbstractTypeSpecifier
{
    Q_OBJECT

public:

    StringSpecificationWidget(ImplementationData implementation,
                              QWidget* parent = nullptr);

    QStringList implementationCode() override;
};


// ICONS, DOCKWIDGETAREA
class ComboboxSpecificationWidget : public QComboBox,
                                    public AbstractTypeSpecifier
{
    Q_OBJECT

public:

    /**
     * @brief ComboClassSpecificationWidget
     * @param implementation
     * @param values values for the combo box
     * @param prefix
     * @param suffix
     * @param parent
     */
    ComboboxSpecificationWidget(ImplementationData implementation,
                                QStringList const& values,
                                QString const& prefix = {},
                                QString const& suffix = {},
                                QWidget* parent = nullptr);

    QStringList implementationCode() override;

private:

    QString m_prefix{};
    QString m_suffix{};
};

#endif // TYPESPECIFICATIONWIDGET_H
