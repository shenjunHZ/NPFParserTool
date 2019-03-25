#pragma once
#include <QtWidgets/QApplication>
#include <QtCore/QSharedMemory>
#include <memory>

namespace common
{
    class AppConfig
    {
    public:
        AppConfig(QApplication& app);
        bool runOnlyOne();
        void releaseShareMemory();

    private:
        bool isRunning();
        void setLog();

    private:
        std::unique_ptr<QSharedMemory> m_sharedMenPtr;
    };
}