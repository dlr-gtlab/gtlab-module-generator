#ifndef SINGLECLASSSPECIFICATIONWIDGET_H
#define SINGLECLASSSPECIFICATIONWIDGET_H

#include "abstract_class_specification_widget.h"

#include <QWidget>

class QHBoxLayout;
class QPushButton;
class QLineEdit;
class ClassSpecificationWidget;
class ModuleGeneratorSettings;
class SingleClassSpecificationWidget :
        public QWidget,
        public AbstractClassSpecification
{
    Q_OBJECT

public:

    SingleClassSpecificationWidget(FunctionStruct& function,
                                   ModuleGeneratorSettings* settings,
                                   bool deletable  = false,
                                   bool editable   = false,
                                   QWidget* parent = nullptr);
    ~SingleClassSpecificationWidget() override;

    QString linkedClassName() const;
    QStringList implementation();
    ClassStruct classImplementation();

    inline bool deleteable() const { return m_deletable; }
    inline bool editable()   const { return m_editable; }

protected:

    QStringList implementationValues() override;
    ClassStructs derivedClasses() override;

private:

    FunctionStruct& m_functionStruct;
    bool m_deletable;
    bool m_editable;

    QPushButton* m_specificationsButton;
    QPushButton* m_deleteButton;
    QLineEdit* m_nameLineEdit;
    QHBoxLayout* m_baseLayout;

    ClassSpecificationWidget* m_specificationsWidget;

private slots:

    void onDeleteBtnPressed();

    void onSpecificationsBtnPressed();

    void onHideSpecficationsWidget();

signals:

    void deleted(SingleClassSpecificationWidget*);

};

#endif // SINGLECLASSSPECIFICATIONWIDGET_H
