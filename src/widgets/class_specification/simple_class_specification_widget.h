#ifndef SIMPLECLASSSPECIFICATIONWIDGET_H
#define SIMPLECLASSSPECIFICATIONWIDGET_H

#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>

#include "abstract_class_specification.h"
#include "module_generator_structs.h"

// BOOLEAN
class BoolClassSpecificationWidget : public QCheckBox,
        public AbstractClassSpecification
{
    Q_OBJECT

public:

    BoolClassSpecificationWidget(ImplementationStruct const& implementation,
                                 QWidget* parent = nullptr) :
        QCheckBox(parent),
        AbstractClassSpecification(implementation)
    {
        connect(this, SIGNAL(stateChanged(int)), this, SLOT(onStateChanged()));
        onStateChanged();
    }

    QStringList implementationValues() override
    {
        return {stateToString()};
    }

    QString stateToString()
    {
        return isChecked() ? QStringLiteral("true") : QStringLiteral("false");
    }

private slots:

    void onStateChanged()
    {
        setText(this->stateToString());
    }
};


// STRINGS
class StringClassSpecificationWidget : public QLineEdit,
        public AbstractClassSpecification
{
    Q_OBJECT

public:

    StringClassSpecificationWidget(ImplementationStruct const& implementation,
                                   QWidget* parent = nullptr) :
        QLineEdit(parent),
        AbstractClassSpecification(implementation) { }

    QStringList implementationValues() override
    {
        return {"QStringLiteral(\"" + text() + "\")"};
    }
};


// ICONS, DOCKWIDGETAREA
class ComboClassSpecificationWidget : public QComboBox,
        public AbstractClassSpecification
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
    ComboClassSpecificationWidget(ImplementationStruct const& implementation,
                                  QStringList const& values,
                                  QString const& prefix = {},
                                  QString const& suffix = {},
                                  QWidget* parent = nullptr) :
        QComboBox(parent),
        AbstractClassSpecification(implementation),
        m_prefix{prefix},
        m_suffix{suffix}
    {
        addItems(values);
    }

    QStringList implementationValues() override
    {
        QString retVal = currentText();

        retVal.prepend(m_prefix);
        retVal.append(m_suffix);

        return {retVal};
    }

private:

    const QString m_prefix{};
    const QString m_suffix{};
};

#endif // SIMPLECLASSSPECIFICATIONWIDGET_H
