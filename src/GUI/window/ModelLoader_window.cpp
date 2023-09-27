#include "ModelLoader_window.h"

#include "Detector.h"
#include "NeuralNetworkDetector.h"
#include "DetectorFactory.h"
#include "CascadeClassifierDetector.h"
#include "CascadeClassifierGroup.h"
#include "OnnxDetector.h"

DetectorEditor::DetectorEditor(const QString& path = "", QWidget* parent) : QDialog(parent) {
	this->setModal(true);
	this->serializationFile = path;
	this->new_cascade_btn = new QPushButton("+");
	setMinimumWidth(400);

	headerLayout = new QVBoxLayout;
	filesLayout = new QVBoxLayout;
	bottomLayout = new QVBoxLayout;
	cascadeActions = new QWidget;
	cascadeActions->setVisible(false);

	name = new QLineEdit(this);

	cascade_btn = new QPushButton("Cascade Classifier", this);
	cascade_btn->setCheckable(true);
	network_btn = new QPushButton("Neural Network", this);
	network_btn->setCheckable(true);

	primaryList = new QComboBox;
	primaryList->setEnabled(false);
	primaryList->setFixedWidth(200);

	ok = new QPushButton("OK", this);
	cancel = new QPushButton("Cancel", this);

	cascade_btn->setStyleSheet("QPushButton:checked { background-color: hsl(206, 77%, 46%); color: white; }");
	network_btn->setStyleSheet("QPushButton:checked { background-color: hsl(206, 77%, 46%); color: white; }");
	name->setStyleSheet("QLineEdit { background-color: hsl(202, 90%, 90%); height: 30px; }");

	QButtonGroup* group = new QButtonGroup(this);
	group->addButton(cascade_btn);
	group->addButton(network_btn);

	QHBoxLayout* typeButtons = new QHBoxLayout;
	typeButtons->addWidget(cascade_btn);
	typeButtons->addWidget(network_btn);

	headerLayout->addWidget(new QLabel("Serialization File Name", name));
	headerLayout->addWidget(name);
	headerLayout->addWidget(new QLabel("Type", this));
	headerLayout->addLayout(typeButtons);

	QHBoxLayout* buttons = new QHBoxLayout;
	buttons->addWidget(ok);
	buttons->addWidget(cancel);

	QHBoxLayout* cascadesLayout = new QHBoxLayout;
	cascadesLayout->addWidget(new_cascade_btn);
	cascadesLayout->addStretch();
	cascadesLayout->addWidget(new QLabel("Primary classifier: "));
	cascadesLayout->addWidget(primaryList);
	new_cascade_btn->setFixedWidth(30);
	cascadeActions->setLayout(cascadesLayout);

	bottomLayout->addStretch(1);
	bottomLayout->addWidget(cascadeActions);
	bottomLayout->addLayout(buttons);

	connect(cancel, &QPushButton::clicked, this, &DetectorEditor::close);
	connect(cascade_btn, &QPushButton::clicked, this, &DetectorEditor::typeChanged);
	connect(network_btn, &QPushButton::clicked, this, &DetectorEditor::typeChanged);
	connect(new_cascade_btn, &QPushButton::clicked, this, [&] {
		DetectorFileWidget* w = new DetectorFileWidget("cascade", this);
		filesLayout->addWidget(w);
		primaryList->addItem(w->objectName);
		primaryList->setEnabled(true);
		});
	connect(primaryList, &QComboBox::currentIndexChanged, this, [&] {
		emit setPrimary(primaryList->currentIndex());
	});

	connect(ok, &QPushButton::clicked, this, &DetectorEditor::submit);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(headerLayout);
	mainLayout->addLayout(filesLayout);
	mainLayout->addLayout(bottomLayout);
	this->setLayout(mainLayout);

	this->setWindowTitle("Add Detector");

	this->show();

	cascade_btn->setChecked(true);

	if (!QFile(serializationFile).exists() || serializationFile.isEmpty()) {
		typeChanged(true);
		return;
	}

	name->setText(QFileInfo(serializationFile).baseName());
	name->setReadOnly(true);
	name->setStyleSheet("QLineEdit { background-color: hsl(0, 0, 90%); height: 30px; }");

	Detector* detector = DetectorFactory::createDetectorFromFile(serializationFile.toStdString());
	if (!detector)
		return;

	if (NeuralNetworkDetector* nn = dynamic_cast<NeuralNetworkDetector*>(detector)) {
		network_btn->setChecked(true);
		DetectorFileWidget* model_w = new DetectorFileWidget("model", this);
		model_w->fileName->setText(QString(nn->getModelFile().c_str()));
		model_w->fileName->setVisible(true);
		filesLayout->addWidget(model_w);

		if (QFileInfo(model_w->fileName->text()).suffix() != "onnx") {
			DetectorFileWidget* config_w = new DetectorFileWidget("config", this);
			config_w->fileName->setText(QString(nn->getConfigFile().c_str()));
			config_w->fileName->setVisible(true);
			filesLayout->addWidget(config_w);
		}

		DetectorFileWidget* classes_w = new DetectorFileWidget("classes", this);
		classes_w->fileName->setText(QString(nn->getClassesFile().c_str()));
		classes_w->fileName->setVisible(true);
		filesLayout->addWidget(classes_w);
	}
	else if (CascadeClassifierDetector* cc = dynamic_cast<CascadeClassifierDetector*>(detector)) {
		cascade_btn->setChecked(true);
		DetectorFileWidget* cascade_w = new DetectorFileWidget("cascade", this);
		cascade_w->changeLabel->setText(QString(cc->getObjectLabel().c_str()));
		cascade_w->fileName->setText(QString(cc->getCascadeFilePath().c_str()));
		cascade_w->fileName->setVisible(true);
		filesLayout->addWidget(cascade_w);
		primaryList->setItemText(0, cascade_w->changeLabel->text());
	}

	else if (CascadeClassifierGroup* cg = dynamic_cast<CascadeClassifierGroup*>(detector)) {
		cascade_btn->setChecked(true);
		int index = 0;
		auto objectLabels = cg->getObjectLabels();
		auto cascades = cg->getCascades();
		for (const auto& c : cascades) {
			DetectorFileWidget* cascade_w = new DetectorFileWidget("cascade", this);
			disconnect(cascade_w->changeLabel, &QLineEdit::textChanged, cascade_w, &DetectorFileWidget::onLabelChanged);
			primaryList->addItem(objectLabels.at(index).c_str());
			cascade_w->changeLabel->setText(objectLabels.at(index).c_str());
			cascade_w->fileName->setText(c.c_str());
			cascade_w->fileName->setVisible(true);
			filesLayout->addWidget(cascade_w);
			cascade_w->circle->setChecked(cg->objectShape(objectLabels.at(index)));
			++index;
			connect(cascade_w->changeLabel, &QLineEdit::textChanged, cascade_w, &DetectorFileWidget::onLabelChanged);
		}
		primaryList->setCurrentIndex(primaryList->findText(cg->getPrimary().c_str()));
		if (cascadesCount > 1)
			primaryList->setEnabled(true);
		emit setPrimary(primaryList->currentIndex());
	}

	delete detector;

	cascade_btn->setEnabled(false);
	network_btn->setEnabled(false);
	emit typeChanged(false);
}

