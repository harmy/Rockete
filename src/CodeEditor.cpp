#include "CodeEditor.h"

#include <QScrollBar>
#include "Settings.h"

// Public:

CodeEditor::CodeEditor() : QTextEdit()
{
}

// Protected:

void CodeEditor::keyPressEvent(QKeyEvent * e)
{
    if (e->key() == Qt::Key_Tab || e->key() == Qt::Key_Backtab) {
        int start;
        int end;
        bool shiftIsPressed = e->key() == Qt::Key_Backtab;
        int scrollPosition = verticalScrollBar()->sliderPosition();
        QStringList lineList = toPlainText().split("\n");
        int currentIndex = 0;
        int nextIndex;
        int futureSelectionStart = -1;
        int futureSelectionOffset = 0;

        start = textCursor().selectionStart();
        end = textCursor().selectionEnd();

        for(int i=0; i<lineList.size(); ++i) {
            nextIndex = currentIndex + lineList[i].size() + 1;

            if ((currentIndex >= start || (start >= currentIndex && start < nextIndex)) && currentIndex <= end) {
                if (!shiftIsPressed) {
                    for(int j=0; j<Settings::getTabSize(); ++j) {
                        lineList[i].insert(0, " ");
                        ++futureSelectionOffset;
                    }
                }
                else {
                    for(int j=0; j<Settings::getTabSize(); ++j) {
                        if (lineList[i][0] == ' ') {
                            lineList[i].remove(0, 1);
                            --futureSelectionOffset;
                        }
                    }
                }

                if (futureSelectionStart == -1)
                    futureSelectionStart = currentIndex;
            }
            currentIndex = nextIndex;
        }

        document()->setPlainText(lineList.join("\n"));
        verticalScrollBar()->setSliderPosition(scrollPosition);
        QTextCursor newTextCursor = textCursor();
        newTextCursor.setPosition(futureSelectionStart);
        newTextCursor.setPosition(end + futureSelectionOffset, QTextCursor::MoveAnchor);
        setTextCursor(newTextCursor);
    }
    else if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
        QStringList lineList = toPlainText().split("\n");
        int currentPosition = 0;
        int lineStartIndex = 0;
        int nextLineStartIndex;
        int spaceCount;

        currentPosition = textCursor().selectionStart();

        for(int i=0; i<lineList.size(); ++i)
        {
            nextLineStartIndex = lineStartIndex + lineList[i].size() + 1;

            if ((lineStartIndex >= currentPosition || (currentPosition >= lineStartIndex && currentPosition < nextLineStartIndex))) {
                for( spaceCount = 0;lineList[i][spaceCount].isSpace(); spaceCount++ );
                textCursor().insertText( "\n" );
                for( ;spaceCount>0; spaceCount-- ){
                    textCursor().insertText( " " );
                }
                break;
            }
            lineStartIndex = nextLineStartIndex;
        }
    }
    else
        QTextEdit::keyPressEvent(e);
}

void CodeEditor::keyReleaseEvent(QKeyEvent * /*e*/)
{
}