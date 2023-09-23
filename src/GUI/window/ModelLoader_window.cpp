#include "ModelLoader_window.h"

DetectorEditor::DetectorEditor(QString json, QString detName, QWidget* parent) {
	this->setModal(true);
	this->jsonPath = json;

	QVBoxLayout* mainLayout = new QVBoxLayout(this);

	name = new QLineEdit(this);
	meanR = new QSpinBox(this);
	meanR->setRange(0, 255);
	meanG = new QSpinBox(this);
	meanG->setRange(0, 255);
	meanB = new QSpinBox(this);
	meanB->setRange(0, 255);
	cascade = new QPushButton("Cascade Classifier", this);
	cascade->setCheckable(true);
	network = new QPushButton("Neural Network", this);
	network->setCheckable(true);
	file_1 = new QPushButton("Upload file", this);
	file_2 = new QPushButton("Upload file", this);
	file_3 = new QPushButton("Upload file", this);
	swapRB = new QCheckBox("Swap Red and Blue channels", this);
	file_1_title = new QLabel("File", this);
	file_1_title->setWordWrap(true);
	file_2_title = new QLabel("File", this);
	file_2_title->setWordWrap(true);
	file_3_title = new QLabel("File", this);
	file_3_title->setWordWrap(true);
	file_1_label = new QLabel("No file selected", this);
	file_1_label->setWordWrap(true);
	file_2_label = new QLabel("No file selected", this);
	file_2_label->setWordWrap(true);
	file_3_label = new QLabel("No file selected", this);
	file_3_label->setWordWrap(true);
	ok = new QPushButton("OK", this);
	cancel = new QPushButton("Cancel", this);

	// set style classes
	cascade->setStyleSheet("QPushButton:checked { background-color: hsl(206, 77%, 46%); color: white; }");
	network->setStyleSheet("QPushButton:checked { background-color: hsl(206, 77%, 46%); color: white; }");
	name->setStyleSheet("QLineEdit { background-color: hsl(202, 90%, 90%); height: 30px; }");

	file_1->setFixedWidth(100);
	file_2->setFixedWidth(100);
	file_3->setFixedWidth(100);

	QButtonGroup* group = new QButtonGroup(this);
	group->addButton(cascade);
	group->addButton(network);

	QHBoxLayout* typeButtons = new QHBoxLayout();
	typeButtons->addWidget(cascade);
	typeButtons->addWidget(network);

	QHBoxLayout* buttons = new QHBoxLayout();

	buttons->addWidget(ok);
	buttons->addWidget(cancel);

	mainLayout->addWidget(new QLabel("Name", name));
	mainLayout->addWidget(name);
	mainLayout->addWidget(new QLabel("Type", this));
	mainLayout->addLayout(typeButtons);
	mainLayout->addWidget(file_1_title);
	mainLayout->addWidget(file_1_label);
	mainLayout->addWidget(file_1);
	mainLayout->addWidget(file_2_title);
	mainLayout->addWidget(file_2_label);
	mainLayout->addWidget(file_2);
	mainLayout->addWidget(file_3_title);
	mainLayout->addWidget(file_3_label);
	mainLayout->addWidget(file_3);

	meanValuesWidget = new QWidget(this);
	meanValuesWidget->setLayout(new QHBoxLayout(meanValuesWidget));
	meanValuesWidget->layout()->addWidget(new QLabel("Mean Values (optional): ", this));
	meanValuesWidget->layout()->addWidget(new QLabel("R=", this));
	meanValuesWidget->layout()->addWidget(meanR);
	meanValuesWidget->layout()->addWidget(new QLabel("G=", this));
	meanValuesWidget->layout()->addWidget(meanG);
	meanValuesWidget->layout()->addWidget(new QLabel("B=", this));
	meanValuesWidget->layout()->addWidget(meanB);
	meanValuesWidget->layout()->addWidget(swapRB);

	mainLayout->addWidget(meanValuesWidget);
	mainLayout->addWidget(swapRB);
	mainLayout->addLayout(buttons);

	connect(file_1, &QPushButton::clicked, this, &DetectorEditor::openFile);
	connect(file_2, &QPushButton::clicked, this, &DetectorEditor::openFile);
	connect(file_3, &QPushButton::clicked, this, &DetectorEditor::openFile);
	connect(ok, &QPushButton::clicked, this, [&] {
		if (name->text() == "") {
			QMessageBox::warning(this, "Error", "Please enter a name for the detector");
			return;
		}
		if (paths.at(0).isEmpty()) {
			QMessageBox::warning(this, "Error", "Please select a file for the detector");
			file_1_label->setStyleSheet(" QLabel { color: red; }");
			return;
		}
		if (network->isChecked() && paths.at(1).isEmpty()) {
			QMessageBox::warning(this, "Error", "Please select a file for the classes");
			file_2_label->setStyleSheet(" QLabel { color: red; }");
			return;
		}
		if (network->isChecked() && paths.at(2).isEmpty()) {
			QMessageBox::warning(this, "Error", "Please select a file for the weights");
			file_3_label->setStyleSheet(" QLabel { color: red; }");
			return;
		}

		close();
		emit detectorCreated(getJsonPath(), getName(), getType(), getPaths(), getMeanVals(), swapRB->isChecked());
		});
	connect(cancel, &QPushButton::clicked, this, &DetectorEditor::close);
	connect(cascade, &QPushButton::toggled, this, &DetectorEditor::typeChanged);
	connect(network, &QPushButton::toggled, this, &DetectorEditor::typeChanged);

	typeChanged();

	this->setLayout(mainLayout);

	this->setWindowTitle("Add Detector");

	this->show();

	cascade->setChecked(true);
	emit typeChanged();

	if (detName == "")
		return;

	name->setText(detName);
	name->setReadOnly(true);
	name->setStyleSheet("QLineEdit { background-color: hsl(0, 0, 90%); height: 30px; }");

	//QJsonObject obj = ModelLoader::getObjectByName(detName, jsonPath);
	//// find the detector by name

	//if (obj.isEmpty())
	//	return;

	//if (obj["type"].toString() == "cascade") {
	//	cascade->setChecked(true);
	//	network->setChecked(false);
	//}
	//else {
	//	cascade->setChecked(false);
	//	network->setChecked(true);
	//}
	cascade->setEnabled(false);
	network->setEnabled(false);
	emit typeChanged();

	//QJsonObject props = obj["properties"].toObject();
	//if (props.contains("meanValues")) {
	//	QJsonArray meanValues = props["meanValues"].toArray();
	//	meanR->setValue(!meanValues.isEmpty() ? meanValues[0].toInt() : 0);
	//	meanG->setValue(!meanValues.isEmpty() ? meanValues[1].toInt() : 0);
	//	meanB->setValue(!meanValues.isEmpty() ? meanValues[2].toInt() : 0);
	//}

	//if (props.contains("swapRB"))
	//	swapRB->setChecked(props["swapRB"].toBool());

	//QJsonObject paths = obj["paths"].toObject();
	//if (cascade->isChecked()) {
	//	this->paths.emplace(0, paths["face"].toString());
	//	if (paths["face"].toString() != "")
	//		file_1_label->setText(paths["face"].toString());

	//	this->paths.emplace(1, paths["eyes"].toString());
	//	if (paths["eyes"].toString() != "")
	//		file_2_label->setText(paths["eyes"].toString());

	//	this->paths.emplace(2, paths["smile"].toString());
	//	if (paths["smile"].toString() != "")
	//		file_3_label->setText(paths["smile"].toString());
	//}
	//else {
	//	this->paths.emplace(0, paths["inf"].toString());
	//	if (paths["inf"].toString() != "")
	//		file_1_label->setText(paths["inf"].toString());

	//	this->paths.emplace(1, paths["classes"].toString());
	//	if (paths["classes"].toString() != "")
	//		file_2_label->setText(paths["classes"].toString());

	//	this->paths.emplace(2, paths["model"].toString());
	//	if (paths["model"].toString() != "")
	//		file_3_label->setText(paths["model"].toString());
	//}
}


