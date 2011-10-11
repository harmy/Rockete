#ifndef OPENEDFILE_H
#define OPENEDFILE_H

#include <QTextEdit>
#include <QFileInfo>
#include <QTextDocument>

class OpenedFile
{
public:
    OpenedFile();
    void fillTextEdit();
    QString getLine(const int line_number);
    int findLineNumber(const QString & str, const int start_line_number=0);
    void replaceLine(const int line_number, const QString & new_line);
    void save();

    int tabIndex;
    QTextEdit * textEdit;
    QTextDocument * textDocument;
    QFileInfo fileInfo;
};

#endif
