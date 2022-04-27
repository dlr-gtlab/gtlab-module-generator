#include "interface_specifications_page.h"

#include "module_generator_settings.h"
#include "module_generator_logger.h"
#include "function_specification_widget.h"

#include <QLabel>
#include <QGridLayout>
#include <QTabWidget>
#include <QMessageBox>
#include <QElapsedTimer>

#include <memory>

const char*
InterfaceSpecificationsPage::C_TITLE_INTERFACE_SPECIFICATIONS_PAGE =
        "Interface Specifications";

const QString
InterfaceSpecificationsPage::S_INFO_TEXT = QStringLiteral(
    "Most interfaces require additional data and classes to be registered. "
    "This process can be automated by filling out the required fields below. "
    "If not filled out correctly the module may not be able to compile and must "
    "be fixed manually. Changing the interface selection on the previous page "
    "may reset all changes.");

InterfaceSpecificationsPage::InterfaceSpecificationsPage(ModuleGeneratorSettings* settings,
                                                        QWidget* parent) :
    AbstractWizardPage(settings, parent)
{
    auto* infoLabel = new QLabel(S_INFO_TEXT);
    auto* baseLayout = new QGridLayout;

    m_interfaceTabBar = new QTabWidget();

    // page gui
    setTitle(tr(C_TITLE_INTERFACE_SPECIFICATIONS_PAGE));

    infoLabel->setWordWrap(true);
    infoLabel->setMinimumHeight(AbstractWizardPage::I_INFOTEXTLABEL_HEIGHT);
    infoLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    baseLayout->addWidget(infoLabel, 0, 0, 1, 3);
    baseLayout->addWidget(m_interfaceTabBar, 2, 0, 1, 3);
    baseLayout->setColumnMinimumWidth(0,
                                      AbstractWizardPage::I_PAGES_COLUMN_WIDTH);

    setLayout(baseLayout);
}

void
InterfaceSpecificationsPage::initializePage()
{
    QElapsedTimer timer;
    timer.start();

    LOG_INDENT("interface specifications page...");

    // only regenerate the widgets if the selection changed
    bool reload = false;
    auto& interfaces = settings()->selectedInterfaces();

    for (auto& interface : interfaces)
    {
        if (!m_activeInterfaces.contains(interface.className))
        {
            reload = true;
            break;
        }
    }

    if (reload || m_activeInterfaces.length() != interfaces.length())
    {
        LOG_INFO << "reloading all widgets!" << ENDL;
        intInterfaces();
    }

    LOG_INFO << "done! Took " << QString::number(timer.elapsed())
             << " ms";
}

bool
InterfaceSpecificationsPage::isComplete() const
{
    return QWizardPage::isComplete();
}

bool
InterfaceSpecificationsPage::validatePage()
{
    LOG_INDENT("validated!");

    setInterfaceImplementation();

    bool success = checkInterfaceData();

    LOG_INFO << "done!";

    return success;
}

void
InterfaceSpecificationsPage::intInterfaces()
{
    m_activeInterfaces.clear();
    clearInterfaceTabs();

    LOG_INDENT("setting up selected interfaces...");

    auto interfaces = settings()->selectedInterfaces();

    for (auto& interface : interfaces)
    {
        QString className(interface.className);

        LOG_INDENT(className);

        m_activeInterfaces << className;

        auto widget = std::make_unique<FunctionSpecificationWidget>(
                    interface.functions, settings());

        if (widget->isEmpty())
        {
            interface.functions = widget->implementedFunctions();

            LOG_WARN << "no widget to register!";
            continue;
        }

        m_interfaceTabBar->addTab(widget.release(), interface.objectName);

        LOG_INFO << "done!";
    }

    // update implementations for empty widgets
    settings()->setSelectedInterfaces(interfaces);

    // label to inform user that no further steps are required
    if (!m_interfaceTabBar->count())
    {
        LOG_INFO << "setting up info label..." << ENDL;

        auto* widget = new QLabel(QStringLiteral("No further steps required!"));

        widget->setAlignment(Qt::AlignCenter);

        m_interfaceTabBar->addTab(widget, QStringLiteral("Interfaces"));
    }

    LOG_INFO << "done!";
}

