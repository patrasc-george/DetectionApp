#include <QFile>

#include "mainwindow.h"

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);

	MainWindow window;
	window.setObjectName("window");

	QFile file(":/assets/stylesheet.qss");
	file.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(file.readAll());
	qApp->setStyleSheet(styleSheet);

	window.setWindowTitle("Detection App");
	window.show();
	window.setWindowIcon(QIcon(":/assets/camera_dark.png"));
	window.menu->toggleCamera->setChecked(true); // camera is on
	return app.exec();
}
