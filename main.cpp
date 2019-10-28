#include "mainwindow.h"
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

            } catch (const std::exception& ex) {
                auto pMessage = ex.what();
                if (pMessage) {
                    std::cout << "An unhandled exception was thrown:\n" << pMessage;
                }
                return -1;
            }


}