void
InterfaceSpecificationsPage::clearInterfaceTabs()
{
    LOG_INDENT("clearing interfaces...");

    for (int i = 0; i < m_interfaceTabBar->count(); i++)
    {
        auto* widget = m_interfaceTabBar->widget(i);

        if (!widget)
        {
            LOG_ERR << "null widget!";
            continue;
        }

        delete widget;
    }

    m_interfaceTabBar->clear();
}

void
InterfaceSpecificationsPage::setInterfaceImplementation()
{
    LOG_INDENT("setting interface implementation...");

    auto interfaces = settings()->selectedInterfaces();

    for (int i = 0; i < m_interfaceTabBar->count(); ++i)
    {
        auto* widget = qobject_cast<FunctionSpecificationWidget*>(
                    m_interfaceTabBar->widget(i));

        if (!widget)
        {
            LOG_ERR << "null widget!" << ENDL;
            continue;
        }

        for (auto& interface : interfaces)
        {
            if (interface.objectName == m_interfaceTabBar->tabText(i))
            {
                LOG_INFO << m_interfaceTabBar->tabText(i) << ENDL;
                interface.functions = widget->implementedFunctions();
                break;
            }
        }
    }

    settings()->setSelectedInterfaces(std::move(interfaces));

    LOG_INFO << "done!";
}

bool
InterfaceSpecificationsPage::checkClassImplementation(ClassStruct const& base,
                                                      QStringList& classNames)
{
    for (const auto& function : base.functions)
    {
        for (const auto& derived : function.implementation.derivedClasses)
        {
            if (derived.className.isEmpty() &&
                function.returnValue == QStringLiteral("QMetaObject"))
            {
                QMessageBox::critical(nullptr,
                    QStringLiteral("Empty class name"),
                    QStringLiteral("The function '") + function.name +
                    QStringLiteral("' has an empty class associated!\n"
                                   "Please create a valid class name."));

                return false;
            }

            classNames << derived.className;

            if (!checkClassImplementation(derived, classNames))
            {
                return false;
            }
        }
    }

    return true;
}

bool
InterfaceSpecificationsPage::checkDuplicateClassNames(QStringList const&
                                                      classNames)
{
    bool messageBoxDisplayed = false;

    for (QString className : classNames)
    {
        if (className.isEmpty())
        {
            // in case multiple classes are empty only report it once
            if (messageBoxDisplayed) continue;

            auto ans = QMessageBox::warning(nullptr,
                QStringLiteral("Empty class name"),
                QStringLiteral("A function implements an invalid class!\n"
                               "Invalid classes may not be generated."),
                QMessageBox::Ignore, QMessageBox::Cancel);
            messageBoxDisplayed = true;

            if (ans == QMessageBox::Cancel)
            {
                return false;
            }
        }
        else if (classNames.count(className) > 1)
        {
            QMessageBox::critical(nullptr,
                QStringLiteral("Duplicate class names"),
                QStringLiteral("The class name '") + className +
                QStringLiteral("' is defined multiple times!\n"
                               "Please choose a different class name."));

            return false;
        }
    }

    return true;
}

bool
InterfaceSpecificationsPage::checkInterfaceData()
{
    LOG_INDENT("checking interface data...");

    QStringList classNames{settings()->moduleClass().className};

    for (auto& interface : settings()->selectedInterfaces())
    {
//        if (interface == Q_NULLPTR) continue;

        if (!checkClassImplementation(interface, classNames))
        {
            LOG_WARN << "missing class implementation!";
            return false;
        }
    }

    if (!checkDuplicateClassNames(classNames))
    {
        LOG_WARN << "duplicate class!";
        return false;
    }

    LOG_INFO << "done!";

    return true;
}

