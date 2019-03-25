#pragma once
#include <memory>
#include "ParserUI.hpp"

namespace app
{
    class AppDependency
    {
    public:
        AppDependency();
        bool initAppInstance();

    private:
        std::unique_ptr<operation::ParserUI> m_ParserUI;
    };
}