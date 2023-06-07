#pragma once

#include <ObjectDetection.h>

#ifdef OBJECTDETECTION_EXPORTS
#define OBJECTDETECTION_API __declspec(dllexport)
#else
#define OBJECTDETECTION_API __declspec(dllimport)
#endif

enum ModelErrors {
	NAME_NOT_FOUND = 0,
	TYPE_NOT_PROVIDED = -1,
	MODEL_PATH_EMPTY = -2,
	INVALID_CASCADE = -3,
	INF_GRAPH_PATH_EMPTY = -4,
	CANNOT_READ_NETWORK = -5
};

class OBJECTDETECTION_API ModelLoader {
public:
    /**
     * @brief Loads all detectors from a JSON file.
     * @details This function reads a JSON file containing an array of detector objects.
     It then creates a Detector object for each object in the array and stores it in the provided vector.
     If an error occurs while creating a Detector object, it is logged using qCritical and the object is deleted.
     * @param[in] path The path to the JSON file.
     * @param[out] vector The vector to store the loaded detectors.
     */
    static void loadAll(const QString& jsonPath, QVector<Detector*>& vector);

    /**
     * @brief Gets the names of all detectors from a JSON file.
     * @details This function reads a JSON file containing an array of detector objects and extracts the "name" field from each object.
     It then stores all the names in a vector and returns it. If an object does not have a "name" field or if it is empty, it is skipped.
     * @param[in] jsonPath The path to the JSON file.
     * @return Returns a vector of detector names.
     */
    static QVector<QString> getNames(const QString& jsonPath);

    /**
     * @brief Gets a JSON object representing a detector by its name.
     * @details This function reads a JSON file containing an array of detector objects and searches for an object with the specified name.
     If found, it returns the JSON object representing the detector. If not found, it returns an empty JSON object.
     * @param[in] name The name of the detector to be loaded.
     * @param[in] jsonPath The path to the JSON file.
     * @return Returns the JSON object representing the detector if found, otherwise returns an empty JSON object.
     */
    static QJsonObject getObjectByName(const QString& name, const QString& jsonPath);

    /**
     * @brief Gets a detector from a JSON file by its name.
     * @details This function reads a JSON file containing an array of detector objects and searches for an object with the specified name.
     If found, it creates a Detector object from the JSON object and stores it in the provided pointer.
     If not found, it returns an error code indicating that the name was not found.
     * @param[out] det The pointer to the detector to be loaded.
     * @param[in] name The name of the detector to be loaded.
     * @param[in] jsonPath The path to the JSON file.
     * @return Returns 0 if successful, otherwise returns an error code.
     */
    static int getFromFileByName(Detector*&, const QString& name, const QString& path);

    /**
     * @brief Gets a detector from a JSON object.
     * @details This function creates a Detector object from a JSON object.
     It checks the "type" field of the JSON object to determine what type of Detector to create (network or cascade)
     and then reads the relevant properties from the JSON object and uses them to initialize the Detector object.
     If successful, it stores the Detector object in the provided pointer.
     If not successful, it returns an error code indicating what went wrong (e.g. type not provided).
     * @param[out] det The pointer to the detector to be loaded.
     * @param[in] obj The JSON object containing the detector data.
     * @return Returns 0 if successful, otherwise returns an error code.
     */
    static int getFromJsonObject(Detector*&, const QJsonObject&);
};