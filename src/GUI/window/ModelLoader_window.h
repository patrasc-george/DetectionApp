#pragma once


#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QListWidget>
#include <QMessageBox>
#include <QFileDialog>

class DetectorEditor : public QDialog {
	Q_OBJECT

private:
	QLineEdit* name;
	QPushButton* cascade;
	QPushButton* network;
	QPushButton* file_1;
	QPushButton* file_2;
	QPushButton* file_3;
	QSpinBox* meanR;
	QSpinBox* meanG;
	QSpinBox* meanB;
	QCheckBox* swapRB;
	QPushButton* ok;
	QPushButton* cancel;

	QLabel* file_1_title;
	QLabel* file_2_title;
	QLabel* file_3_title;

	QLabel* file_1_label;
	QLabel* file_2_label;
	QLabel* file_3_label;

	QStringList paths = { 0,0,0 };
	QWidget* meanValuesWidget;
	QString jsonPath;

public:
	/**
	* @brief Constructs a DetectorEditor object.
	* 	 * @details This constructor initializes a DetectorEditor object with the provided parameters.
	* 	 * It initializes the widgets and layouts for the dialog box, includeing the following:
	* 	 * - A label and line edit for the name of the detector.
	* 	 * - A button group for the type of the detector.
	*	 * - Buttons to upload the files for the detector.
	* 	 * - Spin boxes for the mean values of the detector.
	* 	 * - A check box to swap the red and blue channels.
	* 	 * - Buttons to accept or cancel the changes.
	*	 * It also connects the buttons to their respective slots.
	* 	 * @param[in] json The path to the json file containing the detectors.
	* 	 * @param[in] detName The name of the detector to edit.
	* 	 * @param[in] parent The parent widget.
	* 	 *
	*/
	DetectorEditor(QString json, QString detName = "", QWidget* parent = nullptr);

signals:
	/**
	* @brief Emits a signal when a detector is created (or edited).
	* param[in] json The json string containing the detectors.
	* param[in] name The name of the detector.
	* param[in] type The type of the detector.
	* param[in] paths The paths to the files of the detector.
	* param[in] meanValues The mean values of the detector.
	* param[in] swapRB Whether to swap the red and blue channels.
	* @return void
	*/
	void detectorCreated(QString json, QString name, QString type, QStringList paths, QVector<int> meanValues = { 0,0,0 }, bool swapRB = false);

private slots:
	/**
	* @brief Opens a file dialog to select the file for the detector.
	* The file is then saved in the paths vector.
	* The label of the button is also updated to show the name of the file.
	* @return void
	*/
	void openFile();

	/**
	* @brief Changes the type of the detector.
	* The mean values widget is shown or hidden depending on the type.
	* The labels of the buttons are also updated to account for the change in type.
	* @return void
	*/
	void typeChanged();

public:
	// Getters
	QString getName() { return name->text(); }
	QString getType() { return cascade->isChecked() ? "cascade" : "network"; }
	QVector<QString> getPaths() { return this->paths; };
	QString getJsonPath() { return jsonPath; }
	QVector<int> getMeanVals() {
		QVector<int> meanVals;
		meanVals.push_back(meanR->value());
		meanVals.push_back(meanG->value());
		meanVals.push_back(meanB->value());
		return meanVals;
	}
};

class DetectorsList : public QDialog {
	Q_OBJECT

private:
	QPushButton* add;
	QPushButton* remove;
	QPushButton* edit;
	QString jsonPath;
	QPushButton* addDet;
	QPushButton* removeDet;
	QPushButton* editDet;
	QString selectedDetector;

public:
	/**
	* @brief Constructs a DetectorsList object.
	* * @details This constructor initializes a DetectorsList object with the provided parameters.
	* * It initializes the widgets and layouts for the dialog box, includeing the following:
	* * - A list widget to show the names of the detectors.
	* * - Buttons to add, remove or edit a detector.
	* * It also connects the buttons to their respective slots.
	* * @param[in] json The path to the json file containing the detectors.
	* * @param[in] parent The parent widget.
	*/
	DetectorsList(QString json, QWidget* parent = nullptr);

signals:
	/**
	* @brief Emits a signal when a detector is added.
	* @return void
	*/
	void detectorAdded();

	/**
	* @brief Emits a signal when a detector is removed.
	* @return void
	*/
	void detectorRemoved();

	/**
	* @brief Emits a signal when a detector is edited.
	* @return void
	*/
	void detectorEdited();

private slots:
	/**
	* @brief Opens a DetectorEditor to add a detector.
	* It also connects the dialog box to the detectorAdded() signal.
	* When the addition is accepted, the deetector will be added to the json file and the list widget, and the detectorAdded() signal will be emitted.
	*/
	void addDetector();

	/**
	* @brief Opens a dialog box to remoeve a detector.
	* It also connects the dialog box to the detectorAdded() signal.
	* When the dialog is accepted, the deetector will be removed from the json file and the list widget, and the detectorRemoved() signal will be emitted.
	*/
	void removeDetector();

	/**
	* @brief Opens a DetectorEditor to edit a detector.
	* It also connects the dialog box to the detectorAdded() signal.
	* When the edit is accepted, the deetector will be edited in the json file and the list widget, and the detectorEdited() signal will be emitted.
	*/
	void editDetector();

	/**
	* @brief Updates the enabled state of the remove and edit buttons.
	*/
	void selectionChanged();
};