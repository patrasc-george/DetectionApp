#pragma once
#include <iostream>

#ifdef OBJECTDETECTION_EXPORTS
#define OBJECTDETECTION_API __declspec(dllexport)
#else
#define OBJECTDETECTION_API __declspec(dllimport)
#endif

class OBJECTDETECTION_API Serializable {
public:
    virtual ~Serializable() = default;
    virtual void serialize(const std::string& filePath) const = 0;
    virtual void deserialize(const std::string& filePath) = 0;
    virtual std::string getSerializationFile() const = 0;
};