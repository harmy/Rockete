#include "XMLHighlighter.h"

XMLHighlighter::XMLHighlighter(QTextDocument *document)
: QSyntaxHighlighter(document)
{
    QTextCharFormat keywordFormat;
    QTextCharFormat xmlElementFormat;
    QTextCharFormat xmlAttributeFormat;
    QTextCharFormat singleLineCommentFormat;

    keywordFormat.setForeground(Qt::darkMagenta);
    keywordFormat.setFontWeight(QFont::Bold);

    highlightingRules.push_back(QPair<QRegExp,QTextCharFormat>(QRegExp("\\b?xml\\b"), keywordFormat));
    highlightingRules.push_back(QPair<QRegExp,QTextCharFormat>(QRegExp("/>"), keywordFormat));
    highlightingRules.push_back(QPair<QRegExp,QTextCharFormat>(QRegExp(">"), keywordFormat));
    highlightingRules.push_back(QPair<QRegExp,QTextCharFormat>(QRegExp("<"), keywordFormat));
    highlightingRules.push_back(QPair<QRegExp,QTextCharFormat>(QRegExp("</"), keywordFormat));

    xmlElementFormat.setFontWeight(QFont::Bold);
    xmlElementFormat.setForeground(Qt::darkGreen);

    highlightingRules.push_back(QPair<QRegExp,QTextCharFormat>(QRegExp("\\b[A-Za-z0-9_]+(?=[\\s/>])"), xmlElementFormat));

    xmlAttributeFormat.setFontItalic(true);
    xmlAttributeFormat.setForeground(Qt::blue);

    highlightingRules.push_back(QPair<QRegExp,QTextCharFormat>(QRegExp("\\b[A-Za-z0-9_]+(?=\\=)"), xmlAttributeFormat));

    valueFormat.setForeground(Qt::red);
    valueStartExpression = QRegExp("\"");
    valueEndExpression = QRegExp("\"(?=[\\s></])");

    singleLineCommentFormat.setForeground(Qt::gray);

    highlightingRules.push_back(QPair<QRegExp,QTextCharFormat>(QRegExp("<!--[^\n]*-->"), singleLineCommentFormat));
    highlightingRules.push_back(QPair<QRegExp,QTextCharFormat>(QRegExp("placeholder_rule_for_search_highlight"), keywordFormat));
}

void XMLHighlighter::setHighlightedString(const QString &str)
{
    QTextCharFormat keywordFormat;

    keywordFormat.setBackground(Qt::green);
    
    highlightingRules.removeLast();
    highlightingRules.push_back(QPair<QRegExp,QTextCharFormat>(QRegExp(str), keywordFormat));
}

void XMLHighlighter::highlightBlock(const QString &text)
{
    int index;
    int length;
    int startIndex;
    int endIndex;
    int commentLength;

    for(int i=0; i<highlightingRules.length();++i)
    {
        const QRegExp & expression = highlightingRules[i].first;
        const QTextCharFormat & format = highlightingRules[i].second;

        setCurrentBlockState(0);
        startIndex = 0;

        if(previousBlockState() != 1)
        {
            startIndex = valueStartExpression.indexIn(text);
        }

        while(startIndex >= 0)
        {
            endIndex = valueEndExpression.indexIn(text, startIndex);

            if(endIndex == -1)
            {
                setCurrentBlockState(1);
                commentLength = text.length() - startIndex;
            }
            else
            {
                commentLength = endIndex - startIndex + valueEndExpression.matchedLength();
            }

            setFormat(startIndex, commentLength, valueFormat);

            startIndex = valueStartExpression.indexIn(text, startIndex + commentLength);
        }

        index = expression.indexIn(text);

        while(index >= 0)
        {
            length = expression.matchedLength();
            setFormat(index, length, format);
            index = expression.indexIn(text, index + length);
        }
    }

}