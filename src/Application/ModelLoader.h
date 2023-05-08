#include <ObjectDetection.h>

static enum ModelErrors {
	NAME_NOT_FOUND = 0,
	TYPE_NOT_PROVIDED = -1,
	MODEL_PATH_EMPTY = -2,
	INVALID_CASCADE = -3,
	INF_GRAPH_PATH_EMPTY = -4,
	CANNOT_READ_NETWORK = -5
};

class ModelLoader {
public:
	static void loadAll(QString jsonPath, QVector<Detector*>& vector);
	static QVector<QString> getNames(QString jsonPath);
	static QJsonObject getObjectByName(QString name, QString jsonPath);
	static int getFromFileByName(Detector*&, QString name, QString path);
	static int getFromJsonObject(Detector*&, QJsonObject);
};