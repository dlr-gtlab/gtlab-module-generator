#include "module_wizard.h"

#include "module_generator_logger.h"


#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#if QT_VERSION >= 0x050900
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    ModuleWizard wizard;

    wizard.exec();

    return 0;
}