void DetectorEditor::openFile() {
	QPushButton* button = (QPushButton*)sender();
	QString filename;

	if (cascade->isChecked())
		filename = QFileDialog::getOpenFileName(this, "Open File", "", "XML Files (*.xml);;All Files (*.*)");
	else
		filename = QFileDialog::getOpenFileName(this, "Open File", "", "All Files(*.*)");

	if (filename != "") {
		// set label text
		if (button == file_1) {
			file_1_label->setText(filename);
			paths.replace(0, filename);
		}
		else if (button == file_2) {
			file_2_label->setText(filename);
			paths.replace(1, filename);
		}
		else if (button == file_3) {
			file_3_label->setText(filename);
			paths.replace(2, filename);
		}
	}
	// resize the window to accomodate the text
	this->adjustSize();
}
void DetectorEditor::typeChanged() {
	if (cascade->isChecked()) {
		file_1_title->setText("Face Classifier");
		file_2_title->setText("Eyes Classifier (optional)");
		file_3_title->setText("Smile Classifier (optional)");
		meanValuesWidget->setEnabled(false);
		swapRB->setEnabled(false);
	}
	else {
		file_1_title->setText("Frozen Inference Graph");
		file_2_title->setText("Classes File");
		file_3_title->setText("Weights File");
		meanValuesWidget->setEnabled(true);
		swapRB->setEnabled(true);
	}
}

