#ifndef OPENEDFILE_H
#define OPENEDFILE_H

#include "CodeEditor.h"
#include <QFileInfo>
#include <QTextDocument>
#include <QSyntaxHighlighter>

class OpenedFile : public CodeEditor
{
    Q_OBJECT
public:
    OpenedFile();
    virtual ~OpenedFile();
    virtual void initialize();
    void fillTextEdit();
    QString getLine(const int line_number);
    void cursorFind(const QString &str);
    int findLineNumber(const QString &str, const int start_line_number=0);
    void replaceLine(const int line_number, const QString &new_line);
    int insertLineBeforeBracket(const int start_line, const QString &new_line);
    void removeLine(const int line_number);
    void save();
    void saveAs(const QString &file_path);
    void setTextEditContent(const QString &content, bool undo_friendly = false);
    virtual void highlightString(const QString &/*str*/){}
    virtual void rehighlight(){if(highlighter)highlighter->rehighlight();}

    QFileInfo fileInfo;

public slots:
    virtual void HighlightClosingTag(){CodeEditor::HighlightClosingTag(); rehighlight();};

protected:
    QSyntaxHighlighter *highlighter;

private:
    QString previousSearch;
    int previousStartingIndex;
};

#endif
