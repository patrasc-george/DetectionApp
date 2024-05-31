#pragma once
#include "sidemenu/menu.h"
#include "ModelLoader_window.h"
#include "ImageProcessingUtils.h"
#include "custom_widgets/SceneImageViewer.hpp"

#include <DetectorFactory.h>
#include <QGraphicsPixmapItem>
#include <QHeaderView>
#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QCoreApplication>
#include <QStatusBar>
#include <QStandardPaths>
#include <QMouseEvent>
#include <QTableWidget>


class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	// three main sections
	Menu* menu; // controls
	SceneImageViewer* imageContainer;
	QStatusBar* statusBar;
	QLabel* resLabel;
	QLabel* fpsLabel;
	DetectionMat detMat;
	QMainWindow* magnifierWindow;
	QTableWidget* magnifierTable;

protected:
	void closeEvent(QCloseEvent* event) override;

public:
	/**
	 * @brief Constructs a MainWindow object.
	 * @details This constructor initializes a MainWindow object with the provided parent widget.
	 It creates and initializes various components such as a menu, image container, and status bar.
	 These components are used to interact with the application and control its behavior.
	 The constructor also connects signals emitted by the menu controls to slots in the MainWindow object that handle those signals.
	 The components are arranged in a  and added to the MainWindow widget.
	 * @param[in] parent The parent widget of the MainWindow object.
	 */
	MainWindow(QWidget* parent = nullptr);

	/**
	 * @brief Starts video capture from the camera.
	 * @details This function is called when the camera is turned on.
	 It opens a video capture object and reads frames from the camera.
	 It processes each frame using the selected detector and displays it in the image container.
	 It also updates the FPS label with the current FPS value.
	 */
	void startVideoCapture();

	~MainWindow() {};

private slots:
	/**
	 * @brief Toggles the camera on or off.
	 * @details This function is called when the "Turn Camera On/Off" button is clicked.
	 It checks the state of the button and sets the cameraIsOn and imageIsUpload variables accordingly.
	 It then calls the setOptions() function to update the menu options based on the new state.
	 If the camera is turned on, it starts video capture and selects a detector.
	 If the camera is turned off, it displays an image indicating that the camera is turned off.
	 */
	void toggleCameraEvent();

	/**
	 * @brief Uploads an image selected by the user.
	 * @details This function is called when the "Upload Image" button is clicked.
	 It calls the getImageFileName() function to get the file name of the selected image.
	 If a valid image file is selected, it reads the image data and sets the frame variable to the image data.
	 It then updates the menu options and displays the uploaded image.
	 */
	void uploadImageEvent();

	/**
	 * @brief Selects a detector from the list of available detectors.
	 * @details This function is called when a detector is selected from the list of available detectors.
	 It deletes the current detector object and sets it to nullptr.
	 If a valid detector is selected, it attempts to load the detector from the modelsJSON file.
	 If the detector is successfully loaded, it sets the currDet variable to the loaded detector object.
	 If an image has been uploaded, it processes the image using the selected detector.
	 It then calls the setOptions() function to update the menu options based on the new state.
	 */
	void selectDetectorEvent();

	/**
	 * @brief Changes the minimum confidence value for object detection.
	 * @details This function is called when the minimum confidence value is changed using the slider in the menu.
	 It sets the minimum confidence value of the current detector to the value of the slider.
	 If an image has been uploaded, it processes the image using the new minimum confidence value.
	 */
	void changeMinConfEvent();

	/**
	 * @brief Saves a screenshot of the current image.
	 * @details This function is called when the "Save Screenshot" button is clicked.
	 It opens a file dialog and allows the user to select a file name and location to save the screenshot.
	 If a valid file name is selected, it saves a screenshot of the current image to the specified file.
	 */
	void screenshotEvent();

	/**
	 * @brief Processes the current image using the selected algorithms and detector.
	 * @details This function is called when an image is uploaded or when an algorithm or detector is selected.
	 If an image has been uploaded, it reads the image data from the file.
	 It then calls the selectAlgorithmsEvent() function to apply any selected image processing algorithms to the current frame.
	 It flips the current frame horizontally or vertically if the corresponding buttons are clicked.
	 It then calls the setDetector() function to detect objects in the current frame using the selected detector.
	 If an image has been uploaded, it displays the processed image in the image container.
	 */
	void processImage();

	/**
	 * @brief Changes the threshold value for binary thresholding.
	 * @details This function is called when the threshold value is changed using the slider in the menu.
	 If an image has been uploaded, it processes the image using the new threshold value
	 and updates the status bar with information about the applied threshold value.
	 */
	void changeThresholdEvent();

	void changeKernelSizeEvent();

	/**
	 * @brief Applies selected image processing algorithms to the current frame.
	 * @details This function is called when an image processing algorithm is selected from the list of available algorithms.
	 It checks the state of the algorithm buttons and applies the selected algorithms to the current frame in the order in which they are listed in the menu.
	 For example, if the "Histogram Equalization" and "Binary Thresholding" buttons are both checked,
	 it will first apply histogram equalization to the current frame and then apply binary thresholding to the result.
	 */
	void selectAlgorithmsEvent();

	/**
	 * @brief Event handler for editing detectors in the main window.
	 * @details This function is called when the user edits detectors in the main window.
	 It performs the following tasks:
	 Refreshes the detector list.
	 Deletes all detectors except the first one (None) from the detector list.
	 The function ensures that only the "None" detector remains in the list after editing.
	 */
	void detectorEditEvent();

