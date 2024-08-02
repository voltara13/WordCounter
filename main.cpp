#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QTextCodec>

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN) && QT_VERSION_CHECK(5, 6, 0) <= QT_VERSION && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));

    QApplication app(argc, argv);
    QQuickStyle::setStyle("Universal");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qt/qml/testapp/qml-components/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
