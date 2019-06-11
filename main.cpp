#include <QtWidgets/QApplication>
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickView>
#include <QtCore/QDir>
#include <QtQml/QQmlEngine>
#include <QQuickStyle>
#include <QDebug>
#include <QString>
#include <QSplashScreen>
#include <filesystem>

#include "nidatasource.h"
#include "radialbar.h"
#include "hexplot.h"
#include "photonarrivalgraph.h"
#include "thorcamsource.h"
#include "byteimage.h"
#include "zstage.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    qmlRegisterType<RadialBar>("RadialBar", 1, 0, "RadialBar");
    qmlRegisterType<ByteImage>("ByteImage", 1, 0, "ByteImage");
    REGISTER_HEX_PLOT_TYPES("HexPlot", 1, 0)
    REGISTER_PHOTON_ARRIVAL_PLOT_TYPES("PhotonArrivalGraph", 1, 0)

    app.setApplicationName("smfBox Open Acquisition");
    app.setOrganizationName("University of Sheffield, Craggs Lab");
    app.setOrganizationDomain("craggs-lab.com");

    QQuickStyle::setStyle("Material");

    QQuickView viewer;

    QObject::connect(viewer.engine(), &QQmlEngine::quit, &viewer, &QWindow::close);

    viewer.setTitle(QStringLiteral("smFRETBox Acquisition"));

    NIDataSource dataSource(&viewer);
    ThorCam::ThorCamSource thorcamSource(&viewer);
    ZStage stage(&viewer);

    viewer.rootContext()->setContextProperty("dataSource", &dataSource);
    viewer.rootContext()->setContextProperty("thorcam", &thorcamSource);
    viewer.rootContext()->setContextProperty("mainWindow", &viewer);
    viewer.rootContext()->setContextProperty("zStage", &stage);

    viewer.setSource(QUrl("qrc:/AppLoader.qml"));
    viewer.setResizeMode(QQuickView::SizeRootObjectToView);
    viewer.show();

    return app.exec();
}
