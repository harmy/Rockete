#include "CodeEditor.h"

#include <QScrollBar>
#include "Settings.h"
#include "Rockete.h"

// Public:

CodeEditor::CodeEditor() : QTextEdit()
{
}

bool CodeEditor::CheckCssCorrectness(QString & error_message)
{
    QTextCursor parsingTextCursor = textCursor();
    int opened_brace_counter = 0;

    parsingTextCursor.setPosition(0);

    while(!parsingTextCursor.atEnd())
    {
        if(toPlainText()[parsingTextCursor.position()] == '{')
        {
            opened_brace_counter++;
        }
        else if(toPlainText()[parsingTextCursor.position()] == '}')
        {
            opened_brace_counter--;
        }

        parsingTextCursor.movePosition(QTextCursor::Right);
    }

    if(opened_brace_counter!=0)
    {
        error_message = ( opened_brace_counter < 0 ? "too many '}'" : "too many '{'" );
        error_message += " search for '{|}' to highlight all '{' and '}'";
    }

    return opened_brace_counter == 0;
}

bool CodeEditor::CheckXmlCorrectness(QString & error_message)
{
    QTextCursor parsingTextCursor = textCursor();
    int tag_delimiter_balance = 0;
    QStringList opened_tag_list;

    parsingTextCursor.setPosition(0);

    while(!parsingTextCursor.atEnd())
    {
        if(toPlainText()[parsingTextCursor.position()] == '<')
        {
            tag_delimiter_balance++;
        }
        else if(toPlainText()[parsingTextCursor.position()] == '>')
        {
            tag_delimiter_balance--;
        }

        if ( tag_delimiter_balance == 0 && parsingTextCursor.hasSelection() )
        {
            QString 
                tag_text = parsingTextCursor.selectedText().trimmed();
            
            tag_text.remove('<');
            
            if( !tag_text.contains('/') )
            {
                if ( tag_text.contains(' ') )
                {
                    int first_space = tag_text.indexOf(' ');
                    tag_text.chop(tag_text.count() - first_space);
                }

                opened_tag_list.append(tag_text);
            }
            else
            {
                if(tag_text.startsWith('/'))
                {
                    tag_text.remove('/');
                    if(!opened_tag_list.removeOne(tag_text))
                    {
                        error_message = tag_text + " is closed without being opened";
                        setTextCursor(parsingTextCursor);
                        return false;
                    }
                }
            }
        }

        parsingTextCursor.movePosition(QTextCursor::Right, tag_delimiter_balance > 0 ? QTextCursor::KeepAnchor : QTextCursor::MoveAnchor);
    }

    if(tag_delimiter_balance!=0)
    {
        error_message = ( tag_delimiter_balance < 0 ? "too many '>'" : "too many '<'" );
        error_message += " search for '<|>' to highlight all '<' and '>'";
    }
    else if(!opened_tag_list.isEmpty())
    {
        error_message = opened_tag_list.first() + " is not closed";
    }
    

    return tag_delimiter_balance == 0 && opened_tag_list.isEmpty();
}

// Protected:

void CodeEditor::keyPressEvent(QKeyEvent * e)
{
    if (e->key() == Qt::Key_Tab || e->key() == Qt::Key_Backtab) {
        textCursor().beginEditBlock();
        bool shiftIsPressed = e->key() == Qt::Key_Backtab;

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
            textCursor().beginEditBlock();
            QTextCursor newCursor = textCursor();
            newCursor.setPosition(textCursor().selectionStart());
            setTextCursor(newCursor);
            document()->setModified(false);
            textCursor().endEditBlock();
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
            textCursor().beginEditBlock();
            QTextCursor newCursor = textCursor();
            newCursor.setPosition(textCursor().selectionEnd());
            setTextCursor(newCursor);
            document()->setModified(false);
            textCursor().endEditBlock();
        }
        else
        {
            QTextEdit::keyPressEvent(e);
        }
    }
    else if (e->key() == Qt::Key_Delete)
    {
        if(textCursor().hasSelection())
        {
            QTextEdit::keyPressEvent(e);
        }
        else
        {
            if(toPlainText()[textCursor().position()] == '\n')
            {
                textCursor().beginEditBlock();
                textCursor().deleteChar();
                QTextCursor editingTextCursor = textCursor();
                do
                {
                    editingTextCursor.movePosition( QTextCursor::Right, QTextCursor::KeepAnchor );
                } while (toPlainText()[editingTextCursor.position()] == ' ');

                editingTextCursor.removeSelectedText();
                textCursor().endEditBlock();
            }
            else if(toPlainText()[textCursor().position()] == ' ')
            {
                QTextCursor editingTextCursor = textCursor();

                textCursor().beginEditBlock();

                for(int i = 0; i < Settings::getTabSize(); i++)
                {
                    if( toPlainText()[editingTextCursor.position()] == ' ' )
                    {
                        editingTextCursor.movePosition( QTextCursor::Right, QTextCursor::KeepAnchor );
                    }
                    else
                    {
                        break;
                    }
                }

                editingTextCursor.removeSelectedText();
                textCursor().endEditBlock();
            }
            else
            {
                QTextEdit::keyPressEvent(e);
            }
        }
    }
    else if (e->key() == Qt::Key_Backspace)
    {
        if(textCursor().position()>0 && toPlainText()[textCursor().position()-1] == ' ')
        {
            QTextCursor editingTextCursor = textCursor();

            textCursor().beginEditBlock();

            for(int i = 0; i < Settings::getTabSize(); i++)
            {
                if( textCursor().position()>0 && toPlainText()[editingTextCursor.position()-1] == ' ' )
                {
                    editingTextCursor.movePosition( QTextCursor::Left, QTextCursor::KeepAnchor );
                }
                else
                {
                    break;
                }
            }

            editingTextCursor.removeSelectedText();
            textCursor().endEditBlock();
        }
        else
        {
            QTextEdit::keyPressEvent(e);
        }
    }
    else if (e->key() == Qt::Key_Home) {

        textCursor().beginEditBlock();
        QTextCursor editingTextCursor = textCursor();
        editingTextCursor.movePosition( QTextCursor::StartOfLine, (e->modifiers() & Qt::ShiftModifier) == 0 ? QTextCursor::MoveAnchor : QTextCursor::KeepAnchor );

        while (toPlainText()[editingTextCursor.position()] == ' ') 
        {
            editingTextCursor.movePosition( QTextCursor::Right, (e->modifiers() & Qt::ShiftModifier) == 0 ? QTextCursor::MoveAnchor : QTextCursor::KeepAnchor );
        }

        setTextCursor(editingTextCursor);
        document()->setModified(false);
        textCursor().endEditBlock();
    }
    else
        QTextEdit::keyPressEvent(e);


    if (document()->isModified())
        Rockete::getInstance().codeTextChanged();

}

void CodeEditor::keyReleaseEvent(QKeyEvent * /*e*/)
{
}