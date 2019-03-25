#pragma once
#include <QtWidgets/QDialog>
#include <QtGui/QPalette>
#include <QtCore/QString>
#include <QtGui/QMouseEvent>
#include <QtGui/QImage>
#include <QtGui/QBitmap>
#include <QtGui/QPixmap>
#include <QtGui/QIcon>
#include <QtCore/QThread>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <memory>

namespace Ui 
{
    class ParserUI;
};
namespace common
{
    class CLocalIniFile;
}

namespace operation
{
    class NoticeFrame : public QFrame
    {
        Q_OBJECT

        Q_PROPERTY(bool notice READ notice WRITE setNotice)
    public:
        NoticeFrame(QWidget*parent = 0):QFrame(parent)
            ,m_bNotice(false)
        {
        }
        bool notice() const{return m_bNotice;}
        void setNotice(bool n){m_bNotice = n;}
    private:
        bool  m_bNotice;
    };

    class ParserUI : public QDialog
    {
        Q_OBJECT

    public:
        ParserUI(QWidget *parent = 0);
        ~ParserUI();

        int init();
        void initCommSkinInfo();

    protected:
        virtual bool eventFilter(QObject *obj, QEvent *event);
        virtual void moveEvent(QMoveEvent *event);

    private slots:
        void OnConvertNPF();
        void OnBtnQuit();

        void OnClearInput();

    private:
        void SetErrorMsg(const QString& tipMsg);  
        void UpdateLoginState(bool bLogin);
        void convertNPF(QString& value);

    private:
        std::unique_ptr<Ui::ParserUI>					ui;
        std::unique_ptr<common::CLocalIniFile>          m_pLocalIniFile;

        QLabel*                                         m_msgLabel;
        NoticeFrame*									frUserName;
        NoticeFrame*									frUserPwd;
        NoticeFrame*									frAddress;
        NoticeFrame*									frPort;

        QPoint											m_mousePos;
        bool											m_bmousePressed; 
    };
}

