#ifndef MODULEWIZARD_H
#define MODULEWIZARD_H

#include <QWizard>
#include <memory>

#include "module_generator.h"

class ModuleWizard : public QWizard
{
    Q_OBJECT

    static const QString S_MODULE_GENERATOR_NAME;

    static const QSize S_SIZE_WIZARD;

public:

    explicit ModuleWizard(QWidget* parent = nullptr);

    /**
     * @brief page enum - enumerates wizard pages
     */
    enum PAGE {INTRO_PAGE = 0,
               SETTINGS_PAGE = 1,
               SPECIFICATIONS_PAGE = 2,
               INTERFACE_SELECTION_PAGE = 3,
               INTERFACE_SPECIFICATION_PAGE = 5,
               DEPENDENCY_SELECTION_PAGE = 6,
               SIGNATURE_PAGE = 7,
               SUMMARY_PAGE = 8};

    int nextId() const override;

private:

    std::unique_ptr<ModuleGenerator> m_generator{};
};

#endif // MODULEWIZARD_H
