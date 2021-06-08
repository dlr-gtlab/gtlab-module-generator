#include "interface_specifications_page.h"

#include "module_generator_settings.h"
#include "module_generator_logger.h"
#include "function_specification_widget.h"

#include <QLabel>
#include <QGridLayout>
#include <QTabWidget>
#include <QMessageBox>


const char* C_TITLE_INTERFACE_SPECIFICATIONS_PAGE = "Interface Specifications";

const QString S_INFO_TEXT = QStringLiteral(
    "Most interfaces require additional data and classes to be registered. "
    "This process can be automated by filling out the required fields below. "
    "If not filled out correctly the module may not be able to compile and must "
    "be fixed manually. Changing the interface selection on the previous page "
    "may reset all changes.");

InterfaceSpecificationsPage::InterfaceSpecificationsPage(ModuleGeneratorSettings* settings,
                                                        QWidget* parent) :
    AbstractWizardPage(settings, parent)
{
    m_infoTextLabel = new QLabel(S_INFO_TEXT);
    m_baseLayout = new QGridLayout;

    m_interfaceTabBar = new QTabWidget();

    // page gui
    setTitle(tr(C_TITLE_INTERFACE_SPECIFICATIONS_PAGE));

    m_infoTextLabel->setWordWrap(true);
    m_infoTextLabel->setMinimumHeight(AbstractWizardPage::I_INFOTEXTLABEL_HEIGHT);
    m_infoTextLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    m_baseLayout->addWidget(m_infoTextLabel, 0, 0, 1, 3);
    m_baseLayout->addWidget(m_interfaceTabBar, 2, 0, 1, 3);
    m_baseLayout->setColumnMinimumWidth(0, AbstractWizardPage::I_PAGES_COLUMN_WIDTH);

    setLayout(m_baseLayout);
}

void
InterfaceSpecificationsPage::initializePage()
{
    LOG_INSTANCE("interface specifications page...");

    // only regenerate the widgets if the selection changed
    bool reload = false;
    auto interfaces = settings()->selectedInterfaces();

    for (auto* interface : interfaces)
    {
        if (interface == Q_NULLPTR) continue;

        if (!m_activeInterfaces.contains(interface->className))
        {
            reload = true; break;
        }
    }

    if (reload || m_activeInterfaces.length() != interfaces.length())
    {
        LOG_INFO << "reloading all widgets!" << ENDL;
        setSelectedInterfaces();
    }

    LOG_INFO << "done!";
}

bool
InterfaceSpecificationsPage::isComplete() const
{
    return QWizardPage::isComplete();
}

bool
InterfaceSpecificationsPage::validatePage()
{
    LOG_INSTANCE("validated!");

    setInterfaceImplementation();

    bool success = checkInterfaceData();

    LOG_INFO << "done!";

    return success;
}

void
InterfaceSpecificationsPage::setSelectedInterfaces()
{
    m_activeInterfaces.clear();
    clearInterfaceTabs();

    LOG_INSTANCE("setting up selected interfaces...");

    for (auto* interface : settings()->selectedInterfaces())
    {
        if (interface == Q_NULLPTR)
        {
            LOG_ERR << "null interface!";
            continue;
        }

        QString className(interface->className);

        LOG_INSTANCE(className);

        m_activeInterfaces << className;

        auto* widget = new FunctionSpecificationWidget(interface->functions, settings());

        if (widget->isEmpty())
        {
            interface->functions = widget->implementedFunctions();

            LOG_WARN << "no widget to register!";
            delete widget;
            continue;
        }

        m_interfaceTabBar->addTab(widget, interface->objectName);

        LOG_INFO << "done!";
    }

    // label to inform user that no further steps are required
    if (m_interfaceTabBar->count() == 0)
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
    LOG_INSTANCE("clearing interfaces...");

    for (int i = 0; i < m_interfaceTabBar->count(); i++)
    {
        auto* widget = m_interfaceTabBar->widget(i);

        if (widget == Q_NULLPTR)
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
    LOG_INSTANCE("setting interface implementation...");

    auto interfaces = settings()->selectedInterfaces();

    for (int i = 0; i < m_interfaceTabBar->count(); ++i)
    {
        auto* widget = dynamic_cast<FunctionSpecificationWidget*>(m_interfaceTabBar->widget(i));

        if (widget == Q_NULLPTR)
        {
            LOG_ERR << "null widget!";
            continue;
        }

        for (int j = 0; j < interfaces.length(); ++j)
        {
            if (interfaces[j]->objectName == m_interfaceTabBar->tabText(i))
            {
                qDebug() << "here" << interfaces[j]->className << m_interfaceTabBar->tabText(i);
                interfaces[j]->functions = widget->implementedFunctions();

                break;
            }
        }
    }
}

bool
checkClassImplementation(const ClassStruct& base, QStringList& classNames)
{
    for (const auto& function : base.functions)
    {
        for (const auto& derived : function.implementation.derivedClasses)
        {
            classNames << derived.className;

            if (derived.className.isEmpty() &&
                function.returnValue == QStringLiteral("QMetaObject"))
            {
                QMessageBox::warning(0, QStringLiteral("Empty class name"),
                                     QStringLiteral("The function '") + function.name +
                                     QStringLiteral("' has an empty class associated!\n"
                                                    "Please create a valid class name."));

                return false;
            }

            if (!checkClassImplementation(derived, classNames))
            {
                return false;
            }
        }
    }

    return true;
}

bool
checkDuplicateClassNames(const QStringList& classNames)
{
    bool messageBoxDisplayed = false;

    for (QString className : classNames)
    {
        if (className.isEmpty())
        {
            // in case multiple classes are empty only report it once
            if (messageBoxDisplayed) continue;

            auto ans = QMessageBox::warning(0, QStringLiteral("Empty class name"),
                                            QStringLiteral("There is an empty class defined.\n"
                                                           "Empty classes cannot be generated!"),
                                            QMessageBox::Ignore, QMessageBox::Cancel);
            messageBoxDisplayed = true;

            if (ans == QMessageBox::Cancel)
            {
                return false;
            }
        }
        else if (classNames.count(className) > 1)
        {
            QMessageBox::warning(0, QStringLiteral("Duplicate class names"),
                                 QStringLiteral("The class name '") + className +
                                 QStringLiteral("' is defined multiple times!\n"
                                                "Please create a valid class name."));

            return false;
        }
    }

    return true;
}

bool
InterfaceSpecificationsPage::checkInterfaceData()
{
    LOG_INSTANCE("checking interface data...");

    QStringList classNames;

    classNames << settings()->moduleClass().className;

    for (auto* interface : settings()->selectedInterfaces())
    {
        if (interface == Q_NULLPTR) continue;

        if (!checkClassImplementation(*interface, classNames))
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









