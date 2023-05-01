#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QFile>

#include <ObjectDetection.h>
#include "ModelLoader.h"

void ModelLoader::load(QString path, std::vector<Detector*>& vector) {
    // read the json file
    QString jsonText;
    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    jsonText = file.readAll();
    file.close();

    // get the onjects
    QJsonDocument doc = QJsonDocument::fromJson(jsonText.toUtf8());
    QJsonArray objects = doc.array();

    //go through every object
    for (int i = 0; i < objects.size(); i++) {
        detectorProperties props;

        QJsonObject obj = objects[i].toObject();

        QJsonValue value = obj.value("type");
        if (value == "network") {
            value = obj.value("properties");
            QJsonObject item = value.toObject();

            props.framework = item["framework"].toString().toStdString();
            props.shouldSwapRB = item["swapRB"].toBool();

            QJsonArray means = item["meanValues"].toArray();
            props.meanValues = cv::Scalar(means[0].toDouble(), means[1].toDouble(), means[2].toDouble());

            value = obj.value("paths");
            item = value.toObject();

            props.infGraphPath = item["inf"].toString().toStdString();
            props.classNamesPath = item["classes"].toString().toStdString();
            props.modelPath = item["model"].toString().toStdString();

            ObjectDetector* det = new ObjectDetector(props);
            vector.emplace_back(det);
        }
        else if (value == "cascade") {
            value = obj.value("paths");
            QJsonObject item = value.toObject();

            props.modelPath = item["face"].toString().toStdString();

            FaceDetector* det = new FaceDetector(props, item["eyes"].toString().toStdString());
            vector.emplace_back(det);
        }

    }
}
