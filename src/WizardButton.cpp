#include <QtGui>

#include "WizardButton.h"
#include "Rockete.h"

WizardButton::WizardButton(QWidget *parent)
    : QWizard(parent)
{
    addPage(new InfoPage);
    addPage(new PositionSizePage);
    addPage(new DecorationPage);
    /*addPage(new OutputFilesPage);
    addPage(new ConclusionPage);*/

    setPixmap(QWizard::BannerPixmap, QPixmap(":/images/banner.png"));
    setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/background.png"));

    setWindowTitle(tr("Button Wizard"));
}

void WizardButton::accept()
{
    QString buttonFullClassName = field("buttonClassName").toString();
    QString buttonMainClassName = field("buttonClassName").toString().split(' ').first();
    QString buttonOnClickEventName = field("buttonOnClickEventName").toString();
    QString buttonTextTag = field("buttonTextTag").toString();
    QString buttonTextValue = field("buttonTextValue").toString();
    bool buttonHasText = field("buttonHasText").toBool();

    QString buttonWidth = field("width").toString();
    QString buttonHeight = field("height").toString();
    QString buttonZIndex = field("zindex").toString();
    QString rcssFileName =  field("rcssFileName").toString();
    bool rmlOnCursor = field("onCursor").toBool();
    bool positionAbsolute = field("positionAbsolute").toBool();

    QString defaultImage = field("defaultImage").toString();
    QString hoverImage = field("hoverImage").toString();
    QString pressedImage = field("pressedImage").toString();
    bool imageDecorator = field("decoratorTypeImage").toBool();

    QString new_button_rml;
    QString new_button_rcss;

    new_button_rml = "<button class=\"" + buttonFullClassName + "\"";

    if(!buttonOnClickEventName.isEmpty())
        new_button_rml += " onclick=\"" + buttonOnClickEventName + "\"";

    if(buttonHasText)
    {
        new_button_rml += ">\n";
        new_button_rml += "<" + buttonTextTag + ">\n";
        new_button_rml += buttonTextValue + "\n";
        new_button_rml += "</" + buttonTextTag + ">\n";
        new_button_rml += "</button>\n";
    }
    else
    {
        new_button_rml += "/>";
    }

    if(rmlOnCursor)
    {
        Rockete::getInstance().getCurrentDocument()->addDocumentTextAtCursor(new_button_rml);
    }
    else
    {
        QApplication::clipboard()->setText(new_button_rml);
    }

    new_button_rcss = "button." + buttonMainClassName + "\n{\n";
    new_button_rcss += "display: block;\n";
    new_button_rcss += "position: ";
    new_button_rcss += positionAbsolute ? "absolute" : "relative";
    new_button_rcss += ";\n";
    new_button_rcss += "width: " + buttonWidth + ";\n";
    new_button_rcss += "height: " + buttonHeight + ";\n";
    new_button_rcss += "z-index: " + buttonZIndex + ";\n";
    new_button_rcss += "background-decorator: ";
    new_button_rcss += imageDecorator ? "image" : "WARNING_UNSUPORTED";
    new_button_rcss += ";\n";
    new_button_rcss += "background-image-src: " + defaultImage + ";\n}\n\n";

    new_button_rcss += "button." + buttonMainClassName + ":hover\n{\n";
    new_button_rcss += "background-decorator: ";
    new_button_rcss += imageDecorator ? "image" : "WARNING_UNSUPORTED";
    new_button_rcss += ";\n";
    new_button_rcss += "background-image-src: " + hoverImage + ";\n}\n\n";

    new_button_rcss += "button." + buttonMainClassName + ":active\n{\n";
    new_button_rcss += "background-decorator: ";
    new_button_rcss += imageDecorator ? "image" : "WARNING_UNSUPORTED";
    new_button_rcss += ";\n";
    new_button_rcss += "background-image-src: " + pressedImage + ";\n}\n\n";
    

    QString styleSheetName = rcssFileName.split('/').last();
    if(styleSheetName.isEmpty())
        styleSheetName = rcssFileName.split('\\').last();
    if(styleSheetName.isEmpty())
    {
        QDialog::accept();
        printf("WARNING: could not find stylesheet\n");
        return; // TODO: properly warn user
    }

    Rockete::getInstance().openFile(styleSheetName.toAscii().data());

    OpenedStyleSheet *affectedStyleSheet = Rockete::getInstance().getStyleSheetFromFileName(styleSheetName.toAscii().data());

    if(affectedStyleSheet)
    {
        affectedStyleSheet->addStyleSheetTextAtEnd(new_button_rcss);
    }
    else
    {
        QDialog::accept();
        printf("WARNING: could not find stylesheet\n");
        return; // TODO: properly warn user
    }
    Rockete::getInstance().reloadCurrentDocument();

    QDialog::accept();
}

