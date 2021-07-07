#ifndef SINGLECLASSSPECIFICATIONWIDGET_H
#define SINGLECLASSSPECIFICATIONWIDGET_H

#include "abstract_class_specification.h"

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

    SingleClassSpecificationWidget(const FunctionStruct& function,
                                   ModuleGeneratorSettings* settings,
                                   bool deletable  = false,
                                   bool editable   = false,
                                   QWidget* parent = nullptr);
    ~SingleClassSpecificationWidget() override;

    QString linkedClassName() const;

    inline bool deleteable() const { return m_deletable; }
    inline bool editable()   const { return m_editable; }

    QStringList implementationValues() override;
    ClassStructs derivedClasses() override;
    ClassStructs linkedClasses() override;

private:

    FunctionStruct m_functionStruct;
    bool m_deletable;
    bool m_editable;

    QPushButton* m_specificationsButton;
    QPushButton* m_linkedClassButton;
    QPushButton* m_deleteButton;
    QLineEdit* m_linkedNameLineEdit;
    QHBoxLayout* m_baseLayout;

    ClassSpecificationWidget* m_specificationsWidget;
    ClassSpecificationWidget* m_linkedSpecificationsWidget;

    void setupSpecificationsWidget(ClassSpecificationWidget* widget);

private slots:

    void onDeleteBtnPressed();

    void onSpecificationsBtnPressed();
    void onHideSpecficationsWidget();

    void onLinkedSpecificationsBtnPressed();
    void onHideLinkedSpecficationsWidget();

signals:

    void deleted(SingleClassSpecificationWidget*);

};

#endif // SINGLECLASSSPECIFICATIONWIDGET_H
