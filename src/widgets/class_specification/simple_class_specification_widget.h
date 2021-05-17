#ifndef SIMPLECLASSSPECIFICATIONWIDGET_H
#define SIMPLECLASSSPECIFICATIONWIDGET_H

#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>

#include "abstract_class_specification_widget.h"
#include "module_generator_structs.h"

class BoolClassSpecificationWidget : public QCheckBox,
        public AbstractClassSpecification
{
public:

    BoolClassSpecificationWidget(ImplementationStruct& implementation,
                                 QWidget* parent = nullptr);

protected:

    QStringList implementationValues() override
    {
        return QStringList(QString(isChecked() ? "true":"false"));
    }
};


class StringClassSpecificationWidget : public QLineEdit,
        public AbstractClassSpecification
{
public:

    StringClassSpecificationWidget(ImplementationStruct& implementation,
                                   QWidget* parent = nullptr);

protected:

    QStringList implementationValues() override
    {
        return QStringList('\"' + text() + '\"');
    }
};


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
    ComboClassSpecificationWidget(ImplementationStruct& implementation,
                                  const QStringList& values,
                                  const QString& prefix = "",
                                  const QString& suffix = "",
                                  QWidget* parent = nullptr);

protected:

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
