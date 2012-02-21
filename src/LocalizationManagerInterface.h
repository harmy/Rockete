#ifndef LOCALIZATIONMANAGERINTERFACE_H
#define LOCALIZATIONMANAGERINTERFACE_H

#include <QtPlugin>

class LocalizationManagerInterface
{
public:

    enum LocalizationLanguage {
        English,
        French,
        Italian,
        Spanish,
        Dutch,
        German,
        EnglishUs,
        FrenchCa,
        Chinese,
        Japanese,
        Portuguese,
        Korean
    };

    virtual ~LocalizationManagerInterface(){}

    static LocalizationManagerInterface & getInstance() {return *instance;}
    static bool hasInstance() {return instance != NULL;}
    static void setInstance(LocalizationManagerInterface *new_instance){instance = new_instance;}

    virtual void initialize(const QString &strings_file_path)=0;
    virtual QString getLocalizationForIdentifier(const QString &identifier)=0;
    virtual void setLanguage(LocalizationLanguage language)=0;
    virtual const QList<LocalizationLanguage> &getSupportedLanguages()=0;
    virtual QString getLanguageNameForLanguage(LocalizationLanguage language)=0;

protected:
    static LocalizationManagerInterface *instance;
};

Q_DECLARE_INTERFACE(LocalizationManagerInterface, "com.rockete.LocalizationManagerInterface/1.0")

#endif
