#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "windows/MainWindow.hpp"

int main(int argc, char *argv[]) {
//    Q_INIT_RESOURCE(application);
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("Eugene Triguba");
    QCoreApplication::setApplicationName("Photos");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file to open.");
    parser.process(app);

    MainWindow mainWindow;
//    if (!parser.positionalArguments().isEmpty()) {
//        mainWindow.loadFile(parser.positionalArguments().first());
//    }
    mainWindow.show();

    return app.exec();
}
