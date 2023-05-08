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
	static void loadAll(const QString& jsonPath, QVector<Detector*>& vector);
	static QVector<QString> getNames(const QString& jsonPath);
	static QJsonObject getObjectByName(const QString& name, const QString& jsonPath);
	static int getFromFileByName(Detector*&, const QString& name, const QString& path);
	static int getFromJsonObject(Detector*&, const QJsonObject&);
};