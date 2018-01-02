#include "transform.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	transform w;
	w.show();
	return a.exec();
}
