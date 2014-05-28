#include "mainwindow.h"

#include "averageface.h"
#include "facealigner.h"

#include <QApplication>

#include "glwidget.h"
#include <QTime>

#include "run.h"

/*
 * --compare --rootdir <path-to-root-direcotry> --model1 <path-to-model1> --model2 <path-to-model2>
 * --crdepthamps --rootdir <path-to-root-direcotry> --modelsdir <path-to-3d-models>
 * --evaluate --rootdir <path-to-root-direcotry>
 */
int main(int argc, char *argv[]) {
	QApplication a(argc, argv);

	bool result = false;

	if(a.arguments().size() > 2) {
		if(a.arguments()[1] == "--crdepthmaps"
				&& a.arguments().size() == 6
				&& a.arguments()[2] == "--rootdir"
				&& a.arguments()[4] == "--modelsdir") {

			QString rootDirectory = a.arguments()[3];
			QString modelDirectory = a.arguments()[5];

			Run system(rootDirectory, modelDirectory);

			system.createDepthmaps();
			result = true;

		} else if(a.arguments()[1] == "--evaluate"
				&& a.arguments().size() == 4
				&& a.arguments()[2] == "--rootdir") {

			QString rootDirectory = a.arguments()[3];

			Run system(rootDirectory,"");

			//system.compareFacesInit();
			system.compareFaces();
			system.showResults();
			result = true;

		} else if(a.arguments()[1] == "--compare"
				  && a.arguments().size() == 8
				  && a.arguments()[2] == "--rootdir"
				  && a.arguments()[4] == "--model1"
				  && a.arguments()[6] == "--model2") {

			QString rootDirectory = a.arguments()[3];
			QString model1Path = a.arguments()[5];
			QString model2Path = a.arguments()[7];

			Run system(rootDirectory,"");

			system.compareTwoFaces(model1Path,model2Path);

			result = true;
		}
	}

	if(!result) {
		qWarning() << "usage:";
		qWarning() << "--compare --rootdir <path-to-root-direcotry> --model1 <path-to-model1> --model2 <path-to-model2>";
		qWarning() << "--crdepthamps --rootdir <path-to-root-direcotry> --modelsdir <path-to-3d-models>";
		qWarning() << "--evaluate --rootdir <path-to-root-direcotry>";
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
