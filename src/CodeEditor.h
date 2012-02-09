#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QTextEdit>
#include <QObject>
#include <QKeyEvent>
#include <QCompleter>

class CodeEditor : public QTextEdit
{
    Q_OBJECT

public:
    CodeEditor();

    // this function is intended as basic "check before saving/commiting" that are meant to be extented upon, even rewritten completely if need be
    bool CheckXmlCorrectness(QString & error_message);

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
};

#endif