public:
	QGraphicsPixmapItem pixmap;
	QImage frame;
	QString fileName;
	Detector* currDet;
	OptionsHistory history;
	bool cameraIsOn = false;
	bool imageIsUpload = false;

public:
	/**
	 * @brief Sets the options for the menu based on the current state of the application.
	 * @details This function enables or disables various menu options and sets their text or checked state based on the current state of the application.
	 For example, if the camera is turned on, the "Turn Camera On" button will be changed to "Turn Camera Off".
	 Similarly, if an image has been uploaded, certain options such as zooming in and out will be enabled.
	 */
	void setOptions();

	/**
	 * @brief Sets the detector for object detection.
	 * @details This function is called when a detector is selected from the list of available detectors.
	 If a valid detector is selected, it attempts to detect objects in the current frame using the selected detector.
	 If objects are detected, it updates the status bar with information about the detected objects.
	 */
	void setDetector();

	/**
	 * @brief Flips the current image horizontally or vertically.
	 * @details This function is called when the "Flip Horizontally" or "Flip Vertically" buttons are clicked.
	 It checks the state of the buttons and flips the current frame horizontally or vertically accordingly.
	 */
	void flipImage();

	/**
	 * @brief Displays the current image in the image container.
	 * @details This function converts the current frame to a QImage and sets the pixmap of the image item in the scene to the QImage.
	 It then updates the scene rect to fit the size of the image and updates the resolution label with the resolution of the current frame.
	 */
	void displayImage();

	/**
	 * @brief Returns the file name of an image selected by the user.
	 * @details This function opens a file dialog and allows the user to select an image file.
	 It returns the file name of the selected image.
	 * @return A QString representing the file name of the selected image.
	 */
	QString getImageFileName();

	/**
	 * @brief Prevents the zoom level from being reset when other actions are applied to the image.
	 * @details This function is called when other actions are applied to the image or when the window is resized.
	 It fits the image in view and maintains the current zoom level.
	 */
	void preventReset();

	/**
	* @brief Overrides the default resize event handler.
	* @details This function is an override of the default resize event handler for the MainWindow class.
	It calls the 'preventReset' function to prevent certain actions during window resizing.
	* @param[in] event A pointer to the QResizeEvent object that represents the resize event.
	*/
	void resizeEvent(QResizeEvent* event) override;

	/**
	* @brief Checks if any thresholding option is active in the menu.
	* @details This function checks if any of the binary thresholding, zero thresholding,
	adaptive thresholding, or truncation thresholding options is currently selected in the menu.
	* @return Returns true if at least one thresholding option is active; otherwise, returns false.
	*/
	bool thresholdActive();

	bool kernelActive();

	/**
	* @brief Sorts and updates buttons in the user interface based on detection results.
	* @details This function sorts the buttons in the user interface based on detection results
	and updates their order. It retrieves sorted class names and detected classes from the current
	detector instance, and then rearranges the buttons accordingly.
	* @note This function assumes the existence of specific UI elements and their interactions.
	*/
	void sortButtons();

	void showMagnifierWindow(QPoint imagePoint);

	bool eventFilter(QObject* watched, QEvent* event);

	void zoomReset();
};