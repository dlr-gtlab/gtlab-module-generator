#include "interface_specifications_page.h"

#include "module_generator_settings.h"
#include "module_generator_logger.h"
#include "function_specification_widget.h"

#include <QLabel>
#include <QGridLayout>
#include <QTabWidget>
#include <QMessageBox>


const char* C_TITLE_INTERFACE_SPECIFICATIONS_PAGE = "Interface Specifications";

const QString S_INFO_TEXT =
        QStringLiteral("Most interfaces require additional data and classes to be registered. "
                       "This process can be automate by filling out the required fields below. "
                       "If not filled out fully the module may not be able to compile and must be fixed manually. "
                       "Reverting to the previous page will delete all changes.");

InterfaceSpecificationsPage::InterfaceSpecificationsPage(ModuleGeneratorSettings* settings,
                                                        QWidget* parent) :
    AbstractWizardPage(settings, parent)
{
    m_infoTextLabel = new QLabel;
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

    m_infoTextLabel->setText(S_INFO_TEXT);

    setSelectedInterfaces();

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

    return checkInterfaceData();
}

void
InterfaceSpecificationsPage::cleanupPage()
{
    clearInterfaceTabs();
}

void
InterfaceSpecificationsPage::setSelectedInterfaces()
{
    LOG_INSTANCE("setting up selected interfaces...");

    for (auto* interface : settings()->selectedInterfaces())
    {
        if (interface == Q_NULLPTR)
        {
            LOG_ERR << "null interface!";
            continue;
        }

        LOG_INSTANCE(interface->className);

        auto* widget = new FunctionSpecificationWidget(interface->functions, settings());

        if (widget->isEmpty())
        {
            LOG_WARN << "no widget to register!";
            delete widget;
            continue;
        }

        QString trimmedName = interface->className;

        trimmedName.replace("Gt", "");
        trimmedName.replace("Interface", "");

        m_interfaceTabBar->addTab(widget, trimmedName);
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

    for (int i = 0; i < m_interfaceTabBar->count(); ++i)
    {
        auto* widget = dynamic_cast<FunctionSpecificationWidget*>(m_interfaceTabBar->widget(i));

        if (widget == Q_NULLPTR)
        {
            LOG_ERR << "null widget!";
            continue;
        }

        widget->setImplementation();
    }
}

bool
checkClassImplementation(ClassStruct& classStruct, QStringList& classNames)
{
    for (auto* function : classStruct.functions)
    {
        if (function == Q_NULLPTR) continue;

        for (auto& derived : function->implementation.derivedClasses)
        {
            classNames << derived.className;

            if (derived.className.isEmpty() &&
                function->returnValue == QStringLiteral("QMetaObject"))
            {
                QMessageBox::warning(0, QStringLiteral("Empty class name"),
                                     QStringLiteral("The function '") + function->name +
                                     QStringLiteral("' has an empty class associated!\n"
                                                    "Please create a valid class name."));

                return false;
            }

            if (function->baseClass != Q_NULLPTR &&
                !checkClassImplementation(*function->baseClass, classNames))
            {
                return false;
            }
        }
    }

    return true;
}

bool
checkDuplicateClassNames(QStringList& classNames)
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
            return false;
        }
    }

    return checkDuplicateClassNames(classNames);
}









