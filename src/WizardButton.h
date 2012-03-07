#ifndef WIZARDBUTTON_H
#define WIZARDBUTTON_H

#include <QWizard>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QComboBox>

class WizardButton : public QWizard
{
    Q_OBJECT

public:
    WizardButton(QWidget *parent = 0);

    void accept();
};

class InfoPage : public QWizardPage
{
    Q_OBJECT

public:
    InfoPage(QWidget *parent = 0);


public slots:
    void checkboxChanged(int state);

private:
    QLabel *buttonClassLabel;
    QLabel *buttonOnClickEventLabel;
    QLabel *buttonTextTagLabel;
    QLabel *buttonTextValueLabel;
    QLineEdit *buttonClassLineEdit;
    QLineEdit *buttonOnClickEventLineEdit;
    QLineEdit *buttonTextTagLineEdit;
    QLineEdit *buttonTextValueLineEdit;
    QCheckBox *buttonHasTextCheckbox;
};

class PositionSizePage : public QWizardPage
{
    Q_OBJECT

public:
    PositionSizePage(QWidget *parent = 0);

private:
    QRadioButton *toClipboardRadioButton;
    QRadioButton *onCursorRadioButton;
    QRadioButton *positionRelativeRadioButton;
    QRadioButton *positionAbsoluteRadioButton;
    QGroupBox *groupBoxRML;
    QGroupBox *groupBoxRCSS;
    QGroupBox *groupBoxPosition;
    QComboBox *rcssFilesComboBox;
    QLabel *WidthLabel;
    QLabel *HeightLabel;
    QLabel *ZIndexLabel;
    QLineEdit *WidthLineEdit;
    QLineEdit *HeightLineEdit;
    QLineEdit *ZIndexLineEdit;
};


class DecorationPage : public QWizardPage
{
    Q_OBJECT

public:
    DecorationPage(QWidget *parent = 0);

private:
    QRadioButton *imageRadioButton;
    QGroupBox *groupBoxDecorator;
    QLabel *imageDefaultLabel;
    QLabel *imageHoverLabel;
    QLabel *imagePressedLabel;
    QLineEdit *imageDefaultLineEdit;
    QLineEdit *imageHoverLineEdit;
    QLineEdit *imagePressedLineEdit;
};

/*
class ConclusionPage : public QWizardPage
{
    Q_OBJECT

public:
    ConclusionPage(QWidget *parent = 0);

protected:
    void initializePage();

private:
    QLabel *label;
};
*/
#endif
