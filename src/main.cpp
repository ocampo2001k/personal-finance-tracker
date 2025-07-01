#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "ui/viewmodels/transactionmodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QCoreApplication::setOrganizationName("PersonalFinance");
    QCoreApplication::setApplicationName("FinanceTracker");

    // Register C++ model type for use in QML (creates a new instance in QML when constructed)
    qmlRegisterType<TransactionModel>("App.Models", 1, 0, "TransactionModel");

    QQmlApplicationEngine engine;

    // Add the qml directory to the import path
    engine.addImportPath(QStringLiteral("qrc:/"));
    engine.addImportPath(QStringLiteral("./src/ui/qml"));

    const QUrl url(QStringLiteral("qrc:/ui/qml/Main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
} 