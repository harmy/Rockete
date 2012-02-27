#include "CodeEditor.h"

#include <QScrollBar>
#include <QStringListModel>
#include "Settings.h"
#include "Rockete.h"
#include "ProjectManager.h"

// Public:

CodeEditor::CodeEditor(OpenedFile *parent_file) : QPlainTextEdit()
{
    QFile 
        tags(ProjectManager::getInstance().getWordListPath() + "tag_list.txt"),
        customs(ProjectManager::getInstance().getWordListPath() + "custom_list.txt"),
        keywords(ProjectManager::getInstance().getWordListPath() + "keyword_list.txt");

    parentFile = parent_file;
    tags.open(QFile::ReadOnly);
    while (!tags.atEnd())
    {
        QByteArray line = tags.readLine();
        if (!line.isEmpty())
        {
            tag_list << line.trimmed();
        }
    }

    customs.open(QFile::ReadOnly);
    while (!customs.atEnd())
    {
        QByteArray line = customs.readLine();
        if (!line.isEmpty())
        {
            custom_list << line.trimmed();
        }
    }

    keywords.open(QFile::ReadOnly);
    while (!keywords.atEnd())
    {
        QByteArray line = keywords.readLine();
        if (!line.isEmpty())
        {
            keyword_list << line.trimmed();
        }
    }

    QStringList full_list;

    full_list = keyword_list + custom_list;
    full_list.sort();

    AutoCompleter = new QCompleter(full_list, this);
    AutoCompleter->setWidget(this);
    AutoCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    AutoCompleter->setModelSorting(QCompleter::CaseInsensitivelySortedModel);

    tag_list.sort();

    TagAutoCompleter = new QCompleter(tag_list, this);
    TagAutoCompleter->setWidget(this);
    TagAutoCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    TagAutoCompleter->setModelSorting(QCompleter::CaseInsensitivelySortedModel);

    QObject::connect(AutoCompleter, SIGNAL(activated(const QString &)), this, SLOT(completeText(const QString &)));
    QObject::connect(TagAutoCompleter, SIGNAL(activated(const QString &)), this, SLOT(completeTagText(const QString &)));

    QObject::connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(HighlightClosingTag()));

    PreviousHighlightedOpeningTag.setX(0);
    PreviousHighlightedOpeningTag.setY(0);
    PreviousHighlightedClosingTag.setX(0);
    PreviousHighlightedClosingTag.setY(0);
}

bool CodeEditor::CheckXmlCorrectness(QString & error_message)
{
    QTextCursor parsingTextCursor = textCursor();
    int tag_delimiter_balance = 0;
    int opened_brace_counter = 0;
    int opened_quote = 0;
    QStringList opened_tag_list;
    QString plain_text = toPlainText();

    parsingTextCursor.setPosition(0);

    while(!parsingTextCursor.atEnd())
    {
        if( opened_quote == 0 && plain_text[parsingTextCursor.position()] == '"' && plain_text[parsingTextCursor.position()-1] != '\\')
        {
            opened_quote++;
        }
        else if(opened_quote > 0 && plain_text[parsingTextCursor.position()] == '"' && plain_text[parsingTextCursor.position()-1] != '\\')
        {
            opened_quote--;
        }

        if (opened_quote == 0)
        {
            if(plain_text[parsingTextCursor.position()] == '}')
            {
                opened_brace_counter--;
            }
            else if(plain_text[parsingTextCursor.position()] == '>')
            {
                tag_delimiter_balance--;
            }
            else if(plain_text[parsingTextCursor.position()] == '{')
            {
                opened_brace_counter++;
            }
            else if(plain_text[parsingTextCursor.position()] == '<')
            {
                if (opened_brace_counter > 0 && plain_text[parsingTextCursor.position() + 1] != '!')
                {
                    error_message = "xml tag detected inside css class";
                    setTextCursor(parsingTextCursor);
                    return false;
                }
                tag_delimiter_balance++;
            }
        }

        if(opened_brace_counter > 1 || tag_delimiter_balance > 1)
        {
            error_message = "found two opening character('<' or '{') in a row";
            parsingTextCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
            setTextCursor(parsingTextCursor);
            return false;
        }

        if(opened_brace_counter < 0|| tag_delimiter_balance < 0)
        {
            error_message = "closing character ('>' or '}') before any opening one";
            parsingTextCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
            setTextCursor(parsingTextCursor);
            return false;
        }

        if ( tag_delimiter_balance == 0 && parsingTextCursor.hasSelection() )
        {
            QString 
                tag_text = parsingTextCursor.selectedText().trimmed();

            tag_text.remove('<');

            if(!tag_text.contains("--"))
            {
                if( !tag_text.contains('/')  )
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
                    else
                    {
                         tag_text.remove('>');
                         tag_text = tag_text.trimmed();
                         if(!tag_text.endsWith('/')) // opening tag with source file behind a directory
                         {
                             if ( tag_text.contains(' ') )
                             {
                                 int first_space = tag_text.indexOf(' ');
                                 tag_text.chop(tag_text.count() - first_space);
                             }

                             opened_tag_list.append(tag_text);
                         }
                    }
                }
            }
        }

        // TODO: check l'ordre de fermeture

        parsingTextCursor.movePosition(QTextCursor::Right, tag_delimiter_balance > 0 ? QTextCursor::KeepAnchor : QTextCursor::MoveAnchor);
    }

    opened_tag_list.removeAll( "!--" );

    if(!opened_tag_list.isEmpty())
    {
        error_message = opened_tag_list.first() + " is not closed";
    }

    return opened_tag_list.isEmpty();
}

