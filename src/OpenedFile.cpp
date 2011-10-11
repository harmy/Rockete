#include "OpenedFile.h"

#include <QFile>
#include <QString>
#include <QStringList>

OpenedFile::OpenedFile()
{
}

OpenedFile::~OpenedFile()
{
}

void OpenedFile::fillTextEdit()
{
    QFile file(fileInfo.filePath());
    QString content;

    file.open(QIODevice::ReadOnly);
    content = file.readAll().data();
    content.replace("\t", "    ");
    textDocument->setPlainText(content);

    textEdit->setDocument(textDocument);
    file.close();
}

QString OpenedFile::getLine(const int line_number)
{
    QStringList lines;

    lines = textEdit->toPlainText().split("\n");

    Q_ASSERT(line_number < lines.size());

    return lines[line_number];
}

int OpenedFile::findLineNumber(const QString & str, const int start_line_number)
{
    QStringList lines;

    lines = textEdit->toPlainText().split("\n");

    Q_ASSERT(start_line_number < lines.size());

    for(int i=start_line_number; i<lines.size(); ++i)
    {
        if(lines[i].contains(str))
        {
            return i;
        }
    }

    return -1;
}

void OpenedFile::replaceLine(const int line_number, const QString & new_line)
{
    QStringList lines;

    lines = textEdit->toPlainText().split("\n");

    Q_ASSERT(line_number < lines.size());

    lines[line_number] = new_line;

    textEdit->clear();

    textEdit->setPlainText(lines.join("\n"));

    // :TODO: Set cursor position on modified line.
    textEdit->textCursor().setPosition(1);
}

void OpenedFile::save()
{
    QFile file(fileInfo.filePath());

    if(file.open(QFile::WriteOnly|QFile::Truncate))
    {
        file.write(textEdit->toPlainText().toStdString().c_str());
        file.close();
    }
}