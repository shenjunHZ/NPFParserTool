#include <QtWidgets/QApplication>
#include "AppConfig.hpp"
#include "AppDependency.hpp"

int main(int argc, char* argv[])
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);
    common::AppConfig appConfig(app);
    if (appConfig.runOnlyOne())
    {
        return 1;
    }
    app::AppDependency appDependency;
    appDependency.initAppInstance();

   // app.exec();
    appConfig.releaseShareMemory();

    return 0;
}