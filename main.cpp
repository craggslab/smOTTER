#include <QtWidgets/QApplication>
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickView>
#include <QtCore/QDir>
#include <QtQml/QQmlEngine>
#include <QQuickStyle>
#include <QDebug>
#include <QString>
#include <filesystem>

#include "nidatasource.h"
#include "radialbar.h"
#include "hexplot.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    qmlRegisterType<RadialBar>("RadialBar", 1, 0, "RadialBar");
    REGISTER_HEX_PLOT_TYPES("HexPlot", 1, 0)

    app.setApplicationName("smfBox Open Acquisition");
    app.setOrganizationName("University of Sheffield, Craggs Lab");
    app.setOrganizationDomain("craggs-lab.com");

    QQuickStyle::setStyle("Material");

    QQuickView viewer;

    QObject::connect(viewer.engine(), &QQmlEngine::quit, &viewer, &QWindow::close);

    viewer.setTitle(QStringLiteral("smFRETBox Acquisition"));

    NIDataSource dataSource(&viewer);
    viewer.rootContext()->setContextProperty("dataSource", &dataSource);

    viewer.setSource(QUrl("qrc:/main.qml"));
    viewer.setResizeMode(QQuickView::SizeRootObjectToView);
    viewer.show();


    return app.exec();
}
