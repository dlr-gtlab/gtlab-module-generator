/*
 * GTlab Module Generator
 *
 * SPDX-License-Identifier: MPL-2.0+
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 */

#ifndef MODULEGENERATORLOGGER_H
#define MODULEGENERATORLOGGER_H

#include <QString>

class ModuleGeneratorLogger
{
public:

    enum Type
    {
        Info = 0,
        Warning = 1,
        Error = 2
    };

    using LogFunction = void (*)(QString const&, Type, int);

    static void unregisterLoggingFunction();
    static void registerLoggingFunction(LogFunction function);
    static QString typeName(Type type);

    ModuleGeneratorLogger(QString const& text = {}, const Type& type = Info);
    ~ModuleGeneratorLogger();

    inline void setType(Type const& type) { m_type = type; }

    // operators
    ModuleGeneratorLogger& operator<<(const QString& text);

private:

    bool m_isEndl{true};
    Type m_type{Info};
    QString m_bufferedText{};
};

#define LOG_INDENT ModuleGeneratorLogger __logger__ = ModuleGeneratorLogger
#define LOG_INDENT_WARN(MSG) ModuleGeneratorLogger __logger__ = ModuleGeneratorLogger(MSG, ModuleGeneratorLogger::Type::Warning)
#define LOG_INDENT_ERR(MSG) ModuleGeneratorLogger __logger__ = ModuleGeneratorLogger(MSG, ModuleGeneratorLogger::Type::Error)
#define LOG_INFO __logger__
#define LOG_WARN __logger__.setType(ModuleGeneratorLogger::Type::Warning); __logger__
#define LOG_ERR __logger__.setType(ModuleGeneratorLogger::Type::Error); __logger__
#define ENDL QStringLiteral("\n")

#endif // MODULEGENLoggerEndlERATORLOGGER_H
