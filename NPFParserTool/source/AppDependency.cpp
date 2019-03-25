#include "AppDependency.hpp"

namespace app
{
    AppDependency::AppDependency()
        : m_ParserUI{nullptr}
    {

    }

    bool AppDependency::initAppInstance()
    {
        m_ParserUI = std::make_unique<operation::ParserUI>();
        m_ParserUI->init();
        int dlgCode = m_ParserUI->exec();

        if (QDialog::Rejected == dlgCode)
        {
            return false;
        }

        return true;
    }

} // namespace app