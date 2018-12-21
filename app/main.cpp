#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "flightregistry.h"
#include "flightitem.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QQmlContext *rootContext = engine.rootContext();

    qmlRegisterType<FlightItem>("Flight", 1, 0, "FlightItem");

    FlightRegistry flightRegistry;
    rootContext->setContextProperty("flightRegistry", &flightRegistry);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
