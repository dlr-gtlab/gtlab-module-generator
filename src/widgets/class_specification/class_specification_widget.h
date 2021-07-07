#ifndef CLASSSPECIFICATIONSWIDGET_H
#define CLASSSPECIFICATIONSWIDGET_H

#include <QWidget>

#include "module_generator_structs.h"

class QGridLayout;
class QPushButton;
class QLineEdit;
class QLabel;
class QCheckBox;
class FunctionSpecificationWidget;
class ModuleGeneratorSettings;
class QRegularExpressionValidator;
class ClassSpecificationWidget : public QWidget
{
    Q_OBJECT

public:

    ClassSpecificationWidget(const FunctionStruct& function,
                             ModuleGeneratorSettings* settings,
                             bool isLinked = false,
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
    QCheckBox* m_autoEditCheckBox;

    /// module name validator
    QRegularExpressionValidator* m_nameValidator;

    FunctionSpecificationWidget* m_functionSpecificationWidget;

    QGridLayout* m_baseLayout;

    void autoComplete();

protected:

    void closeEvent(QCloseEvent* event) Q_DECL_OVERRIDE;

private slots:

    void onEditedObjectName(QString string);

    void onAutoCompleteChanged(int state);

signals:

    void hidden();
};

#endif // CLASSSPECIFICATIONSWIDGET_H
