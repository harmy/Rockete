#ifndef XMLHIGHLIGHTER_H
#define XMLHIGHLIGHTER_H

#include <QtGui/QSyntaxHighlighter>
#include <QList>
#include <QPair>
#include <QRegExp>

class XMLHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    XMLHighlighter(QTextDocument *document);


protected:
    void highlightBlock(const QString &text);

private:
    QList<QPair<QRegExp,QTextCharFormat>> highlightingRules;
    QTextCharFormat valueFormat;
    QRegExp valueStartExpression;
    QRegExp valueEndExpression;
};

#endif
