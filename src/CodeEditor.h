#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QObject>
#include <QKeyEvent>
#include <QCompleter>
#include <QTextBlockUserData>

class OpenedFile; 
class LineNumberArea;

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor();

    // this function is intended as basic "check before saving/commiting" that is meant to be extented upon, even rewritten completely if need be
    bool CheckXmlCorrectness(QString & error_message);
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    struct BlockData : public QTextBlockUserData
    {
        QTextCharFormat format;
    };

public slots:
    void completeText(const QString &text);
    void completeTagText(const QString &text);
    virtual void HighlightClosingTag();


protected:
    virtual void keyPressEvent(QKeyEvent * e);
    virtual void keyReleaseEvent(QKeyEvent * e);
    void resizeEvent(QResizeEvent *event);
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dragMoveEvent ( QDragMoveEvent * e );
    virtual void dropEvent(QDropEvent *event);

    QCompleter *AutoCompleter;
    QCompleter *TagAutoCompleter;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);

private:
    QStringList tag_list;
    QStringList custom_list;
    QStringList keyword_list;
    QPoint PreviousHighlightedOpeningTag;
    QPoint PreviousHighlightedClosingTag;
    QWidget *lineNumberArea;
};




class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;
};

#endif
