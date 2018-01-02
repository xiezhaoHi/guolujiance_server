#include "qregistdlg.h"
#include <QLabel>
#include <QGridLayout>
#include <QGroupBox>
#include <QDebug>
int QRegistDlg::m_createNum = 0;
QRegistDlg::QRegistDlg(QString diskid, QWidget *parent)
	: m_diskid( diskid) ,QDialog(parent)
{
	QLabel *lb_local = new QLabel(tr("Local Host"), this );
	m_LocalHost = new QLineEdit(m_diskid, this );
	QLabel *lb_regist = new QLabel(tr("Regist code"), this );
	m_RegistCode = new QLineEdit( this );
	QGridLayout *gridLayout = new QGridLayout( this );
	m_btn_regist = new QPushButton(tr("Regist"), this );
	gridLayout->addWidget( lb_local, 0, 0, 1, 1 );
	gridLayout->addWidget(m_LocalHost, 0, 1 , 1, 2 );
	gridLayout->addWidget(lb_regist, 1, 0, 1, 1 );
	gridLayout->addWidget(m_RegistCode, 1, 1, 1, 2);
	gridLayout->addWidget( m_btn_regist, 2, 2, 1, 1 );
	setLayout( gridLayout );

	connect( m_btn_regist, SIGNAL(clicked()), this, SLOT(RegistSlot()));
	resize( 300, 100 );
	m_createNum++;
	qDebug() << "QRegistDlg count:" << m_createNum;
}

QRegistDlg::~QRegistDlg()
{
	m_createNum--;
	qDebug() << "QRegistDlg count:" << m_createNum;
}
void QRegistDlg::RegistSlot()
{
	m_code = m_RegistCode->text();
	accept();
}