// public slots:

void CodeEditor::completeText(const QString &text)
{
    textCursor().beginEditBlock();
    QTextCursor editingTextCursor = textCursor();

    editingTextCursor.setPosition(textCursor().selectionStart());
    editingTextCursor.movePosition( QTextCursor::EndOfWord, QTextCursor::MoveAnchor );
    editingTextCursor.movePosition( QTextCursor::StartOfWord, QTextCursor::KeepAnchor );
    editingTextCursor.removeSelectedText();

    editingTextCursor.insertText(text);
    setTextCursor(editingTextCursor);

    textCursor().endEditBlock();
}

void CodeEditor::completeTagText(const QString &text)
{
    textCursor().beginEditBlock();
    QString adding_text;
    QTextCursor editingTextCursor = textCursor();

    editingTextCursor.setPosition(textCursor().selectionStart());
    editingTextCursor.movePosition( QTextCursor::EndOfWord, QTextCursor::MoveAnchor );
    editingTextCursor.movePosition( QTextCursor::StartOfWord, QTextCursor::KeepAnchor );
    editingTextCursor.removeSelectedText();
    adding_text = text;

    adding_text += ">";
    adding_text += "</";
    adding_text += text;
    adding_text += ">";

    editingTextCursor.insertText(adding_text);
    editingTextCursor.movePosition( QTextCursor::PreviousWord, QTextCursor::MoveAnchor );
    editingTextCursor.movePosition( QTextCursor::PreviousWord, QTextCursor::MoveAnchor );
    editingTextCursor.movePosition( QTextCursor::PreviousWord, QTextCursor::MoveAnchor );
    editingTextCursor.movePosition( QTextCursor::Right, QTextCursor::MoveAnchor );
    setTextCursor(editingTextCursor);

    textCursor().endEditBlock();
}