void DetectorEditor::submit() {
	Detector* detector = nullptr;

	if (network_btn->isChecked()) {
		std::string model, config, classes;
		for (int i = 0; i < filesLayout->count(); ++i) {
			QWidget* widget = filesLayout->itemAt(i)->widget();
			if (widget) {
				DetectorFileWidget* file_w = qobject_cast<DetectorFileWidget*>(widget);
				if (file_w && file_w->label->text() == "Model file") {
					if (file_w->fileName->text().isEmpty()) {
						QMessageBox::warning(this, "Missing field", "Please select a model file");
						file_w->uploadButton->setStyleSheet("QPushButton {color: red;}");
						return;
					}
					model = file_w->fileName->text().toStdString();
				}
				else if (file_w && file_w->label->text() == "Configuration file") {
					if (QFileInfo(file_w->fileName->text()).suffix() != "onnx" && file_w->fileName->text().isEmpty()) {
						QMessageBox::warning(this, "Missing field", "Please select a configuration file");
						file_w->uploadButton->setStyleSheet("QPushButton {color: red;}");
						return;
					}
					config = file_w->fileName->text().toStdString();
				}
				else if (file_w && file_w->label->text() == "Object Labels file") {
					if (file_w->fileName->text().isEmpty()) {
						QMessageBox::warning(this, "Missing field", "Please select a text file containing all the labels for the model");
						file_w->uploadButton->setStyleSheet("QPushButton {color: red;}");
						return;
					}
					classes = file_w->fileName->text().toStdString();
				}
			}
		}
		if (QFileInfo(QString(model.c_str())).suffix() == "onnx")
			detector = new OnnxDetector(model, classes);
		else
			detector = new NeuralNetworkDetector(model, config, classes);
	}
	else {
		std::map<std::string, std::pair<std::string, Detection::Shape>> map; // { label, {cascade, shape} }
		std::string primary;

		for (int i = 0; i < filesLayout->count(); ++i) {
			QWidget* widget = filesLayout->itemAt(i)->widget();
			if (widget) {
				DetectorFileWidget* file_w = qobject_cast<DetectorFileWidget*>(widget);
				if (file_w)
					map[file_w->changeLabel->text().toStdString()] = { file_w->fileName->text().toStdString(), file_w->circle->isChecked() ? Detection::Circle : Detection::Rectangle};
			}
		}
		auto it = map.begin();
		while (it != map.end()) {
			if (it->second.first.empty()) { // cascade
				it = map.erase(it);
			}
			else ++it;
		}
		if (map.size() == 0) {
			QMessageBox::warning(this, "Missing field", "Please add at least one cascade classifier");
			return;
		}
		if (map.size() == 1) {
			detector = new CascadeClassifierDetector(map.begin()->second.first, map.begin()->first);
		}
		else {
			CascadeClassifierGroup* det = new CascadeClassifierGroup;
			for (const auto& pair : map) {
				det->addDetector(pair.second.first, pair.first);
				det->setObjectShape(pair.first, pair.second.second);
			}
			det->setPrimary(primaryList->currentText().toStdString());
			detector = det;
		}
	}

	if (name->text().isEmpty()) {
		QMessageBox::warning(this, "Missing field", "Please set a name for the serialization file. This name will be displayed in the detector list.");
		name->setStyleSheet("QLineEdit {border-color: red;}");
		name->setFocus();
		delete detector;
		return;
	}
	detector->serialize("../detector_paths/" + name->text().toStdString() + ".yaml");
	delete detector;
	close();

	emit editingFinished(QString("../detector_paths/%1.yaml)").arg(name->text()));
}

