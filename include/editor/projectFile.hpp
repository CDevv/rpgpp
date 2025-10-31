#ifndef _RPGPP_EDITOR_PROJECTFILE_H
#define _RPGPP_EDITOR_PROJECTFILE_H

#include <memory>
#include <string>

enum EngineFileType {
    FILE_TILESET,
    FILE_ROOM,
    FILE_ACTOR,
    FILE_DIALOGUE,
    FILE_IMAGE,
    FILE_FONT,
    FILE_SOUND
};

#define ENGINEFILETYPE_MAX (7)

class VariantWrapper {
public:
    VariantWrapper() {};
    virtual ~VariantWrapper() {};
};

template<typename T>
class Variant : public VariantWrapper {
public:
    std::unique_ptr<T> data;
    Variant<T>() {};
    Variant<T>(T* p) { data = std::make_unique<T>(); data.reset(p); };
    void set(T* data) { this->data.reset(data); };
    T* get() { return data.get(); };
};

class ProjectFile {
private:
    std::string relativePath;
    EngineFileType fileType;
    static std::array<std::string, ENGINEFILETYPE_MAX> fileTypeNames;
    std::unique_ptr<VariantWrapper> variant;
public:
    ProjectFile();
    ProjectFile(std::string relativePath, EngineFileType fileType);
    static std::array<std::string, ENGINEFILETYPE_MAX> getTypeNames();
    void setFromPath(std::string relativePath);
    std::string getRelativePath();
    EngineFileType getFileType();
    template<typename T>
    T* getData() { return (static_cast<Variant<T>*>(variant.get()))->get(); };
    template<typename T>
    void setData(T* data) { variant.reset(new Variant<T>(data)); };
};

#endif
