#include "CodeEditor.h"

#include <QScrollBar>
#include "Settings.h"
#include "Rockete.h"

// Public:

CodeEditor::CodeEditor() : QTextEdit()
{
}

// Protected:

void CodeEditor::keyPressEvent(QKeyEvent * e)
{
    if (e->key() == Qt::Key_Tab || e->key() == Qt::Key_Backtab) {

        bool shiftIsPressed = e->key() == Qt::Key_Backtab;

        textCursor().beginEditBlock();

        QTextCursor editingTextCursor = textCursor();
        QTextCursor endingTextCursor = textCursor();

        editingTextCursor.setPosition(textCursor().selectionStart());
        endingTextCursor.setPosition(textCursor().selectionEnd());
        endingTextCursor.movePosition( QTextCursor::Down );
        endingTextCursor.movePosition( QTextCursor::StartOfLine );

        do{
            editingTextCursor.movePosition( QTextCursor::StartOfLine );

            if (!shiftIsPressed) {
                for(int j=0; j<Settings::getTabSize(); ++j) {
                    editingTextCursor.insertText(" ");
                }
            }
            else {
                for(int j=0; j<Settings::getTabSize(); ++j) {
                    if (toPlainText()[editingTextCursor.position()] == ' ') {
                        editingTextCursor.deleteChar();
                    }
                }
            }
            editingTextCursor.movePosition( QTextCursor::Down );
            editingTextCursor.movePosition( QTextCursor::StartOfLine );

        } while(editingTextCursor.position() != endingTextCursor.position());

        textCursor().endEditBlock();
    }
    else if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
        QStringList lineList = toPlainText().split("\n");
        int currentPosition = 0;
        int lineStartIndex = 0;
        int nextLineStartIndex;
        int spaceCount;

        textCursor().beginEditBlock();

        // TODO: refactor using same kind of functions as the tabbing system
        currentPosition = textCursor().selectionStart();

        for(int i=0; i<lineList.size(); ++i){
            nextLineStartIndex = lineStartIndex + lineList[i].size() + 1;

            if ((lineStartIndex >= currentPosition || (currentPosition >= lineStartIndex && currentPosition < nextLineStartIndex))) {
                for( spaceCount = 0;lineList[i][spaceCount].isSpace(); spaceCount++ );
                textCursor().insertText("\n");
                for( ;spaceCount>0; spaceCount-- ){
                    textCursor().insertText(" ");
                }
                break;
            }
            lineStartIndex = nextLineStartIndex;
        }
        textCursor().endEditBlock();
    }
    else if (e->key() == Qt::Key_Left)
    {
        if(textCursor().hasSelection() && (e->modifiers() & Qt::ShiftModifier) == 0)
        {
            QTextCursor newCursor = textCursor();
            newCursor.setPosition(textCursor().selectionStart());
            setTextCursor(newCursor);
            document()->setModified(false);
        }
        else
        {
            QTextEdit::keyPressEvent(e);
        }
    }
    else if (e->key() == Qt::Key_Right)
    {
        if(textCursor().hasSelection() && (e->modifiers() & Qt::ShiftModifier) == 0)
        {
            QTextCursor newCursor = textCursor();
            newCursor.setPosition(textCursor().selectionEnd());
            setTextCursor(newCursor);
            document()->setModified(false);
        }
        else
        {
            QTextEdit::keyPressEvent(e);
        }
    }
    else
        QTextEdit::keyPressEvent(e);

    if (document()->isModified())
        Rockete::getInstance().codeTextChanged();
}

void CodeEditor::keyReleaseEvent(QKeyEvent * /*e*/)
{
}