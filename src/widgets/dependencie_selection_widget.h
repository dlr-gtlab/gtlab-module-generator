#ifndef DEPENDENCIESELECTIONWIDGET_H
#define DEPENDENCIESELECTIONWIDGET_H

#include <QWidget>


class QPushButton;
class QLineEdit;
class QCheckBox;
class QHBoxLayout;
class QRegularExpressionValidator;
struct DependencieStruct;


class DependencieSelectionWidget : public QWidget
{
    Q_OBJECT

public:

    DependencieSelectionWidget(QString& name, QWidget* parent = nullptr);

    QString name() const;

    QString version() const;

    bool isChecked() const;

    bool isComplete() const;


private:

    ///delete push button
    QPushButton*  m_deletePushBtn;

    QLineEdit* m_versionEdit;
    /// check box to selet the dependencie
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

#endif // DEPENDENCIESELECTIONWIDGET_H