void CodeEditor::HighlightClosingTag()
{
    if(textCursor().hasSelection())
        return;

    QTextCursor parsingTextCursor = textCursor();
    QTextCursor unHighlightingTextCursor = textCursor();
    QString plain_text = toPlainText();
    QString opening_tag;
    QStringList opened_tag_list;
    int opened_quote = 0,
        tag_delimiter_balance = 0,
        start_parse_position = 0;
    BlockData
        * data;

    if(!PreviousHighlightedOpeningTag.isNull())
    {
        unHighlightingTextCursor.setPosition(PreviousHighlightedOpeningTag.x());
        unHighlightingTextCursor.setPosition(PreviousHighlightedOpeningTag.y(), QTextCursor::KeepAnchor);
        unHighlightingTextCursor.block().setUserData(NULL);
        //unHighlightingTextCursor.mergeCharFormat(format);
        PreviousHighlightedOpeningTag.setX(0);
        PreviousHighlightedOpeningTag.setY(0);
    }

    if(!PreviousHighlightedClosingTag.isNull())
    {
        unHighlightingTextCursor.setPosition(PreviousHighlightedClosingTag.x());
        unHighlightingTextCursor.setPosition(PreviousHighlightedClosingTag.y(), QTextCursor::KeepAnchor);
        unHighlightingTextCursor.block().setUserData(NULL);
        //unHighlightingTextCursor.mergeCharFormat(format);
        PreviousHighlightedClosingTag.setX(0);
        PreviousHighlightedClosingTag.setY(0);
    }

    parsingTextCursor.movePosition(QTextCursor::StartOfWord);
    parsingTextCursor.movePosition(QTextCursor::Left);

    if(plain_text[parsingTextCursor.position()] != '<')
        return; // not a(n opening) tag

    parsingTextCursor.movePosition(QTextCursor::Right);
    do
    {
        parsingTextCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
    } while (plain_text[parsingTextCursor.position()] != '>');

    start_parse_position = parsingTextCursor.position();
    opening_tag = parsingTextCursor.selectedText();
    opening_tag = opening_tag.trimmed();

    if(opening_tag.endsWith("/"))
        return; // closing tag is the opening one

    if(opening_tag.contains(' '))
    {
        int index_space;
        index_space = opening_tag.indexOf(' ');
        opening_tag.chop(opening_tag.count() - index_space);
    }

    parsingTextCursor.setPosition(parsingTextCursor.selectionStart());

    do
    {
        parsingTextCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
    } while (plain_text[parsingTextCursor.position()] != '>' && plain_text[parsingTextCursor.position()] != ' ');

    data = new BlockData();
    data->format.setFontUnderline(true);
    data->format.setFontOverline(true);
    parsingTextCursor.block().setUserData(data);

    PreviousHighlightedOpeningTag.setX(parsingTextCursor.selectionStart());
    PreviousHighlightedOpeningTag.setY(parsingTextCursor.selectionEnd());

    parsingTextCursor.setPosition(start_parse_position+1);

    while(!parsingTextCursor.atEnd())
    {
        if( opened_quote == 0 && plain_text[parsingTextCursor.position()] == '"' && plain_text[parsingTextCursor.position()-1] != '\\')
        {
            opened_quote++;
        }
        else if(opened_quote > 0 && plain_text[parsingTextCursor.position()] == '"' && plain_text[parsingTextCursor.position()-1] != '\\')
        {
            opened_quote--;
        }

        if (opened_quote == 0)
        {
            if(plain_text[parsingTextCursor.position()] == '>')
            {
                tag_delimiter_balance--;
            }
            else if(plain_text[parsingTextCursor.position()] == '<')
            {
                tag_delimiter_balance++;
            }
        }


        if ( tag_delimiter_balance == 0 && parsingTextCursor.hasSelection() )
        {
            QString 
                tag_text = parsingTextCursor.selectedText().trimmed();

            tag_text.remove('<');

            if(!tag_text.contains("--"))
            {
                if(tag_text.startsWith('/'))
                {
                    tag_text.remove('/');
                    if(opened_tag_list.empty())
                    {
                        //found our tag, select it
                        parsingTextCursor.movePosition(QTextCursor::PreviousWord);
                        parsingTextCursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);

                        data = new BlockData();
                        data->format.setFontUnderline(true);
                        data->format.setFontOverline(true);
                        parsingTextCursor.block().setUserData(data);
                        PreviousHighlightedClosingTag.setX(parsingTextCursor.selectionStart());
                        PreviousHighlightedClosingTag.setY(parsingTextCursor.selectionEnd());
                        break;
                    }
                    else
                    {
                        opened_tag_list.removeOne(tag_text);
                    }
                }
                else if(!tag_text.contains('/'))
                {
                    if (tag_text.contains(' '))
                    {
                        int first_space = tag_text.indexOf(' ');
                        tag_text.chop(tag_text.count() - first_space);
                    }
                    opened_tag_list.append(tag_text);
                }
                // we don't need to test for <... /> since it won't influence the position of the closing tag
            }
        }

        parsingTextCursor.movePosition(QTextCursor::Right, tag_delimiter_balance > 0 ? QTextCursor::KeepAnchor : QTextCursor::MoveAnchor);
    }

    parentFile->rehighlight();
}

