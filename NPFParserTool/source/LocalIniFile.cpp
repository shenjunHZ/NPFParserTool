#include "LocalIniFile.hpp"
#include "Tools/Dir.h"
#include "qinifile.h"

using namespace DSGUI;

namespace common
{
    QVariant CLocalIniFile::ReadValue(const QString &strSecName, const QString &strKey, const QString& qstrFilename)
    {
        QIniFile LocalIniFile(DSDir::GetCurrentPath() + QString("/") + qstrFilename);
        return LocalIniFile.ReadValue(strSecName, strKey);
    }

    void CLocalIniFile::WriteValue(const QString &strSecName, const QString &strKey, QVariant qvValue, const QString& qstrFilename)
    {
        QIniFile LocalIniFile(DSDir::GetCurrentPath() + QString("/") + qstrFilename);
        LocalIniFile.WriteValue(strSecName, strKey, qvValue);
    }

}