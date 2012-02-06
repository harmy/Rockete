#include "OpenedFile.h"

#include <QFile>
#include <QString>
#include <QStringList>
#include <QMessageBox>

OpenedFile::OpenedFile()
{
    previousStartingIndex = -1;
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

    setTextEditContent(content);

    file.close();
}

QString OpenedFile::getLine(const int line_number)
{
    QStringList lines;

    lines = textEdit->toPlainText().split("\n");

    Q_ASSERT(line_number < lines.size());

    return lines[line_number];
}

void OpenedFile::find(const QString &str)
{
    QString plain_text;
    int starting_index;

    if (previousSearch != str || previousStartingIndex == -1)
    {
        previousSearch = str;
        previousStartingIndex = -1;
    }

    plain_text = textEdit->toPlainText();

    starting_index = plain_text.indexOf(previousSearch, previousStartingIndex == -1 ? 0 : previousStartingIndex);
    if (starting_index>=0)
    {
        textEdit->setFocus();
        QTextCursor newTextCursor = textEdit->textCursor();
        newTextCursor.setPosition(starting_index);
        newTextCursor.setPosition(starting_index+str.size(), QTextCursor::KeepAnchor);
        textEdit->setTextCursor(newTextCursor);
        previousStartingIndex = starting_index+str.size();
    }
    else if (starting_index != previousStartingIndex)
    {
        previousStartingIndex = starting_index+str.size();
        find(previousSearch);
    }
}

int OpenedFile::findLineNumber(const QString &str, const int start_line_number)
{
    QStringList lines;

    lines = textEdit->toPlainText().split("\n");

    Q_ASSERT(start_line_number < lines.size());

    for (int i=start_line_number; i<lines.size(); ++i) {
        if(lines[i].contains(str))
            return i;
    }

    return -1;
}

void OpenedFile::replaceLine(const int line_number, const QString &new_line)
{
    QStringList lines;

    lines = textEdit->toPlainText().split("\n");

    Q_ASSERT(line_number < lines.size());

    lines[line_number] = new_line;

    textEdit->clear();

    setTextEditContent(lines.join("\n"));

    // :TODO: Set cursor position on modified line.
    textEdit->textCursor().setPosition(1);
}

int OpenedFile::insertLineBeforeBracket(const int start_line, const QString &new_line)
{
    QStringList lines;
    int line_number;

    lines = textEdit->toPlainText().split("\n");

    Q_ASSERT(start_line < lines.size());

    for (line_number=start_line;line_number<lines.size();++line_number) {
        if(lines[line_number].indexOf('}') != -1)
            break;
    }

    lines.insert(line_number,new_line);

    textEdit->clear();

    setTextEditContent(lines.join("\n"));

    // :TODO: Set cursor position on modified line.
    textEdit->textCursor().setPosition(1);

    return line_number;
}

void OpenedFile::removeLine(const int line_number)
{
    QStringList lines;

    lines = textEdit->toPlainText().split("\n");

    Q_ASSERT(line_number < lines.size());

    lines.removeAt(line_number);

    setTextEditContent(lines.join("\n"));
}

void OpenedFile::save()
{
    QFile file(fileInfo.filePath());
    QString error_message;

    if (!textEdit->CheckXmlCorrectness(error_message))
    {
        QMessageBox msgBox;
        msgBox.setText("the document is not valid: " + error_message );
        msgBox.exec();
    }

    if (file.open(QFile::WriteOnly|QFile::Truncate)) {
        file.write(textEdit->toPlainText().toAscii().data());
        file.close();
        textDocument->setModified( false );
    }
}

void OpenedFile::saveAs(const QString &file_path)
{
    QFile file(file_path);
    QString error_message;

    if (!textEdit->CheckXmlCorrectness(error_message))
    {
        QMessageBox msgBox;
        msgBox.setText("the document is not valid: " + error_message );
        msgBox.exec();
    }

    if (file.open(QFile::WriteOnly|QFile::Truncate)) {
        file.write(textEdit->toPlainText().toAscii().data());
        file.close();
    }
}

void OpenedFile::setTextEditContent(const QString &content)
{
    textDocument->setPlainText(content);
    textEdit->setDocument(textDocument);
    textDocument->setModified( false );
}