// Protected:

void CodeEditor::keyPressEvent(QKeyEvent * e)
{
    if (e->key() == Qt::Key_Tab || e->key() == Qt::Key_Backtab) {
        if (AutoCompleter->popup()->isVisible()||TagAutoCompleter->popup()->isVisible())
        {
            e->ignore();
            return;
        }

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

        if (AutoCompleter->popup()->isVisible()||TagAutoCompleter->popup()->isVisible())
        {
            e->ignore();
            return;
        }

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
            QPlainTextEdit::keyPressEvent(e);
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
        else if(toPlainText()[textCursor().position()] == '\n')
        {
            textCursor().beginEditBlock();
            QTextCursor editingTextCursor = textCursor();
            do
            {
                editingTextCursor.movePosition( QTextCursor::Right, (e->modifiers() & Qt::ShiftModifier) == 0 ? QTextCursor::MoveAnchor : QTextCursor::KeepAnchor );
            } while (toPlainText()[editingTextCursor.position()] == ' ');

            setTextCursor(editingTextCursor);
            textCursor().endEditBlock();
        }
        else
        {
            QPlainTextEdit::keyPressEvent(e);
        }
    }
    else if (e->key() == Qt::Key_Delete)
    {
        if(textCursor().hasSelection())
        {
            QPlainTextEdit::keyPressEvent(e);
            return;
        }

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
            QPlainTextEdit::keyPressEvent(e);
        }
    }
    else if (e->key() == Qt::Key_Backspace)
    {
        if(textCursor().position()>0 && toPlainText()[textCursor().position()-1] == ' ' && !textCursor().hasSelection() )
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
            QPlainTextEdit::keyPressEvent(e);
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
    else if (e->key() == Qt::Key_Space && (e->modifiers() & Qt::ControlModifier) != 0) {
        int character_position = 0;
        QTextCursor editingTextCursor = textCursor();

        if (AutoCompleter->popup()->isVisible())
        {
            AutoCompleter->popup()->hide();
        }
        if (TagAutoCompleter->popup()->isVisible())
        {
            TagAutoCompleter->popup()->hide();
        }

        editingTextCursor.setPosition(textCursor().selectionStart());
        editingTextCursor.movePosition( QTextCursor::StartOfWord, QTextCursor::KeepAnchor );
        if(editingTextCursor.position()-1 >=0 )
        {
            character_position = editingTextCursor.position() - 1;
        }

        if (toPlainText()[character_position] == '<')
        {
            TagAutoCompleter->setCompletionPrefix( editingTextCursor.selectedText().trimmed() );
            TagAutoCompleter->complete();
        }
        else
        {
            AutoCompleter->setCompletionPrefix( editingTextCursor.selectedText().trimmed() );
            AutoCompleter->complete();
        }

    }
    else
    {
        QPlainTextEdit::keyPressEvent(e);
    }

    if(AutoCompleter->popup()->isVisible())
    {
        QTextCursor editingTextCursor = textCursor();

        editingTextCursor.setPosition(textCursor().selectionStart());
        editingTextCursor.movePosition( QTextCursor::StartOfWord, QTextCursor::KeepAnchor );
        AutoCompleter->setCompletionPrefix( editingTextCursor.selectedText().trimmed() );
        AutoCompleter->complete();
    }

    if(TagAutoCompleter->popup()->isVisible())
    {
        QTextCursor editingTextCursor = textCursor();

        editingTextCursor.setPosition(textCursor().selectionStart());
        editingTextCursor.movePosition( QTextCursor::StartOfWord, QTextCursor::KeepAnchor );
        TagAutoCompleter->setCompletionPrefix( editingTextCursor.selectedText().trimmed() );
        TagAutoCompleter->complete();
    }

    if (document()->isModified())
        Rockete::getInstance().codeTextChanged();

}

void CodeEditor::keyReleaseEvent(QKeyEvent * /*e*/)
{
}