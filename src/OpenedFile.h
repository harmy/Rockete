#ifndef OPENEDFILE_H
#define OPENEDFILE_H

#include "CodeEditor.h"
#include <QFileInfo>
#include <QTextDocument>
#include <QSyntaxHighlighter>

class OpenedFile
{
public:
    OpenedFile();
    virtual ~OpenedFile();
    virtual void initialize();
    void fillTextEdit();
    QString getLine(const int line_number);
    void find(const QString &str);
    int findLineNumber(const QString &str, const int start_line_number=0);
    void replaceLine(const int line_number, const QString &new_line);
    int insertLineBeforeBracket(const int start_line, const QString &new_line);
    void removeLine(const int line_number);
    void save();
    void saveAs(const QString &file_path);
    void setTextEditContent(const QString &content);
    virtual void highlightString(const QString &/*str*/){}
    virtual void rehighlight(){highlighter->rehighlight();}

    //int tabIndex;
    CodeEditor *textEdit;
    QTextDocument * textDocument;
    QFileInfo fileInfo;
    QString previousSearch;
    int previousStartingIndex;
    QSyntaxHighlighter *highlighter;
};

#endif
