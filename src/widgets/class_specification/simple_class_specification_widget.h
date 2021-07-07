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
public:

    BoolClassSpecificationWidget(const ImplementationStruct& implementation,
                                 QWidget* parent = nullptr) :
      QCheckBox(parent),
      AbstractClassSpecification(implementation) { }

    QStringList implementationValues() override
    {
        return QStringList(QString(isChecked() ? "true":"false"));
    }
};


// STRINGS
class StringClassSpecificationWidget : public QLineEdit,
        public AbstractClassSpecification
{
public:

    StringClassSpecificationWidget(const ImplementationStruct& implementation,
                                   QWidget* parent = nullptr) :
        QLineEdit(parent),
        AbstractClassSpecification(implementation) { }

    QStringList implementationValues() override
    {
        return QStringList("QStringLiteral(\"" + text() + "\")");
    }
};


// ICONS, DOCKWIDGETAREA
class ComboClassSpecificationWidget : public QComboBox,
        public AbstractClassSpecification
{
public:

    /**
     * @brief ComboClassSpecificationWidget
     * @param implementation
     * @param values values for the combo box
     * @param prefix
     * @param suffix
     * @param parent
     */
    ComboClassSpecificationWidget(const ImplementationStruct& implementation,
                                  const QStringList& values,
                                  const QString& prefix = "",
                                  const QString& suffix = "",
                                  QWidget* parent = nullptr) :
        QComboBox(parent),
        AbstractClassSpecification(implementation),
        m_prefix(prefix),
        m_suffix(suffix)
    {
        addItems(values);
    }

    QStringList implementationValues() override
    {
        QString retVal = currentText();

        retVal.prepend(m_prefix);
        retVal.append(m_suffix);

        return QStringList(retVal);
    }

private:

    const QString m_prefix;
    const QString m_suffix;
};

#endif // SIMPLECLASSSPECIFICATIONWIDGET_H
