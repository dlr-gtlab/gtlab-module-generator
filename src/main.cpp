#include "module_wizard.h"

#include <QApplication>

int main(int argc, char *argv[])
{
#if QT_VERSION >= 0x050900
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication a{argc, argv};

    ModuleWizard wizard;

    wizard.exec();

    return 0;
}