DetectorsList::DetectorsList(QString jsonPath, QWidget* parent) {
	this->jsonPath = jsonPath;

	this->setModal(true);
	this->setWindowTitle("Detectors List");
	this->setMinimumSize(400, 300);

	//QVector <QString> names = ModelLoader::getNames(jsonPath);

	//QListWidget* list = new QListWidget(this);

	//for (int i = 0; i < names.size(); i++) {
	//	QListWidgetItem* item = new QListWidgetItem(names[i], list);
	//	item->setFlags(Qt::ItemFlag::ItemIsSelectable | Qt::ItemFlag::ItemIsEnabled);
	//}

	//connect(list, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));

	addDet = new QPushButton("Add", this);
	removeDet = new QPushButton("Remove", this);
	editDet = new QPushButton("Edit", this);

	removeDet->setEnabled(false);
	editDet->setEnabled(false);

	QHBoxLayout* buttons = new QHBoxLayout();
	buttons->addWidget(addDet);
	buttons->addWidget(removeDet);
	buttons->addWidget(editDet);

	QVBoxLayout* mainLayout = new QVBoxLayout();
	//mainLayout->addWidget(list);
	mainLayout->addLayout(buttons);

	connect(addDet, SIGNAL(clicked()), this, SLOT(addDetector()));
	connect(removeDet, SIGNAL(clicked()), this, SLOT(removeDetector()));
	connect(editDet, SIGNAL(clicked()), this, SLOT(editDetector()));

	this->setLayout(mainLayout);

	this->setWindowTitle("Detectors");

	this->show();
}

void DetectorsList::selectionChanged() {
	if (sender() == nullptr)
		return;

	QListWidget* list = (QListWidget*)sender();

	if (list->selectedItems().size() == 0) {
		removeDet->setEnabled(false);
		editDet->setEnabled(false);
	}
	else if (list->selectedItems().size() == 1) {
		removeDet->setEnabled(true);
		editDet->setEnabled(true);
		selectedDetector = list->selectedItems()[0]->text();
	}
	else {
		removeDet->setEnabled(true);
		editDet->setEnabled(false);
	}
}

void DetectorsList::addDetector() {
	DetectorEditor* editor = new DetectorEditor(jsonPath);
	//connect(editor, &DetectorEditor::detectorCreated, this, [&](QString jsonPath, QString name, QString type, QStringList paths, QVector<int> meanValues, bool swapRB) {
	//	// add to json
	//	QFile file(jsonPath);
	//	file.open(QIODevice::ReadOnly | QIODevice::Text);
	//	QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
	//	file.close();

	//	QJsonArray arr = doc.array();
	//	QJsonObject obj;
	//	obj["name"] = name;
	//	obj["type"] = type;

	//	if (obj["type"] == "cascade") {
	//		obj["paths"] = QJsonObject({
	//			{"face", paths.at(0)},
	//			{"eyes", paths.size() > 1 ? paths.at(1) : ""},
	//			{"smile", paths.size() > 2 ? paths.at(2) : ""}
	//			});
	//	}
	//	else {
	//		obj["paths"] = QJsonObject({
	//			{"inf", paths.at(0)},
	//			{"classes", paths.at(1)},
	//			{"model", paths.at(2)}
	//			});
	//		QJsonObject propertiesObj = obj["properties"].toObject();
	//		QJsonArray meanV = propertiesObj["meanValues"].toArray();
	//		meanV.push_back(meanValues.at(0));
	//		meanV.push_back(meanValues.at(1));
	//		meanV.push_back(meanValues.at(2));

	//		propertiesObj["meanValues"] = meanV;
	//		propertiesObj["swapRB"] = swapRB;

	//		obj["properties"] = propertiesObj;
	//	}

	//	arr.append(obj);

	//	doc.setArray(arr);

	//	file.open(QIODevice::WriteOnly | QIODevice::Text);
	//	file.write(doc.toJson());
	//	file.close();

	emit detectorAdded();

	// add to list
	QListWidget* list = (QListWidget*)this->layout()->itemAt(0)->widget();
	//list->addItem(name);
	//});
}

