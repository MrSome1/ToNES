
#include <iostream>

#include <QApplication>

#include "Simulator.h"


int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    tones::Simulator simulator;

    simulator.setWindowTitle("ToNES");
    simulator.show();

    return app.exec();
}
