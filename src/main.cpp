#include "module_wizard.h"

#include "module_generator_logger.h"


#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ModuleWizard wizard;

    wizard.exec();

    return 0;
}
