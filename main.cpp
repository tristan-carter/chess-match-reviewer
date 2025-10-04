#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include "MatchReview.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Basic");

    QQmlApplicationEngine engine;

    MatchReview match_review;
    engine.rootContext()->setContextProperty("match_review", &match_review);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection
        );

    engine.loadFromModule("com.tristancarter.chessmatchreviewer", "EnterMatchToReviewScreen");

    return app.exec();
}