void DetectorEditor::typeChanged(bool reset) {
	cascadeActions->setVisible(cascade_btn->isChecked());
	if (!reset)
		return;

	while (QLayoutItem* item = filesLayout->takeAt(0)) {
		QWidget* widget = item->widget();
		if (widget) {
			delete widget;
		}
	}

	if (cascade_btn->isChecked()) {
		cascadesCount = 0;
		filesLayout->addWidget(new DetectorFileWidget("cascade", this));
		primaryList->clear();
		primaryList->addItem("Cascade 1");
	}
	else {
		filesLayout->addWidget(new DetectorFileWidget("model", this));
		filesLayout->addWidget(new DetectorFileWidget("classes", this));
	}
}

void DetectorListWindow::deselect(const QString& str) {
	QListWidget* list = (QListWidget*)this->layout()->itemAt(0)->widget();
	QModelIndexList selectedIndexes = list->selectionModel()->selectedIndexes();

	foreach(const QModelIndex & index, selectedIndexes) {
		QString itemText = list->model()->data(index, Qt::DisplayRole).toString();
		if (itemText == str) {
			list->selectionModel()->select(index, QItemSelectionModel::Deselect);
			selectionChanged();
		}
	}
}

DetectorListWindow::DetectorListWindow(const QStringList& paths, QWidget* parent) : originalPaths(paths) {
	this->setModal(true);
	this->setWindowTitle("Detectors List");
	this->setMinimumSize(400, 300);


	list = new QListWidget(this);
	list->setSelectionMode(QAbstractItemView::SelectionMode::MultiSelection);

	for (int i = 0; i < originalPaths.size(); i++) {
		QListWidgetItem* item = new QListWidgetItem(QFileInfo(originalPaths.at(i)).baseName(), list);
		item->setFlags(Qt::ItemFlag::ItemIsSelectable | Qt::ItemFlag::ItemIsEnabled);
	}

	connect(list, &QListWidget::itemSelectionChanged, this, &DetectorListWindow::selectionChanged);

	add_btn = new QPushButton("Add New", this);
	remove_btn = new QPushButton("Remove", this);
	edit_btn = new QPushButton("Edit", this);

	remove_btn->setEnabled(false);
	edit_btn->setEnabled(false);

	QHBoxLayout* buttons = new QHBoxLayout();
	buttons->addWidget(add_btn);
	buttons->addWidget(remove_btn);
	buttons->addWidget(edit_btn);

	QVBoxLayout* mainLayout = new QVBoxLayout();
	mainLayout->addWidget(list);
	mainLayout->addLayout(buttons);

	connect(add_btn, SIGNAL(clicked()), this, SLOT(addDetectorRequest()));
	connect(remove_btn, SIGNAL(clicked()), this, SLOT(removeDetectorsRequest()));
	connect(edit_btn, &QPushButton::clicked, this, [&] { editDetectorRequest(selectionPaths.at(0)); });

	this->setLayout(mainLayout);
	this->show();
}

