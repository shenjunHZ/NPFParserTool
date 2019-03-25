#ifndef QINIFILE_H
#define QINIFILE_H

#include <QtCore/QString>
#include <QtCore/QSettings>
#include <QtCore/QVariant>

namespace common
{
    class QIniFile
    {
    public:
        QIniFile(const QString &strIniFilePath);
        ~QIniFile(void);
        QVariant ReadValue(const QString &strSecName, const QString &strKey);
        void WriteValue(const QString &strSecName, const QString &strKey, const QVariant& qvValue);

    private:
        QSettings *m_settings;
    };

}
#endif // QINIFILE_H
