#ifndef CLASSSPECIFICATIONITEM_H
#define CLASSSPECIFICATIONITEM_H

#include "abstract_type_specifier.h"

#include "class_specification_widget.h"

#include <QWidget>
#include <memory>

class QPushButton;
class ModuleGeneratorSettings;
class ClassSpecificationItem : public QWidget,
                               public AbstractTypeSpecifier
{
    Q_OBJECT

    static const QString S_DERIVED_CLASS_BTN_TEXT;
    static const QString S_LINKED_CLASS_BTN_TEXT;

    static const QString S_CLASS_BTN_TOOLTIP;

    static const QString S_DELETE_BTN_ICON_PATH;

public:

    ClassSpecificationItem(FunctionData function,
                           ModuleGeneratorSettings* settings,
                           bool deletable  = false,
                           QWidget* parent = nullptr);

    bool deleteable() const { return m_deletable; }

    QStringList implementationCode() override;
    QStringList additionalIncludes() override;
    ClassDataList derivedClasses() override;
    ClassDataList linkedClasses() override;

private:

    FunctionData m_functionData{};
    bool m_deletable{false};

    ModuleGeneratorSettings* m_settings{};
    QPushButton* m_derivedClassButton{};
    QPushButton* m_linkedClassButton{};

    std::unique_ptr<ClassSpecificationWidget> m_derivedClassWidget{};
    std::unique_ptr<ClassSpecificationWidget> m_linkedClassWidget{};

    void setupSpecificationsWidget(ClassSpecificationWidget& widget);

    void updateSpecificationButton(QPushButton& button,
                                   ClassData const& implClass,
                                   bool isLinkedBtn);

private slots:

    void onDeleteBtnPressed();

    void onDerivedClassBtnPressed();
    void onHideDerivedClassWidget();

    void onLinkedClassBtnPressed();
    void onHideLinkedClassWidget();

signals:

    void deleted(ClassSpecificationItem*);

};

#endif // CLASSSPECIFICATIONITEM_H
