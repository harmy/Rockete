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
                PropertyBlock *current_block;
                QString unparsed_properties;
                QString unparsed_owners;
                QStringList unparsed_owner_list;
                QStringList unparsed_property_list;

                current_block = new PropertyBlock;

                unparsed_owners = content.mid(current_index, bracket_begin_index - current_index - 1);
                unparsed_properties = content.mid(bracket_begin_index+1, bracket_end_index - bracket_begin_index - 1);

                unparsed_owner_list = unparsed_owners.split(',');
                unparsed_property_list = unparsed_properties.split(';');

                foreach(const QString & unparsed_owner, unparsed_owner_list)
                {
                    PropertyOwner *new_owner = parseOwner(unparsed_owner.trimmed());
                    if(new_owner)
                    {
                        current_block->ownerList.push_back(new_owner);
                    }
                }

                foreach(const QString & unparsed_property, unparsed_property_list)
                {
                    Property *new_property = parseProperty(unparsed_property.trimmed());
                    if(new_property)
                    {
                        current_block->propertyList.push_back(new_property);
                    }
                }

                propertyBlockList.push_back(current_block);
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

bool StyleSheet::findMatchingProperties(QList<PropertyBlock*> & result, const Element *element) const
{
    bool it_has_result = false;

    foreach(PropertyBlock *block, propertyBlockList)
    {
        foreach(PropertyOwner *owner, block->ownerList)
        {
            const Element *inheriter;

            inheriter = findDirectInheriter(owner,element);

            if(inheriter)
            {
                PropertyOwner *ancester = owner->ancester;
                bool it_matches = true;

                while(ancester)
                {
                    inheriter = findDirectInheriter(ancester,inheriter);

                    if(!inheriter)
                    {
                        it_matches = false;
                        break;
                    }

                    ancester = ancester->ancester;
                }

                if(it_matches)
                {
                    result.push_back(block);
                    it_has_result = true;
                    break;
                }
            }
        }
    }

    return it_has_result;
}

// Private:

const Element *StyleSheet::findDirectInheriter(const PropertyOwner *owner, const Element *_element) const
{
    const Element *element = _element;

    while(element)
    {
        if( (owner->tagName.isEmpty() || owner->tagName == element->GetTagName().CString())
            && (owner->identifier.isEmpty() || owner->tagName == element->GetId().CString())
            && (owner->className.isEmpty() || element->IsClassSet(owner->className.toStdString().c_str()))
            )
        {
            return element;
        }

        element = element->GetParentNode();
    }

    return NULL;
}

StyleSheet::PropertyOwner *StyleSheet::parseOwner(const QString & content)
{
    int index;
    PropertyOwner *last_owner = NULL;
    QString content_copy = content;
    QString special;
    QStringList content_list = content_copy.replace('\n',' ').split(' ');

    foreach(const QString &content_list_item, content_list)
    {
        PropertyOwner *current_owner = NULL;
        QString unparsed_owner = content_list_item;

        index = unparsed_owner.indexOf(':');

        // :TODO: Handle multi-special (ex: radio:checked:hover)

        if(index!=-1)
        {
            special = unparsed_owner.mid(index+1).trimmed();
            unparsed_owner.remove(index,content_list.last().length() - index);
        }

        index = unparsed_owner.indexOf('.');

        if(index!=-1)
        {
            current_owner = new PropertyOwner;
            current_owner->tagName = unparsed_owner.mid(0,index).trimmed();
            current_owner->className = unparsed_owner.mid(index+1).trimmed();
            current_owner->pseudoClassName = special;
        }

        index = unparsed_owner.indexOf('#');

        if(index!=-1)
        {
            current_owner = new PropertyOwner;
            current_owner->tagName = unparsed_owner.mid(0,index).trimmed();
            current_owner->identifier = unparsed_owner.mid(index+1).trimmed();
            current_owner->pseudoClassName = special;
        }

        if(!current_owner)
        {
            current_owner = new PropertyOwner;
            current_owner->tagName = unparsed_owner.trimmed();
            current_owner->pseudoClassName = special;
        }

        if(last_owner)
        {
            current_owner->ancester = last_owner;
        }

        last_owner = current_owner;
    }


    return last_owner;
}

StyleSheet::Property *StyleSheet::parseProperty(const QString & content)
{
    Property *result = NULL;
    int index = content.indexOf(':');

    if(index!=-1)
    {
        result = new Property;
        result->name = content.mid(0,index).trimmed();
        result->value = content.mid(index+1).trimmed();
    }

    return result;
}
