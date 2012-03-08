#include "RocketFileInterface.h"
#include <Rocket/Core.h>
#include <QString>
#include <QFileInfo>
#include "Rockete.h"


RocketFileInterface::RocketFileInterface()
{

}

// Opens a file.
Rocket::Core::FileHandle RocketFileInterface::Open(const Rocket::Core::String& path)
{
    // since we load from memory, the files will not always have the good path
    // if it doesn't exists, we check for project's paths
    QString real_path = path.CString();
    QFileInfo file_info = real_path;

    if(file_info.exists())
        return (Rocket::Core::FileHandle)fopen(path.CString(), "rb");

    if(real_path.contains("rml") || real_path.contains("rcss") || real_path.contains("lua"))
    {
        real_path = Rockete::getInstance().getPathForFileName(file_info.fileName());
    }
    else
    {
        printf("WARNING: libRocket looking for a file neither rml nor rcss nor lua: %s\n", real_path.toAscii().data());
    }

    file_info = real_path;

    if(!file_info.exists())
    {
        printf("WARNING: File not found %s. Search string: %s\n", file_info.filePath().toAscii().data(), path.CString());
    }


    // TODO: other places to look if its not in the project files ( like texture search)

    return (Rocket::Core::FileHandle)fopen(real_path.toAscii().data(), "rb");
}

// Closes a previously opened file.
void RocketFileInterface::Close(Rocket::Core::FileHandle file)
{
    fclose((FILE*) file);
}

// Reads data from a previously opened file.
size_t RocketFileInterface::Read(void* buffer, size_t size, Rocket::Core::FileHandle file)
{
    return fread(buffer, 1, size, (FILE*) file);
}

// Seeks to a point in a previously opened file.
bool RocketFileInterface::Seek(Rocket::Core::FileHandle file, long offset, int origin)
{
    return fseek((FILE*) file, offset, origin) == 0;
}

// Returns the current position of the file pointer.
size_t RocketFileInterface::Tell(Rocket::Core::FileHandle file)
{
    return ftell((FILE*) file);
}