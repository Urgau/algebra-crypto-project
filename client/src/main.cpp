/*
** EPITECH PROJECT, 2020
** client/src/main
** File description:
** Client
*/

#include <QCommandLineParser>
#include <QtWidgets/QApplication>
#include "BabelWindow.hpp"
#include "Error.hpp"
#include "Log.hpp"

int main(int argc, char *argv[])
{
    try {
        QApplication app (argc, argv);
        QCoreApplication::setApplicationName("babel");
        QCoreApplication::setApplicationVersion("1.0");

        QCommandLineParser parser;
        parser.addHelpOption();
        parser.addVersionOption();

        parser.addPositionalArgument("server-ip",
                QCoreApplication::translate("main", "Server IP (ex: 127.0.0.1)"));

        QCommandLineOption unsafeCertOption(QStringList() << "unsafe-cert",
            QCoreApplication::translate("main", "Allow unsafe certificate"));
        parser.addOption(unsafeCertOption);

        parser.process(app);

        const QStringList positionalArguments = parser.positionalArguments();
        if (positionalArguments.isEmpty()) {
            throw "Argument 'server-ip' missing";
        }

        Ui::BabelWindow babelWindow(positionalArguments.at(0),
                parser.isSet(unsafeCertOption));
        babelWindow.show();

        return (app.exec());
    } catch (const Error &ex) {
        ERROR(ex.getComponent(), " error: ", ex.what());
    } catch (const std::exception &ex) {
        ERROR("Error of type: ", ex.what());
    } catch(const std::string &ex) {
        ERROR("Error of type: ", ex);
    } catch(const char *ex) {
        ERROR("Error of type: ", ex);
    }
    return (84);
}
