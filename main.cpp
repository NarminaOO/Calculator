#include "calculator.h"
#include <QApplication>

int main(int argc, char *argv[]) {
   QApplication app(argc, argv);
   Calculator calculator;
   calculator.setStyleSheet("background-color: pink");
   calculator.setWindowTitle("Calculator");
  // calculator.resize(320,200);
   //calculator.setFixedSize(500,300);
   calculator.setFixedWidth(400);
   calculator.setFixedHeight(375);
   calculator.show();
   return app.exec();
}
