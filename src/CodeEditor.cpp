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
        newTextCursor.setPosition(end + futureSelectionOffset, QTextCursor::KeepAnchor);
        setTextCursor(newTextCursor);
    }
    else
        QTextEdit::keyPressEvent(e);
}

void CodeEditor::keyReleaseEvent(QKeyEvent * /*e*/)
{
}