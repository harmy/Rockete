#include "StyleSheet.h"

#include <QMessageBox>

// Public:

StyleSheet::StyleSheet()
{

}

StyleSheet::~StyleSheet()
{
    clear();
}

bool StyleSheet::parse(const QString & content)
{
    int current_index = 0;
    int bracket_begin_index = 0;
    int bracket_end_index = 0;

    while(1)
    {
        bracket_begin_index = content.indexOf('{',current_index);

        if(bracket_begin_index != -1)
        {
            bracket_end_index = content.indexOf('}',bracket_begin_index);

            if(bracket_end_index != -1)
            {
                QString unparsed_properties;
                QString unparsed_owners;

                unparsed_owners = content.mid(current_index, bracket_begin_index - current_index - 1);
                unparsed_properties = content.mid(bracket_begin_index+1, bracket_end_index - bracket_begin_index - 1);

                QMessageBox msgBox;
                msgBox.setText(unparsed_owners);
                msgBox.exec();
                msgBox.setText(unparsed_properties);
                msgBox.exec();

            }
            else
            {
                return false;
                break;
            }
        }
        else
        {
            break;
        }

        current_index = bracket_end_index+1;
    }

    return false;
}

void StyleSheet::clear()
{
    foreach(PropertyBlock *propertyBlock, propertyBlockList)
    {
        foreach(PropertyOwner *propertyOwner, propertyBlock->ownerList)
        {
            delete propertyOwner;
        }

        foreach(Property *property, propertyBlock->propertyList)
        {
            delete property;
        }

        delete propertyBlock;
    }
}

// Private:

StyleSheet::PropertyOwner *StyleSheet::parseOwner(const QString & content)
{
    int index;
    PropertyOwner * result = NULL;
    QStringList content_list = content.split("\n ");

    index = content_list.last().indexOf('.');

    if(index!=-1)
    {
        result = new PropertyOwner;
        result->tagName = content_list.last().mid(0,index-1).trimmed();
        result->className = content_list.last().mid(index+1).trimmed();
    }

    index = content_list.last().indexOf('#');

    if(index!=-1)
    {
        result = new PropertyOwner;
        result->tagName = content_list.last().mid(0,index-1).trimmed();
        result->identifier = content_list.last().mid(index+1).trimmed();
    }

    if(!result)
    {
        result = new PropertyOwner;
        result->tagName = content_list.last().trimmed();
    }

    if(content_list.size()>1)
    {
        // :TODO: Ancesters;
    }


    return result;
}

StyleSheet::Property *StyleSheet::parseProperty(const QString & content)
{

}
