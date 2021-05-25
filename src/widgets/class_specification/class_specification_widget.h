#ifndef CLASSSPECIFICATIONSWIDGET_H
#define CLASSSPECIFICATIONSWIDGET_H

#include <QWidget>

#include "module_generator_structs.h"

class QGridLayout;
class QPushButton;
class QLineEdit;
class QLabel;
class FunctionSpecificationWidget;
class ModuleGeneratorSettings;
class QRegularExpressionValidator;
class ClassSpecificationWidget : public QWidget
{
    Q_OBJECT

public:

    ClassSpecificationWidget(const FunctionStruct& function,
                             ModuleGeneratorSettings* settings,
                             QWidget* parent = nullptr);

    ClassStruct implementedClass();

private:

    ModuleGeneratorSettings* m_settings;

    ClassStruct m_implementedClass;

    QLineEdit* m_classNameEdit;
    QLabel* m_classNameLabel;
    QLineEdit* m_objectNameEdit;
    QLabel* m_objectNamelabel;
    QLineEdit* m_fileNameEdit;
    QLabel* m_fileNameLabel;

    /// module name validator
    QRegularExpressionValidator* m_nameValidator;

    FunctionSpecificationWidget* m_functionSpecificationWidget;

    QGridLayout* m_baseLayout;


protected:

    void closeEvent(QCloseEvent* event) Q_DECL_OVERRIDE;

private slots:

    void onEditedObjectName(QString string);

signals:

    void hidden();
};

#endif // CLASSSPECIFICATIONSWIDGET_H
