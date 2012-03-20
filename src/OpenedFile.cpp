#include "OpenedFile.h"

#include <QFile>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QTextStream>
#include "Rockete.h"

OpenedFile::OpenedFile()
    : highlighter(NULL)
{
    previousStartingIndex = -1;
}

OpenedFile::~OpenedFile()
{
}

void OpenedFile::initialize()
{
    setLineWrapMode(QPlainTextEdit::NoWrap);
    setTabChangesFocus(false);

    document()->setDefaultFont(QFont("Courier",10));
    document()->setDocumentLayout(new QPlainTextDocumentLayout(document()));
}


void OpenedFile::fillTextEdit()
{
    QFile file(fileInfo.filePath());
    QString content;

    file.open(QIODevice::ReadOnly);

    if(fileInfo.filePath().endsWith(".rml"))
    {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        content = in.readAll();
    }
    else
    {
        content = file.readAll().data();
    }

    content.replace("\t", "    ");
    setTextEditContent(content);

    file.close();
}

QString OpenedFile::getLine(const int line_number)
{
    QStringList lines;

    lines = toPlainText().split("\n");

    Q_ASSERT(line_number < lines.size());

    return lines[line_number];
}

void OpenedFile::cursorFind(const QString &str, bool from_start)
{
    QString plain_text;
    int starting_index;

    if(str.isEmpty())
    {
        return;
    }

    if (previousSearch != str || previousStartingIndex == -1 || from_start)
    {
        previousSearch = str;
        previousStartingIndex = -1;
    }

    plain_text = toPlainText();

    starting_index = plain_text.indexOf(previousSearch, previousStartingIndex == -1 ? 0 : previousStartingIndex);
    if (starting_index>=0)
    {
        setFocus();
        QTextCursor newTextCursor = textCursor();
        newTextCursor.setPosition(starting_index);
        newTextCursor.setPosition(starting_index+str.size(), QTextCursor::KeepAnchor);
        setTextCursor(newTextCursor);
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
    QTextCursor parsingCursor = textCursor();
    
    parsingCursor.setPosition(0);
    
    while(parsingCursor.blockNumber() != start_line_number)
    {
        parsingCursor.movePosition(QTextCursor::Down);
    }

    while(!parsingCursor.atEnd())
    {
        if(parsingCursor.block().text().contains(str))
            return parsingCursor.blockNumber();

        parsingCursor.movePosition(QTextCursor::Down);
    }

    return -1;
}

void OpenedFile::replaceLine(const int line_number, const QString &new_line)
{
    QString old_text;
    QString new_text = new_line;
    QTextCursor parsingCursor = textCursor();
    int space_count = 0;

    parsingCursor.setPosition(0);

    while(parsingCursor.blockNumber() != line_number)
    {
        parsingCursor.movePosition(QTextCursor::Down);
    }

    parsingCursor.movePosition(QTextCursor::StartOfLine);
    parsingCursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);

    old_text = parsingCursor.selectedText();

    while(old_text.startsWith(" "))
    {
        old_text.remove(0,1);
        space_count++;
    }

    while(space_count>0)
    {
        new_text.prepend(" ");
        space_count--;
    }
    new_text.remove(".0000");
    parsingCursor.insertText(new_text);
    parsingCursor.clearSelection();

    setTextCursor(parsingCursor);
}

int OpenedFile::insertLineBeforeBracket(const int start_line, const QString &new_line)
{
    QTextCursor parsingCursor = textCursor();
    QString new_text = new_line;

    parsingCursor.setPosition(0);

    while(parsingCursor.blockNumber() != start_line)
    {
        parsingCursor.movePosition(QTextCursor::Down);
    }

    while(!parsingCursor.atEnd())
    {
        if(parsingCursor.block().text().contains('}'))
            break;

        parsingCursor.movePosition(QTextCursor::Down);
    }

    parsingCursor.movePosition(QTextCursor::StartOfLine);

    new_text.remove(".0000", Qt::CaseInsensitive);
    parsingCursor.insertText(new_text);
    parsingCursor.insertText("\n");
    parsingCursor.movePosition(QTextCursor::Up);
    setTextCursor(parsingCursor);
    return parsingCursor.blockNumber();

}

void OpenedFile::removeLine(const int line_number)
{
    printf("WARNING: OpenedFile::removeLine(const int line_number) never tested!!!!\n");

    QTextCursor parsingCursor = textCursor();

    parsingCursor.setPosition(0);

    while(parsingCursor.blockNumber() != line_number)
    {
        parsingCursor.movePosition(QTextCursor::Down);
    }

    parsingCursor.movePosition(QTextCursor::StartOfLine);
    parsingCursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);

    parsingCursor.deleteChar();
}

void OpenedFile::save()
{
    QFile file(fileInfo.filePath());
    QString error_message;
    Rockete::getInstance().getFileWatcher()->removePath(fileInfo.filePath());
    if (!CheckXmlCorrectness(error_message))
    {
        QMessageBox msgBox;
        msgBox.setText(fileInfo.fileName() + " is not valid: " + error_message );
        msgBox.exec();
    }

    if (file.open(QFile::WriteOnly|QFile::Truncate|QIODevice::Text)) {
        if(fileInfo.filePath().endsWith(".rml"))
        {
            QTextStream out(&file);
            out.setCodec("UTF-8");
            out.setGenerateByteOrderMark(true);
            out << toPlainText();
            out.flush();
        }
        else
        {
            file.write(toPlainText().toAscii().data());
        }
        file.close();
        document()->setModified( false );
    }

    Rockete::getInstance().getFileWatcher()->addPath(fileInfo.filePath());
}

void OpenedFile::saveAs(const QString &file_path)
{
    QFile file(file_path);
    QString error_message;

    if (!CheckXmlCorrectness(error_message))
    {
        QMessageBox msgBox;
        msgBox.setText("the document is not valid: " + error_message );
        msgBox.exec();
    }

    if (file.open(QFile::WriteOnly|QFile::Truncate)) {
        if(fileInfo.filePath().endsWith(".rml"))
        {
            QTextStream out(&file);
            out.setCodec("UTF-8");
            out.setGenerateByteOrderMark(true);
            out << toPlainText();
            out.flush();
        }
        else
        {
            file.write(toPlainText().toAscii().data());
        }
        file.close();
    }
}

void OpenedFile::setTextEditContent(const QString &content, bool undo_friendly)
{
    if(undo_friendly)
    {
        QTextCursor replacingCursor = textCursor();

        replacingCursor.setPosition(0);
        replacingCursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
        replacingCursor.insertText(content);
        replacingCursor.setPosition(0);
        setTextCursor(replacingCursor);
    }
    else
    {
        setPlainText(content);
    }
}