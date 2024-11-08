/*
 * GTlab Module Generator
 *
 * SPDX-License-Identifier: MPL-2.0+
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 */

#include "module_wizard.h"

#include <QApplication>

int main(int argc, char *argv[])
{
#if QT_VERSION >= 0x050900
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    QApplication a{argc, argv};

    ModuleWizard wizard;

    wizard.exec();

    return 0;
}
