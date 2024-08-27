#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QDir>
#include <QUrl>
#include <QFileInfo>
#include <QSerialPortInfo>

int main(int argc, char *argv[])
{
    qputenv("QT_SCALE_FACTOR", "1.3");

    QGuiApplication app(argc, argv);

    QDir dir(QGuiApplication::applicationDirPath());
    dir.cd("images");

    QStringList images;
    foreach (QFileInfo fileInfo, dir.entryInfoList(QDir::Files))
    {
        images << QUrl::fromLocalFile(fileInfo.canonicalFilePath()).url();
    }

    QStringList devices;
    foreach (QSerialPortInfo serialPortInfo, QSerialPortInfo::availablePorts()) {
        devices << serialPortInfo.portName();
    }

    QQuickStyle::setStyle("Fusion");
    QQmlApplicationEngine engine;
    engine.setInitialProperties({ {"images", images}, {"devices", devices} });
    const QUrl url(QStringLiteral("qrc:/BleInfrared/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
