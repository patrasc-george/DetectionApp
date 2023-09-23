#pragma once
#include "Detection.h"

#include <iterator>
#include <opencv2/core.hpp>
#include <set>
#include <memory>

#ifdef OBJECTDETECTION_EXPORTS
#define OBJECTDETECTION_API __declspec(dllexport)
#else
#define OBJECTDETECTION_API __declspec(dllimport)
#endif

class OBJECTDETECTION_API DetectionMat {
public:
	/**
	* @brief Add a detected object to the collection.
	* @details This function adds a detected object to the collection for further processing and rendering.
	* @param[in] detection A shared pointer to the detected object to be added.
	*/
	void add(std::shared_ptr<Detection>& detection);

	/**
	* @brief Sort the detected objects by confidence score in descending order.
	* @details This function sorts the detected objects based on their confidence scores in descending order.
	The objects with higher confidence scores appear first in the collection after sorting.
	*/
	void sortByConfidence();

	/**
	* @brief Set the rendering status of a detected object at a specific index.
	* @details This function allows you to control the rendering status of a specific detected object
	at the specified index in the collection. Enabling rendering will cause the object to be rendered
	when the `render` function is called.
	* @param[in] index The index of the detected object to modify.
	* @param[in] enableRender Set to `true` to enable rendering, `false` to disable.
	*/
	void setShapeRenderStatus(size_t index, bool enableRender);

	/**
	* @brief Toggle the visibility of confidence scores for all detected objects.
	* @details This function toggles the visibility of confidence scores for all detected objects in the collection.
	When confidence scores are shown, they will be rendered along with the objects.
	* @param[in] show Set to `true` to show confidence scores, `false` to hide them.
	*/
	void setShowConfidence(bool show);

	/**
	* @brief Render the detected objects on an image.
	* @details This function renders the detected objects onto the specified image. Only objects with rendering
	enabled will be displayed on the image.
	* @param[in,out] image The image on which to render the detected objects.
	*/
	void render(cv::Mat& image);

	/**
	* @brief Get a copy of all detected objects as a vector.
	* @details This function returns a vector containing copies of all the detected objects currently in the collection.
	* @return A vector containing copies of all detected objects.
	*/
	std::vector<Detection> getAll();

	class OBJECTDETECTION_API iterator {
	public:
		iterator(std::vector<std::shared_ptr<Detection>>::iterator it);
		iterator& operator++();
		iterator operator++(int);
		Detection& operator*();
		bool operator==(const iterator&) const;
		bool operator!=(const iterator&) const;
	private:
		std::vector<std::shared_ptr<Detection>>::iterator iter;
	};

private:
	std::vector<std::shared_ptr<Detection>> detections;
public:

	/**
	* @brief Construct a `DetectionMat` object from a vector of detected objects.
	* @details This constructor initializes a `DetectionMat` object with a vector of detected objects.
	The provided vector of detected objects will be used as the initial content of the collection.
	* @param[in] detections A vector containing shared pointers to detected objects.
	*/
	DetectionMat(const std::vector<std::shared_ptr<Detection>>& detections);

	/**
	 * @brief Get an iterator to the beginning of the detected objects.
	* @details This function returns an iterator pointing to the beginning of the detected objects in the collection.
	* @return An iterator pointing to the beginning of the detected objects.
	*/
	DetectionMat::iterator begin();

	/**
	* @brief Get an iterator to the end of the detected objects.
	* @details This function returns an iterator pointing to the end of the detected objects in the collection.
	* @return An iterator pointing to the end of the detected objects.
	*/
	DetectionMat::iterator end();

	/**
	* @brief Check if there are no detected objects in the collection.
	* @details This function checks if the collection of detected objects is empty and returns `true` if it is,
	indicating that there are no detected objects in the collection.
	* @return `true` if the collection is empty, `false` otherwise.
	*/
	bool empty();

	DetectionMat() = default;
};
