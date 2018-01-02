#include "qpasswd_dlg.h"
#include <QGridlayout>
#include <QMessageBox>
#include <QDebug>
QString g_user = "admin";
QString g_pwd = "h2sclient";
int QPasswd_dlg::m_irecord = 0;
QPasswd_dlg::QPasswd_dlg(QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle(QString::fromLocal8Bit("�������׼�˺�"));
	QLabel *lb_user = new QLabel(QString::fromLocal8Bit("�û�����"), this );
	QLabel *lb_pwd = new QLabel(QString::fromLocal8Bit("�� �룺"), this );
	m_lne_user = new QLineEdit( this );
	m_lne_pwd = new QLineEdit( this );
	m_lne_pwd->setEchoMode(QLineEdit::Password);
	m_bt_sure = new QPushButton( QString::fromLocal8Bit("ȷ��"), this );
	m_bt_cancel = new QPushButton( QString::fromLocal8Bit("ȡ��"), this );
	QGridLayout *layout = new QGridLayout( this );
	layout->addWidget( lb_user, 0, 0, 1, 1 );
	layout->addWidget( lb_pwd, 1, 0, 1, 1 );
	layout->addWidget( m_lne_user, 0, 1, 1, 2 );
	layout->addWidget( m_lne_pwd, 1, 1, 1, 2 );
	layout->addWidget( m_bt_sure, 2, 1, 1, 1 );
	layout->addWidget( m_bt_cancel, 2, 2, 1, 1 );
	setLayout( layout );
	setFixedSize( 300, 150 );
	connect( m_bt_cancel, SIGNAL(clicked()), this, SLOT(CancelSlot()));
	connect( m_bt_sure, SIGNAL(clicked()), this, SLOT(SureSlot()));
	m_irecord++;
	qDebug() << "qpasswd_dlg construct:" << m_irecord;
}

QPasswd_dlg::~QPasswd_dlg()
{
	m_irecord--;
	qDebug() << "qpasswd_dlg construct:" << m_irecord;
}
void QPasswd_dlg::CancelSlot()
{
	QDialog::reject();
}
void QPasswd_dlg::SureSlot()
{
	QString user = m_lne_user->text();
	QString pwd = m_lne_pwd->text();
	if( user.compare(g_user ) == 0 )
	{
		if( pwd.compare( g_pwd ) == 0 )
		{
			QDialog::accept();
		}
		else
		{
			QMessageBox box(QMessageBox::Question,QString::fromLocal8Bit("��ʾ"),QString::fromLocal8Bit("��׼���벻��ȷ�����������룡"));
			QPushButton *okButton = (box.addButton(QString::fromLocal8Bit("ȷ��"),QMessageBox::AcceptRole));		
			box.exec();
			return;
		}
	}
	else
	{
		QMessageBox box(QMessageBox::Question,QString::fromLocal8Bit("��ʾ"),QString::fromLocal8Bit("��׼�û�������ȷ�����������룡"));
		QPushButton *okButton = (box.addButton(QString::fromLocal8Bit("ȷ��"),QMessageBox::AcceptRole));		
		box.exec();
		return;
	}
}