InfoPage::InfoPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("Button Information");
    setSubTitle("Specify basic information about the button");

    buttonClassLabel = new QLabel("Button class:");
    buttonClassLineEdit = new QLineEdit;
    buttonClassLabel->setBuddy(buttonClassLineEdit);

    buttonOnClickEventLabel = new QLabel("OnClick event name:");
    buttonOnClickEventLineEdit = new QLineEdit;
    buttonOnClickEventLabel->setBuddy(buttonOnClickEventLineEdit);

    buttonTextTagLabel = new QLabel("Tag to use for text:");
    buttonTextTagLineEdit = new QLineEdit;
    buttonTextTagLabel->setBuddy(buttonTextTagLineEdit);

    buttonTextTagLabel->setVisible(false);
    buttonTextTagLineEdit->setVisible(false);

    buttonTextValueLabel = new QLabel("Text:");
    buttonTextValueLineEdit = new QLineEdit;
    buttonTextValueLabel->setBuddy(buttonTextValueLineEdit);

    buttonTextValueLabel->setVisible(false);
    buttonTextValueLineEdit->setVisible(false);

    buttonHasTextCheckbox = new QCheckBox("Button supports text");
    connect(buttonHasTextCheckbox, SIGNAL(stateChanged(int)), this, SLOT(checkboxChanged(int)));

    registerField("buttonClassName*", buttonClassLineEdit);
    registerField("buttonOnClickEventName", buttonOnClickEventLineEdit);
    registerField("buttonTextTag", buttonTextTagLineEdit);
    registerField("buttonTextValue", buttonTextValueLineEdit);
    registerField("buttonHasText", buttonHasTextCheckbox);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(buttonClassLabel, 0, 0);
    layout->addWidget(buttonClassLineEdit, 0, 1);
    layout->addWidget(buttonOnClickEventLabel, 1, 0);
    layout->addWidget(buttonOnClickEventLineEdit, 1, 1);
    layout->addWidget(buttonHasTextCheckbox, 2, 0, 1, 2);
    layout->addWidget(buttonTextTagLabel, 3, 0);
    layout->addWidget(buttonTextTagLineEdit, 3, 1);
    layout->addWidget(buttonTextValueLabel, 4, 0);
    layout->addWidget(buttonTextValueLineEdit, 4, 1);
    setLayout(layout);
}

void InfoPage::checkboxChanged(int state)
{
    buttonTextTagLabel->setVisible(state==Qt::Checked);
    buttonTextTagLineEdit->setVisible(state==Qt::Checked);
    buttonTextValueLabel->setVisible(state==Qt::Checked);
    buttonTextValueLineEdit->setVisible(state==Qt::Checked);
}


