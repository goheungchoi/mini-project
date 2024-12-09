#include "AssetManager.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  QFont font("Arial", 10);
  a.setFont(font);
  AssetManager w;
  w.show();
  return a.exec();
}