void DetectorListWindow::addDetectorRequest() {
	DetectorEditor* editor = new DetectorEditor();
	editor->show();

	connect(editor, &DetectorEditor::editingFinished, this, [&](const QString& new_file) {
		list->clear();
		if (!new_file.isEmpty())
			originalPaths.append(new_file);
		for (int i = 0; i < originalPaths.size(); i++) {
			QListWidgetItem* item = new QListWidgetItem(QFileInfo(originalPaths.at(i)).baseName(), list);
			item->setFlags(Qt::ItemFlag::ItemIsSelectable | Qt::ItemFlag::ItemIsEnabled);
		}
		emit detectorAdded();
	});
}

void DetectorListWindow::removeDetectorsRequest() {
	QListWidget* list = (QListWidget*)this->layout()->itemAt(0)->widget();

	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, "Remove Detector", "Are you sure you want to remove this detector?", QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::No)
		return;
	else {
		for (int i = 0; i < selectionPaths.size(); ++i) {
			// remove serialization file(s)
			QFile file(selectionPaths.at(i));

			if (!file.exists()) {
				QMessageBox::critical(this, "Error", QString("Tthe file \"%1 is missing or does not exist. Entry will be removed from list.").arg(QFileInfo(file).absolutePath()));
				for (int i = 0; i < list->count(); i++) {
					if (list->item(i)->text() == QFileInfo(file).baseName()) {
						list->takeItem(i);
					}
				}
				break;
			}

			if (!file.remove()) {
				QMessageBox::warning(this, "Error", QString("Couldn't remove detector(s) because of an error. The file might be open in another program."));
				deselect(QFileInfo(file).baseName());
				break;
			}

			// remove from list
			for (int i = 0; i < list->count(); i++) {
				if (list->item(i)->text() == QFileInfo(file).baseName()) {
					list->takeItem(i);
				}
			}
			//emit detectorRemoved();
		}
	}
}

void DetectorListWindow::editDetectorRequest(const QString& path) {
	DetectorEditor* editor = new DetectorEditor(path);
	editor->show();
}

void DetectorListWindow::selectionChanged() {
	QListWidget* list = (QListWidget*)this->layout()->itemAt(0)->widget();
	QModelIndexList selectedIndexes = list->selectionModel()->selectedIndexes();

	selectionPaths.clear();
	for (auto& item : selectedIndexes) {
		QString itemText = originalPaths.at(item.row());
		selectionPaths.append(itemText);
	}

	if (list->selectedItems().size() == 0) {
		remove_btn->setEnabled(false);
		edit_btn->setEnabled(false);
	}
	else if (list->selectedItems().size() == 1) {
		remove_btn->setEnabled(true);
		edit_btn->setEnabled(true);
	}
	else {
		remove_btn->setEnabled(true);
		edit_btn->setEnabled(false);
	}
}

