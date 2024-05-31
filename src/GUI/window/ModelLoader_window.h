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
#include <qcombobox.h>
#include <custom_widgets/LabeledSlider.hpp>

class DetectorEditor : public QDialog {
	Q_OBJECT

private:
	QLineEdit* name;
	QPushButton* cascade_btn;
	QPushButton* network_btn;

	QWidget* cascadeActions;
	QPushButton* new_cascade_btn;
	QComboBox* primaryList;

	QVBoxLayout* headerLayout;
	QVBoxLayout* filesLayout;
	QVBoxLayout* bottomLayout;

	int cascadesCount = 0;
	int currentPrimaryIndex = 0;

	// { path, role } roles: cascade, model, config, classes file
	QMap<QString, QString> paths;
	QString serializationFile;

	QPushButton* ok;
	QPushButton* cancel;
	friend class DetectorFileWidget;

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
	DetectorEditor(const QString& path, QWidget* parent = nullptr);

private slots:

	/**
	* @brief Changes the type of the detector.
	* The mean values widget is shown or hidden depending on the type.
	* The labels of the buttons are also updated to account for the change in type.
	* @return void
	*/
	void typeChanged(bool reset);

	void submit();

public:
	// Getters
	QString getName() { return name->text(); }
	QString getType() { return cascade_btn->isChecked() ? "cascade" : "network"; }
	QString getSerializationFile() { return serializationFile; }
signals:
	void setPrimary(int);
	void editingFinished(const QString& new_file = QString());
};

class DetectorListWindow : public QDialog {
	Q_OBJECT

private:
	QPushButton* add_btn;
	QPushButton* remove_btn;
	QPushButton* edit_btn;
	QStringList originalPaths;
	QStringList selectionPaths;
	QListWidget* list;

	void deselect(const QString& str);
public:
	/**
	* @brief Constructs a DetectorListWindow object.
	* * @details This constructor initializes a DetectorListWindow object with the provided parameters.
	* * It initializes the widgets and layouts for the dialog box, includeing the following:
	* * - A list widget to show the names of the detectors.
	* * - Buttons to add, remove or edit a detector.
	* * It also connects the buttons to their respective slots.
	* * @param[in] a list of paths to the serialization files of each detector
	* * @param[in] parent The parent widget.
	*/
	DetectorListWindow(const QStringList& paths, QWidget* parent = nullptr);

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
	* @brief Opens a DetectorEditor to add a new detector.
	* It also connects the dialog box to the detectorAdded() signal.
	* When the addition is accepted, the detector will be added in the list widget, a new serialization file will be stored, and the detectorAdded() signal will be emitted.
	*/
	void addDetectorRequest();

	/**
	* @brief Opens a dialog box to remove detector(s).
	* It also connects the dialog box to the detectorAdded() signal.
	* When the dialog is accepted, the selected detectors will be removed from the list widget along with their serialization file, and the detectorRemoved() signal will be emitted.
	*/
	void removeDetectorsRequest();

	/**
	* @brief Opens a DetectorEditor to edit an existing detector.
	* It also connects the dialog box to the detectorAdded() signal.
	* When the edit is accepted, the detector's serialization file will be edited, and the detectorEdited() signal will be emitted.
	*/
	void editDetectorRequest(const QString& path);

	/**
	* @brief Updates the enabled state of the remove and edit buttons.
	*/
	void selectionChanged();
};

class DetectorFileWidget : public QWidget {
	Q_OBJECT
public:
	QLabel* label;
	QLabel* fileName;
	QPushButton* uploadButton;
	int shape = -1;
	QString objectName;
	DetectorFileWidget(const QString& role, DetectorEditor* editor, QWidget* parent = nullptr);

	QPushButton* rect;
	QPushButton* circle;
	QLineEdit* changeLabel;
	int index = 0;
private:
	DetectorEditor* editor;
public slots:
	void onLabelChanged();
};