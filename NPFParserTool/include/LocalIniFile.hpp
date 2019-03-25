#pragma once

#include <QtCore/QVariant>
#include <QtCore/QString>


namespace common
{
    class CLocalIniFile
    {
    public:
        /* @fn				从程序目录的ini文件获取参数
        * @brief
        * @param <IN>		qstrSecName			参数归类名称
        strKey				参数名称
        qstrFilename		目标 ini文件名称
        * @param <OUT>
        * @return none.	QVariant			文件的参数数据  失败：	QVariant.isValid() return false
        * @date 			2015/7/2 16:04
        */
        static QVariant ReadValue(const QString &qstrSecName, const QString &strKey, const QString& qstrFilename);
        /* @fn				从程序目录的ini文件获取参数
        * @brief
        * @param <IN>		qstrSecName			参数归类名称
        strKey				参数名称
        qvValue				写入的数据
        qstrFilename		目标 ini文件名称
        * @param <OUT>
        * @return none.	QVariant			文件的参数数据  失败：	QVariant.isValid() return false
        * @date 			2015/7/2 16:04
        */
        static void WriteValue(const QString &strSecName, const QString &strKey, QVariant qvValue, const QString& qstrFilename);
    };
}




