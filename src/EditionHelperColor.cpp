#include "EditionHelperColor.h"

#include <QColorDialog>
#include <QColor>

bool EditionHelperColor::help(QString &result)
{
    QColor color = QColorDialog::getColor();

    if (color.isValid()) {
        result.reserve(21);
        result = "rgba(";
        result.append(QString::number(color.red()));
        result.append(",");
        result.append(QString::number(color.green()));
        result.append(",");
        result.append(QString::number(color.blue()));
        result.append(",");
        result.append(QString::number(color.alpha()));
        result.append(")");

        return true;
    }

    return false;
}

