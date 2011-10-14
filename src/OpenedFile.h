#ifndef OPENEDFILE_H
#define OPENEDFILE_H

#include <QTextEdit>
#include <QFileInfo>
#include <QTextDocument>

class OpenedFile
{
public:
    OpenedFile();
    virtual ~OpenedFile();
    void fillTextEdit();
    QString getLine(const int line_number);
    int findLineNumber(const QString &str, const int start_line_number=0);
    void replaceLine(const int line_number, const QString &new_line);
    int insertLineBeforeBracket(const int start_line, const QString &new_line);
    void removeLine(const int line_number);
    void save();
    void setTextEditContent(const QString &content);

    int tabIndex;
    QTextEdit * textEdit;
    QTextDocument * textDocument;
    QFileInfo fileInfo;
};

#endif
