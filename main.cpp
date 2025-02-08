#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "MatchReview.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    MatchReview match_review;
    engine.rootContext()->setContextProperty("match_review", &match_review);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("ALevelNEAChessReviewer", "EnterMatchToReviewScreen");

    return app.exec();
}
