#include "module_generator_logger.h"

void defaultFunction(QString&, ModuleGeneratorLogger::Type, int);

static const int s_indentSize =  3;
static       int s_indent     = 0;

static void (*s_loggingFunction)(QString&, ModuleGeneratorLogger::Type, int)(defaultFunction);


void
ModuleGeneratorLogger::unregisterLoggingFunction()
{
    // reset indent
    s_indent = 0;
    // register default function
    s_loggingFunction = defaultFunction;
}

void
ModuleGeneratorLogger::registerLoggingFunction(void(*function)(QString&, Type, int))
{
    if (function != Q_NULLPTR)
    {
        // reset indent
        s_indent = 0;
        // register new function
        s_loggingFunction = function;
    }
}

QString
ModuleGeneratorLogger::typeName(ModuleGeneratorLogger::Type type)
{
    if (type == Type::Warning) return QStringLiteral("Warning");
    if (type == Type::Error) return QStringLiteral("Error");
    return QStringLiteral("Info");
}

ModuleGeneratorLogger::ModuleGeneratorLogger(const QString& text,
                                             const Type& type)
{
    m_type   = type;
    m_isEndl = true;

    *this << text << ENDL;

    s_indent += s_indentSize;
}

ModuleGeneratorLogger::~ModuleGeneratorLogger()
{
    if (!m_isEndl)
    {
        m_bufferText.replace("├", "└");
        *this << ENDL;
    }

    s_indent -= s_indentSize;
}

ModuleGeneratorLogger&
ModuleGeneratorLogger::operator<<(const QString& text)
{    
    // prepend new line with dir symbols
    if (m_isEndl)
    {
        int indent = s_indent - s_indentSize;
        m_isEndl = false;
        m_bufferText = QStringLiteral(" ").repeated(indent);

        for (int i = 0; i < indent; ++i)
        {
            if (i % s_indentSize == 0) m_bufferText.replace(i, 1, "│");
        }

        if (s_indent > 0) m_bufferText += "├ ";
    }

    m_bufferText += text;

    // only print if line is finished
    if (text.contains(ENDL))
    {
        m_isEndl = true;

        s_loggingFunction(m_bufferText, m_type, s_indent);

        // reset type to info
        if (m_type != Type::Info) m_type = Type::Info;

        m_bufferText.clear();
    }


    return *this;
}

void
defaultFunction(QString& text, ModuleGeneratorLogger::Type type, int indent)
{
    QString indendation = text.left(indent > 0 ? indent - 1:0);
    QString actualText  = text.mid(indent > 0 ? indent - 1:0);

    if (type != ModuleGeneratorLogger::Type::Info)
    {
        actualText = QString("[" + ModuleGeneratorLogger::typeName(type) +"] " + actualText);
    }

    actualText.remove(ENDL);
    qDebug().noquote() << indendation + actualText;
}
