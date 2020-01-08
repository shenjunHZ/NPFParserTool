#include "ParserUI.hpp"
#include "ui_ParserUI.h"
#include "LocalIniFile.hpp"
#include "commonControl.hpp"
#include "ErrorCode.hpp"
#include "LineEdit/LineEdit.h"
#include <libdsl/DStr.h>
#include <QtWidgets/QLineEdit>
#include <QtCore/QByteArray>
#include <QtGui/QDesktopServices>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#include <libdsl/DNetUtil.h>
#include <Tools/Dir.h>
#include <QtCore/QSettings>
#include <QtCore/QProcess>
#include <iphlpapi.h>

using namespace DSGUI;

namespace operation
{
    static const QRegExp regExValues("([0-9A-Ea-e,\\s]+$)");

    ParserUI::ParserUI(QWidget *parent)
        : QDialog(parent, Qt::FramelessWindowHint)
        , m_pLocalIniFile(nullptr)
        , m_bmousePressed(false)
    {
        ui.reset( new Ui::ParserUI() );
        ui->setupUi(this);
        setAttribute(Qt::WA_DeleteOnClose, false);
        setAttribute(Qt::WA_TranslucentBackground, true);
        // move to middle
        QDesktopWidget* pDeskWidget = QApplication::desktop();
        int iScreen = pDeskWidget->primaryScreen(); // use primary screen for show
        QWidget* pPrimaryScreen = pDeskWidget->screen(iScreen);

        int iWidth = pPrimaryScreen->width();
        int iHeight = pPrimaryScreen->height();
        this->move((iWidth - width()) / 2, (iHeight - height()) / 2);
        
        QString oemAppname = "NPF Parser";
        this->setWindowTitle(oemAppname);

        m_msgLabel = ui->msgLabel;
        m_msgLabel->setObjectName("msgLabel");
        m_msgLabel->hide();

        ui->leValues->setTextMargins(24, 0, 20, 0);
        ui->leValues->setPlaceholderText(tr("Please input NPF values"));
        ui->leValues->setValidator(new QRegExpValidator(regExValues, ui->leValues));
        ui->lePath->setTextMargins(24, 0, 20, 0);
         
        ui->leValues->installEventFilter(this);
        ui->lePath->installEventFilter(this);
        ui->ParserBg->installEventFilter(this);
        ui->ParserBg->setFocus();
        setTabOrder(ui->leValues, ui->lePath);

        connect(ui->convertBtn,  SIGNAL(clicked()),     this, SLOT(OnConvertNPF()));
        connect(ui->clearBtn,    SIGNAL(clicked()),     this, SLOT(OnClearInput()));
        connect(ui->btnQuit,     SIGNAL(clicked()),     this, SLOT(OnBtnQuit()));

        SET_MODULE_STYLE_BY_PATH("ParserUI");

        this->setModal(true);
        this->activateWindow();
    }

    ParserUI::~ParserUI()
    {

    }

    int ParserUI::init()
    {
        m_pLocalIniFile = std::make_unique<common::CLocalIniFile>();

        initCommSkinInfo();
        return 0;
    }

    void ParserUI::initCommSkinInfo()
    {	
        ui->leValues->setText("");
        ui->lePath->setText("");
    }

    void ParserUI::SetErrorMsg(const QString& tipMsg)
    {
        if (!this->isVisible())
        {
            QDialog::setVisible(true);
        }
        m_msgLabel->setText(tipMsg);
        if (tipMsg.isEmpty())
        {
            m_msgLabel->hide();
        }
        else
        {
            m_msgLabel->setText(tipMsg);
            m_msgLabel->show();
        }
    }

    void ParserUI::UpdateLoginState(bool bLogin)
    {
        if (bLogin)
        {
            ui->ParserBg->setFocus();
        }
    }

    bool ParserUI::eventFilter(QObject *obj, QEvent *event)
    {
        if (obj == ui->ParserBg)
        {
            if (event->type() == QEvent::MouseButtonPress)
            {
                QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
                if (mouseEvent->button() == Qt::LeftButton)
                {
                    m_mousePos = mouseEvent->globalPos();
                    m_bmousePressed = true;
                    return true;
                }
            }
            else if (event->type() == QEvent::MouseMove)
            {
                QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
                if ((mouseEvent->buttons() & Qt::LeftButton) && m_bmousePressed)
                {
                    int xDis = mouseEvent->globalPos().x() - m_mousePos.x();
                    int yDis = mouseEvent->globalPos().y() - m_mousePos.y();
                    m_mousePos = mouseEvent->globalPos();
                    QPoint pos = mapToGlobal(QPoint(0,0));
                    move(pos.x() + xDis,pos.y() + yDis);
                    return true;
                }
            }
            else if (event->type() == QEvent::MouseButtonRelease)
            {
                if(m_bmousePressed)
                {
                    ui->ParserBg->setFocus();
                }
                m_bmousePressed = false;

                QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
                if (mouseEvent->button() == Qt::LeftButton)
                {
                    return true;
                }
            }
        }
        else if (obj == ui->leValues)
        {
            if (QEvent::FocusIn == event->type())
            {
                ui->leValues->setPlaceholderText("");
            }
            else if (QEvent::FocusOut == event->type() && ui->leValues->text().isEmpty())
            {
                ui->leValues->setPlaceholderText(tr("Please input NPF values"));
            }
        }

        return QDialog::eventFilter(obj, event);
    }

    void ParserUI::moveEvent(QMoveEvent *event)
    {
        const int margin = 20;
        QPoint pt = event->pos();
        QRect rtWnd = QApplication::desktop()->availableGeometry(pt);
        if (pt.x()+this->width()<=margin)
        {
            pt = QPoint(-this->width()+margin, pt.y());
        }
        if (pt.x()>rtWnd.right()-margin)
        {
            pt = QPoint(rtWnd.right()-margin, pt.y());
        }
        if (pt.y()+this->height()<=margin)
        {
            pt = QPoint(pt.x(), -this->height()+margin);
        }
        if (pt.y()>rtWnd.bottom()-margin)
        {
            pt = QPoint(pt.x(), rtWnd.bottom()-margin);
        }
        move(pt);
    }

    void ParserUI::OnConvertNPF()
    {
        SetErrorMsg("");
        QString values = ui->leValues->text().trimmed(); // rid space at head and tail

        if (values.isEmpty())
        {
            SetErrorMsg(tr("NPF value can not be Empty"));
            ui->leValues->setFocus();
        }

        convertNPF(values);
        //this->accept();
    }

    void ParserUI::convertNPF(QString& value)
    {
        if (!value.isEmpty())
        {
            QStringList values = value.split(",");
            std::vector<uint8_t> vecValues;
            for (const auto& val : values)
            {
                vecValues.push_back(val.toInt());
            }
            if (vecValues.size())
            {
                std::string path(vecValues.cbegin(), vecValues.cend());
                ui->lePath->setText(QString::fromStdString(path));
            }
        }
    }

    void ParserUI::OnBtnQuit()
    {
        UpdateLoginState(false);
        this->accept();
    }

    void ParserUI::OnClearInput()
    {
        QWidget *obj = (QWidget*)sender();
        ui->leValues->clear();
        ui->lePath->clear();
        SetErrorMsg("");
    }

} // namespace operation