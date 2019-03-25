#include "qinifile.h"

#include <QtCore/QStringList>

#define INDEX(SEC,KEY) QString("%1/%2").arg(SEC).arg(KEY)

namespace common
{

QIniFile::QIniFile(const QString &strIniFilePath)
{
    m_settings = new QSettings(strIniFilePath,QSettings::IniFormat);
    m_settings->setIniCodec("UTF-8");
}

QIniFile::~QIniFile(void)
{
    if(m_settings)
        delete m_settings;
}

QVariant QIniFile::ReadValue(const QString &strSecName, const QString &strKey)
{
    QVariant ret = m_settings->value(INDEX(strSecName,strKey));
    if(ret.type() == QVariant::StringList)
    {
        QString strRet;
        QStringList lstGet = ret.toStringList();
        QStringListIterator it(lstGet);
        while(it.hasNext())
        {
            strRet += it.next();
            if(it.hasNext())
                strRet += ",";
        }
        return QVariant(strRet);
    }
    else
    {
        return ret;
    }
}

void QIniFile::WriteValue(const QString &strSecName, const QString &strKey, const QVariant& qvValue)
{
    m_settings->setValue(INDEX(strSecName,strKey), qvValue);
    m_settings->sync();
}
}