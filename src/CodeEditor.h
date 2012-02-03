#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QTextEdit>
#include <QObject>
#include <QKeyEvent>

class CodeEditor : public QTextEdit
{
    Q_OBJECT

public:
    CodeEditor();

    // these two functions are intended as basic "check before saving/commiting" that are meant to be extented upon, even rewritten completely if need be
    bool CheckCssCorrectness(QString & error_message);
    bool CheckXmlCorrectness(QString & error_message);

protected:
    virtual void keyPressEvent(QKeyEvent * e);
    virtual void keyReleaseEvent(QKeyEvent * e);


};

#endif
