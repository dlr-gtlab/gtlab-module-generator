#ifndef MODULEGENERATORLOGGER_H
#define MODULEGENERATORLOGGER_H

#include <QDebug>

class QString;
class ModuleGeneratorLogger
{
public:

    enum Type
    {
        Info = 0,
        Warning = 1,
        Error = 2
    };

    static void unregisterLoggingFunction();
    static void registerLoggingFunction(void (*)(QString&, Type, int));    
    static QString typeName(Type type);

    ModuleGeneratorLogger(const QString& text = "", const Type& type = Info);
    ~ModuleGeneratorLogger();

    ModuleGeneratorLogger& operator<<(const QString& text);

    inline void setType(const Type& type) { m_type = type; }

private:

    bool m_isEndl;
    Type m_type;
    QString m_bufferText;
};

#define LOG_INSTANCE ModuleGeneratorLogger __logger__ = ModuleGeneratorLogger
#define LOG_INFO __logger__
#define LOG_WARN __logger__.setType(ModuleGeneratorLogger::Type::Warning); __logger__
#define LOG_ERR __logger__.setType(ModuleGeneratorLogger::Type::Error); __logger__
#define ENDL QStringLiteral("\n")

#endif // MODULEGENLoggerEndlERATORLOGGER_H
