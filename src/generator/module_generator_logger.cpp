/*
 * GTlab Module Generator
 *
 * SPDX-License-Identifier: MPL-2.0+
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 */

#include "module_generator_logger.h"

#include <QDebug>

void consoleLog(QString const&, ModuleGeneratorLogger::Type, int);

static const int s_indentSize =  3;
static       int s_indent     = 0;

static ModuleGeneratorLogger::LogFunction s_loggingFunction(consoleLog);

void
ModuleGeneratorLogger::unregisterLoggingFunction()
{
    // reset indent
    s_indent = 0;
    // register default function
    s_loggingFunction = consoleLog;
}

void
ModuleGeneratorLogger::registerLoggingFunction(LogFunction function)
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
                                             const Type& type) :
    m_type(type)
{
    *this << text << ENDL;

    s_indent += s_indentSize;
}

ModuleGeneratorLogger::~ModuleGeneratorLogger()
{
    if (!m_isEndl)
    {
        m_bufferedText.replace("├", "└");
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
        m_bufferedText = QStringLiteral(" ").repeated(indent);

        for (int i = 0; i < indent; ++i)
        {
            if (i % s_indentSize == 0) m_bufferedText.replace(i, 1, "│");
        }

        if (s_indent > 0) m_bufferedText += "├ ";
    }

    m_bufferedText += text;

    // only print if line is finished
    if (text.contains(ENDL))
    {
        m_isEndl = true;

        s_loggingFunction(m_bufferedText, m_type, s_indent);

        // reset type to info
        if (m_type != Type::Info) m_type = Type::Info;

        m_bufferedText.clear();
    }

    return *this;
}

void
consoleLog(QString const& text, ModuleGeneratorLogger::Type type, int indent)
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