PositionSizePage::PositionSizePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Position & Size Options"));
    setSubTitle("Where should the generated RML go and how the button is placed");

    QRadioButton *toClipboardRadioButton;
    QRadioButton *onCursorRadioButton;
    QRadioButton *positionRelativeRadioButton;
    QRadioButton *positionAbsoluteRadioButton;
    QLabel *WidthLabel;
    QLabel *HeightLabel;
    QLabel *ZIndexLabel;
    QLineEdit *WidthLineEdit;
    QLineEdit *HeightLineEdit;
    QLineEdit *ZIndexLineEdit;

    WidthLabel = new QLabel("Button width:");
    WidthLineEdit = new QLineEdit;
    WidthLabel->setBuddy(WidthLineEdit);

    HeightLabel = new QLabel(tr("Button height:"));
    HeightLineEdit = new QLineEdit;
    HeightLabel->setBuddy(HeightLineEdit);

    ZIndexLabel = new QLabel(tr("Z-Index:"));
    ZIndexLineEdit = new QLineEdit;
    ZIndexLabel->setBuddy(ZIndexLineEdit);

    groupBoxRML = new QGroupBox(tr("RML goes in:"));

    toClipboardRadioButton = new QRadioButton(tr("Rml copied to the clipboard"));
    onCursorRadioButton = new QRadioButton(tr("Rml inserted on cursor"));
    onCursorRadioButton->setChecked(true);

    groupBoxRCSS = new QGroupBox(tr("RCSS goes in:"));

    rcssFilesComboBox = new QComboBox(this);

    foreach (QString file, Rockete::getInstance().getCurrentDocument()->getRCSSFileList()) {
        rcssFilesComboBox->addItem(file);
    }

    groupBoxPosition = new QGroupBox(tr("position:"));

    positionRelativeRadioButton = new QRadioButton(tr("Relative"));
    positionAbsoluteRadioButton = new QRadioButton(tr("Absolute"));
    positionAbsoluteRadioButton->setChecked(true);

    QVBoxLayout *groupBoxLayout = new QVBoxLayout;
    groupBoxLayout->addWidget(toClipboardRadioButton);
    groupBoxLayout->addWidget(onCursorRadioButton);
    groupBoxRML->setLayout(groupBoxLayout);

    QVBoxLayout *groupBoxLayout3 = new QVBoxLayout;
    groupBoxLayout3->addWidget(rcssFilesComboBox);
    groupBoxRCSS->setLayout(groupBoxLayout3);

    QVBoxLayout *groupBoxLayout2 = new QVBoxLayout;
    groupBoxLayout2->addWidget(positionRelativeRadioButton);
    groupBoxLayout2->addWidget(positionAbsoluteRadioButton);
    groupBoxPosition->setLayout(groupBoxLayout2);

    registerField("width", WidthLineEdit);
    registerField("height", HeightLineEdit);
    registerField("zindex", ZIndexLineEdit);
    registerField("toClipboard", toClipboardRadioButton);
    registerField("onCursor", onCursorRadioButton);
    registerField("positionRelative", positionRelativeRadioButton);
    registerField("positionAbsolute", positionAbsoluteRadioButton);
    registerField("rcssFileName", rcssFilesComboBox, "currentText");

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(WidthLabel, 0, 0);
    layout->addWidget(WidthLineEdit, 0, 1);

    layout->addWidget(HeightLabel, 1, 0);
    layout->addWidget(HeightLineEdit, 1, 1);

    layout->addWidget(ZIndexLabel, 2, 0);
    layout->addWidget(ZIndexLineEdit, 2, 1);

    layout->addWidget(groupBoxPosition, 3, 0, 1, 2);
    layout->addWidget(groupBoxRML, 4, 0, 1, 2);
    layout->addWidget(groupBoxRCSS, 5, 0, 1, 2);

    setLayout(layout);
}

DecorationPage::DecorationPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Decoration Options"));
    setSubTitle("How it will look");

    groupBoxDecorator = new QGroupBox(tr("Decoration type:"));

    imageRadioButton = new QRadioButton(tr("Image")); // will need(?) other types of decorators
    imageRadioButton->setChecked(true);

    imageDefaultLabel = new QLabel(tr("Default image:"));
    imageDefaultLineEdit = new QLineEdit;
    imageDefaultLabel->setBuddy(imageDefaultLineEdit);

    imageHoverLabel = new QLabel(tr("Hover image:"));
    imageHoverLineEdit = new QLineEdit;
    imageHoverLabel->setBuddy(imageHoverLineEdit);

    imagePressedLabel = new QLabel(tr("Pressed image:"));
    imagePressedLineEdit = new QLineEdit;
    imagePressedLabel->setBuddy(imagePressedLineEdit);

    QHBoxLayout *groupBoxLayout = new QHBoxLayout;
    groupBoxLayout->addWidget(imageRadioButton);
    groupBoxDecorator->setLayout(groupBoxLayout);

    registerField("defaultImage", imageDefaultLineEdit);
    registerField("hoverImage", imageHoverLineEdit);
    registerField("pressedImage", imagePressedLineEdit);
    registerField("decoratorTypeImage", imageRadioButton);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(groupBoxDecorator, 0, 0, 1, 2);
    layout->addWidget(imageDefaultLabel, 1, 0);
    layout->addWidget(imageDefaultLineEdit, 1, 1);
    layout->addWidget(imageHoverLabel, 2, 0);
    layout->addWidget(imageHoverLineEdit, 2, 1);
    layout->addWidget(imagePressedLabel, 3, 0);
    layout->addWidget(imagePressedLineEdit, 3, 1);
    setLayout(layout);
}
/*
void OutputFilesPage::initializePage()
{
QString className = field("className").toString();
headerLineEdit->setText(className.toLower() + ".h");
implementationLineEdit->setText(className.toLower() + ".cpp");
outputDirLineEdit->setText(QDir::convertSeparators(QDir::tempPath()));
}

ConclusionPage::ConclusionPage(QWidget *parent)
: QWizardPage(parent)
{
setTitle(tr("Conclusion"));
setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark2.png"));

label = new QLabel;
label->setWordWrap(true);

QVBoxLayout *layout = new QVBoxLayout;
layout->addWidget(label);
setLayout(layout);
}

void ConclusionPage::initializePage()
{
QString finishText = wizard()->buttonText(QWizard::FinishButton);
finishText.remove('&');
label->setText(tr("Click %1 to generate the class skeleton.")
.arg(finishText));
}
*/
