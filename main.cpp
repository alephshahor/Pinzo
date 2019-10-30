#include "mainwindow.h"
#include "include/util.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{

        try {
                // bootstrap Catch, running all TEST_CASE sequences.
                QApplication a(argc, argv);
                MainWindow w;
                w.show();
                return a.exec();

            } catch (const char* ex) {
                    std::cerr << "An unhandled exception was thrown:\n" << ex;
                    return -1;
            }


}
