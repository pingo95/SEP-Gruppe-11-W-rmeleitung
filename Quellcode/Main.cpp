#include "./presentation/Ui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    presentation::UI w;
    w.show();

    return a.exec();
}
