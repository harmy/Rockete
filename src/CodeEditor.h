#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QObject>
#include <QKeyEvent>
#include <QCompleter>
#include <QTextBlockUserData>

class OpenedFile;

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(OpenedFile *parent_file);

    // this function is intended as basic "check before saving/commiting" that are meant to be extented upon, even rewritten completely if need be
    bool CheckXmlCorrectness(QString & error_message);

    struct BlockData : public QTextBlockUserData
    {
        QTextCharFormat format;
    };

public slots:
    void completeText(const QString &text);
    void completeTagText(const QString &text);
    void HighlightClosingTag();


protected:
    virtual void keyPressEvent(QKeyEvent * e);
    virtual void keyReleaseEvent(QKeyEvent * e);

    QCompleter *AutoCompleter;
    QCompleter *TagAutoCompleter;

private:
    QStringList tag_list;
    QStringList custom_list;
    QStringList keyword_list;
    QPoint PreviousHighlightedOpeningTag;
    QPoint PreviousHighlightedClosingTag;
    OpenedFile *parentFile;
};

#endif
