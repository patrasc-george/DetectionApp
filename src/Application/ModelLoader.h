#include <ObjectDetection.h>

class ModelLoader {
public:
	static void loadAll(QString jsonPath, QVector<Detector*>& vector);
	static QVector<QString> getNames(QString jsonPath);
	static bool getFromFileByName(Detector*&, QString name, QString path);
	static bool getFromJsonObject(Detector*&, QJsonObject);
};