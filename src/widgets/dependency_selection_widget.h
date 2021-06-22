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

public:

    DependencySelectionWidget(const DependencyStruct& dependency,
                              const bool isEditable,
                              QWidget* parent = nullptr);

    DependencyStruct dependency() const;

    bool isChecked() const;

    bool isComplete() const;

private:

    ///delete push button
    QPushButton*  m_deletePushBtn;

    QLineEdit* m_versionEdit;
    /// check box to selet the dependency
    QCheckBox* m_isActivCheckBox;
    /// horizontal layout
    QHBoxLayout* m_baseLayout;

    QRegularExpressionValidator* m_versionValidator;

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
