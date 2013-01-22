#ifndef SNIPPETSMANAGER_H
#define SNIPPETSMANAGER_H

#include <QListWidget>
#include <QXmlDefaultHandler>

enum CodeLanguage {
    Rml,
    Rcss,
    Count,
    None
};

struct CodeSnippet
{
    QString Title;
    QString ToolTip;
    QString Language;
    QString Code;
    QString FilePath;
};

class SnippetsManager : public QListWidget,
                        public QXmlDefaultHandler
{
public:
    SnippetsManager(QWidget *parent = 0);
    void initialize();

    virtual bool startDocument();
    virtual bool startElement(const QString &, const QString &localName, const QString &, const QXmlAttributes &atts);
    virtual bool endElement(const QString &, const QString &, const QString &);
    virtual bool characters(const QString &ch);
    virtual bool endDocument();
    QString addSnippet();
    QString removeSnippet();
    void filterSnippetsForLanguage(QString language);

public slots:

protected:

    virtual QStringList mimeTypes() const;
    virtual QMimeData *mimeData(const QList<QListWidgetItem *> items) const;

private slots:

private:
    QString currentTag;
    CodeSnippet *currentCodeSnippet;
    QString currentFilePath;
};

#endif
