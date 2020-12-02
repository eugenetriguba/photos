#include <QApplication>
#include <QCommandLineParser>

#include "windows/MainWindow.hpp"

/**
 * Sets up the application by setting the application name,
 * setting up the CLI parser, showing the GUI window, and
 * starting up the loop that waits for user input.
 *
 * The CLI parser allows us to specify an image we want to open
 * up with the application rather than just opening the blank
 * application and opening an image from there.
 *
 * Args:
 *   argc: The argument count.
 *   argv: The arguments we've passed in.
 *
 * Returns:
 *   -1 if we've specified a filepath for an image to open but
 *   we've unable to open that image.
 */
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QGuiApplication::setApplicationDisplayName(MainWindow::tr("Photos"));

    MainWindow MainWindow;
    QCommandLineParser commandLineParser;
    commandLineParser.addHelpOption();
    commandLineParser.addPositionalArgument(
        MainWindow::tr("[filepath]"),
        MainWindow::tr("The filepath to the image to view."));
    commandLineParser.process(QCoreApplication::arguments());

    if (!commandLineParser.positionalArguments().isEmpty()) {
        bool opened =
            MainWindow.openFile(commandLineParser.positionalArguments().front());

        if (!opened) {
            return -1;
        }
    }

    MainWindow.show();
    return app.exec();
}
