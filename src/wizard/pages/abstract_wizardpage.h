#ifndef ABSTRACTWIZARDPAGE_H
#define ABSTRACTWIZARDPAGE_H

#include <QFont>
#include <QWizardPage>

class ModuleGeneratorSettings;

/**
 * @brief The AbstractWizardpage class - extension of QWizardPage, allows the
 * passing of a class, which may hold properties useful for all wizard pages
 */
class AbstractWizardPage : public QWizardPage
{
    Q_OBJECT

public:

    const static int I_INFOTEXTLABEL_HEIGHT = 30;
    const static int I_PAGES_COLUMN_WIDTH   = 130;

    /**
     * @brief inSettings- getter for global settings
     * @return
     */
    ModuleGeneratorSettings* settings() const { return m_settings; }

protected:

    /**
     * @brief AbstractWizardPage - abstract constructor
     * @param settings
     * @param parent
     */
    AbstractWizardPage(ModuleGeneratorSettings* settings, QWidget* parent = 0)
        : QWizardPage(parent), m_settings(settings) { }
    ~AbstractWizardPage() { }

private:

    /// pointer to global settings class
    ModuleGeneratorSettings* m_settings{};
};

#endif // GLOBALWIZARDPAGE_H

