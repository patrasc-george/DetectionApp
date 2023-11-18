#include "window/MainWindow.h"

#include <QFile>
#include <QApplication>

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);

	MainWindow window;
	window.setObjectName("window");

	QFile file(":/assets/stylesheet.qss");
	file.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(file.readAll());
	qApp->setStyleSheet(styleSheet);

	window.move(0, 0);
	window.setWindowTitle("Detection App");
	window.show();
	window.setWindowIcon(QIcon(":/assets/camera_dark.png"));
	window.menu->toggleCamera->setChecked(true); // camera is on
	return app.exec();
}
