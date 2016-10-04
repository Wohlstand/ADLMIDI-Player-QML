#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "player/midi_player.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    MidiPlayer midiPlayer;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("midiPlayer", &midiPlayer);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