void DetectorsList::removeDetector() {
	// ask if sure
	//QMessageBox::StandardButton reply;
	//reply = QMessageBox::question(this, "Remove Detector", "Are you sure you want to remove this detector?", QMessageBox::Yes | QMessageBox::No);
	//if (reply == QMessageBox::No)
	//	return;
	//else {
	//	// remove from json
	//	QFile file(jsonPath);
	//	file.open(QIODevice::ReadOnly | QIODevice::Text);
	//	QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
	//	file.close();

	//	QJsonArray arr = doc.array();
	//	QJsonArray newArr;
	//	for (int i = 0; i < arr.size(); i++) {
	//		if (arr[i].toObject()["name"].toString() != selectedDetector)
	//			newArr.append(arr[i]);
	//	}

	//	doc.setArray(newArr);

	//	file.open(QIODevice::WriteOnly | QIODevice::Text);
	//	file.write(doc.toJson());
	//	file.close();

	//	// remove from list
	//	QListWidget* list = (QListWidget*)this->layout()->itemAt(0)->widget();
	//	for (int i = 0; i < list->count(); i++) {
	//		if (list->item(i)->text() == selectedDetector) {
	//			list->takeItem(i);
	//			break;
	//		}
	//	}

	//}
	emit detectorRemoved();
}

void DetectorsList::editDetector() {
	DetectorEditor* editor = new DetectorEditor(jsonPath, selectedDetector);
	//connect(editor, &DetectorEditor::detectorCreated, this, [&](QString jsonPath, QString name, QString type, QStringList paths, QVector<int> meanValues, bool swapRB) {
	//	// add to json
	//	QJsonObject obj = ModelLoader::getObjectByName(selectedDetector, jsonPath);

	//	QJsonObject pathsObj = obj["paths"].toObject();
	//	QJsonObject propertiesObj = obj["properties"].toObject();

	//	if (obj["type"] == "cascade") {
	//		pathsObj["face"] = paths.at(0);
	//		pathsObj["eyes"] = paths.size() > 1 ? paths.at(1) : "";
	//		pathsObj["smile"] = paths.size() > 2 ? paths.at(2) : "";
	//	}
	//	else {
	//		pathsObj["inf"] = paths.at(0);
	//		pathsObj["classes"] = paths.at(1);
	//		pathsObj["model"] = paths.at(2);

	//		QJsonArray meanV = propertiesObj["meanValues"].toArray();
	//		meanV.replace(0, meanValues.at(0));
	//		meanV.replace(1, meanValues.at(1));
	//		meanV.replace(2, meanValues.at(2));

	//		propertiesObj["meanValues"] = meanV;
	//		propertiesObj["swapRB"] = swapRB;

	//		obj["properties"] = propertiesObj;
	//	}

	//	obj["paths"] = pathsObj;

	//	QFile file(jsonPath);
	//	file.open(QIODevice::ReadOnly | QIODevice::Text);
	//	QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
	//	file.close();

	//	QJsonArray arr = doc.array();
	//	for (int i = 0; i < arr.size(); i++) {
	//		if (arr[i].toObject()["name"].toString() == selectedDetector) {
	//			arr[i] = obj;
	//			break;
	//		}
		//}

		//doc.setArray(arr);

		//file.open(QIODevice::WriteOnly | QIODevice::Text);
		//file.write(doc.toJson());
		//file.close();

		//emit detectorEdited();

		//this->close();
		//});
}