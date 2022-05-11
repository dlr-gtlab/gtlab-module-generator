#ifndef DEPENDENCYSELECTIONWIDGET_H
#define DEPENDENCYSELECTIONWIDGET_H

#include <QWidget>


class QPushButton;
class QLineEdit;
class QCheckBox;
class QHBoxLayout;
class QRegularExpressionValidator;
struct DependencyStruct;
class DependencySelectionWidget : public QWidget
{
    Q_OBJECT

    static const QString S_DELETE_BTN_ICON_PATH;

    static const QString S_DELETE_BTN_TOOLTIP;
    static const QString S_CHECKBOX_TOOLTIP;
    static const QString S_VERSION_TOOLTIP;

public:

    DependencySelectionWidget(DependencyStruct const& dependency,
                              bool isEditable,
                              QWidget* parent = nullptr);

    DependencyStruct dependency() const;

    bool isChecked() const;

    bool isComplete() const;

private:

    QLineEdit* m_versionEdit{};
    /// check box to selet the dependency
    QCheckBox* m_isActivCheckBox{};

    QRegularExpressionValidator* m_versionValidator{};

private slots:

    /**
     * @brief onDeletePushBtnPressed -deletes the widget
     */
    void onDeleteBtnPressed();

    void onEditedVersion(QString content);

signals:

    void completeChanged();

};

#endif // DEPENDENCYSELECTIONWIDGET_H
