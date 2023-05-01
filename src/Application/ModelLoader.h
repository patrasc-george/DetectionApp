#include <qjsondocument.h>
#include <ObjectDetection.h>

class ModelLoader {
public:
	static void load(QString jsonPath, std::vector<Detector*>& vector);
};