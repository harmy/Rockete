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

protected:
    virtual void keyPressEvent(QKeyEvent * e);
    virtual void keyReleaseEvent(QKeyEvent * e);


};

#endif