DetectorFileWidget::DetectorFileWidget(const QString& role, DetectorEditor* detEditor, QWidget* parent) : QWidget(parent), editor(detEditor) {
	this->label = new QLabel;
	this->rect = new QPushButton("R");
	rect->setObjectName("shapeButton");
	this->circle = new QPushButton("C");
	circle->setObjectName("shapeButton");
	this->uploadButton = new QPushButton("Upload file");
	this->changeLabel = new QLineEdit;
	changeLabel->setObjectName("labelEdit");
	this->fileName = new QLabel;
	this->changeLabel->setVisible(false);
	uploadButton->setFixedWidth(100);

	if (role == "model") {
		label->setText("Model file");
	}
	else if (role == "config") {
		label->setText("Configuration file");
	}
	else if (role == "classes") {
		label->setText("Object Labels file");
	}
	else if (role == "cascade") {
		editor->cascadesCount++;
		objectName = "Cascade " + QString::number(editor->cascadesCount);
		index = editor->cascadesCount - 1;
		changeLabel->setText(objectName);
		label->setVisible(false);
		changeLabel->setVisible(true);
	}

	fileName->setVisible(false);

	connect(uploadButton, &QPushButton::clicked, this, [&, role] {
		if (role == "model") {
			QString file = QFileDialog::getOpenFileName(this, "Open Model File", "", "Model Files (*.pb *.onnx)");
			if (file.isEmpty())
				return;
			fileName->setText(file);

			if (QFileInfo(file).suffix() == "onnx") {
				if (editor->filesLayout->count() > 2) {
					QWidget* widgetToRemove = editor->filesLayout->itemAt(1)->widget();
					editor->filesLayout->removeWidget(widgetToRemove);
					delete widgetToRemove;
				}
			}
			else {
				if (editor->filesLayout->count() < 3)
					editor->filesLayout->insertWidget(1, new DetectorFileWidget("config", editor));
			}
		}
		else if (role == "config") {
			QString file = QFileDialog::getOpenFileName(this, "Open Config File", "", "Text Files (*.txt);; All Files (*.*)");
			if (file.isEmpty())
				return;
			fileName->setText(file);
		}
		else if (role == "classes") {
			QString file = QFileDialog::getOpenFileName(this, "Open Classes File", "", "Text Files (*.txt);;All Files (*.*)");
			if (file.isEmpty())
				return;
			fileName->setText(file);
		}
		else if (role == "cascade") {
			QString file = QFileDialog::getOpenFileName(this, "Open Cascade File", "", "XML Files (*.xml);;All Files (*.*)");
			if (file.isEmpty())
				return;
			fileName->setText(file);
		}
		fileName->setVisible(true);
	});
	
	fileName->setStyleSheet("QLabel { color: gray;}");
	fileName->setWordWrap(true);

	QVBoxLayout* vbox = new QVBoxLayout;
	vbox->addWidget(label);
	vbox->addWidget(changeLabel);
	vbox->addWidget(fileName);

	QHBoxLayout* hbox = new QHBoxLayout;
	hbox->addWidget(uploadButton);
	hbox->addStretch();

	vbox->addLayout(hbox);
	setLayout(vbox);

	if (role == "cascade") {
		QLabel* shapeLabel = new QLabel("Choose shape");
		hbox->addWidget(shapeLabel);
		hbox->addWidget(rect);
		hbox->addWidget(circle);

		rect->setCheckable(true);
		circle->setCheckable(true);

		rect->setStyleSheet("QPushButton:checked { background-color: hsl(206, 77%, 46%); color: white; }");
		circle->setStyleSheet("QPushButton:checked { background-color: hsl(206, 77%, 46%); color: white; }");

		rect->setFixedWidth(30);
		circle->setFixedWidth(30);

		rect->setChecked(true);

		QButtonGroup* group = new QButtonGroup;
		group->addButton(rect);
		group->addButton(circle);

		connect(group, &QButtonGroup::buttonClicked, this, [&] {
			if (rect->isChecked())
				shape = 0;
			else
				shape = 1;
		});
		connect(editor, &DetectorEditor::setPrimary, this, [&, shapeLabel](int index) {
			rect->setVisible(index != this->index);
			circle->setVisible(index != this->index);
			shapeLabel->setVisible(index != this->index);
		});
		connect(changeLabel, &QLineEdit::textChanged, this, &DetectorFileWidget::onLabelChanged);
	}
}

void DetectorFileWidget::onLabelChanged() {
	editor->primaryList->setItemText(index, changeLabel->text());
}
