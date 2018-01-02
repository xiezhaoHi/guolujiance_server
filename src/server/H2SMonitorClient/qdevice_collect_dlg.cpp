#include "qdevice_collect_dlg.h"
#include <QGridLayout>
#include <QDebug>
#include <QPainter>
int QDevice_Collect_dlg::m_irecord = 0;
QDevice_Collect_dlg::QDevice_Collect_dlg(QWidget *parent)
	: QDialog(parent)
{
	setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint&~Qt::WindowMinimizeButtonHint &~Qt::WindowCloseButtonHint);
	setWindowTitle(QString::fromLocal8Bit("设备校准"));
	m_lb_id = new QLabel(QString::fromLocal8Bit("设备ID："), this );
	QLabel *lb_h2s = new QLabel( QString::fromLocal8Bit("探头1：H2S"), this );
	QLabel *lb_so2 = new QLabel( QString::fromLocal8Bit("探头2：SO2"), this );
	QImage image;
	image.load(":/login/images/login/bt_collect_zero.png");
	m_bt_Zero_h2s = new QCustomButton( image, this );
	m_bt_Zero_so2 = new QCustomButton( image, this );
	image.load(":/login/images/login/bt_collect_full.png");
	m_bt_full_h2s = new QCustomButton( image, this );
	m_bt_full_so2 = new QCustomButton( image, this );

	image.load(":/login/images/login/bt_collect_close.png");
	m_bt_close = new QCustomButton(image, this );

	/*m_bt_Zero_h2s->setMinimumHeight(40);
	m_bt_Zero_h2s->setMaximumWidth(80);
	m_bt_full_h2s->setMinimumHeight(40);
	m_bt_full_h2s->setMaximumWidth(80);

	m_bt_Zero_so2->setMinimumHeight(40);
	m_bt_Zero_so2->setMaximumWidth(80);
	m_bt_full_so2->setMinimumHeight(40);
	m_bt_full_so2->setMaximumWidth(80);
	QGridLayout *layout = new QGridLayout( this );
	layout->addWidget( m_lb_id , 0, 0, 1, 1 );
	layout->addWidget( lb_h2s, 1, 0, 1, 1 );
	layout->addWidget( lb_so2, 2, 0, 1, 1 );
	layout->addWidget( m_bt_Zero_h2s, 1, 1, 1, 1 );
	layout->addWidget( m_bt_full_h2s, 1, 2, 1, 1 );
	layout->addWidget( m_bt_Zero_so2, 2, 1, 1, 1 );
	layout->addWidget( m_bt_full_so2, 2, 2, 1, 1 );*/
	setFixedSize( 588, 384 );
	//setLayout( layout );
	QRect rc = geometry();
	m_lb_id->setGeometry(50, 50, 150, 30);
	lb_h2s->setGeometry(50, 120, 100, 30);
	lb_so2->setGeometry(50, 200, 100, 30);
	m_bt_Zero_h2s->setGeometry(150, 120, 101, 38);
	m_bt_Zero_so2->setGeometry(150, 200, 101, 38);
	m_bt_full_h2s->setGeometry(350, 120, 101, 38);
	m_bt_full_so2->setGeometry(350, 200, 101, 38);
	m_bt_close->setGeometry(250, 300, 100, 34);

	m_irecord++;
	connect( m_bt_Zero_h2s, SIGNAL(clicked()), this, SLOT(H2StoZeroSlot()));
	connect( m_bt_Zero_so2, SIGNAL(clicked()), this, SLOT(SO2toZeroSlot()));
	connect( m_bt_full_h2s, SIGNAL(clicked()), this, SLOT(H2StoFullSlot()));
	connect( m_bt_full_so2, SIGNAL(clicked()), this, SLOT(SO2toFullSlot()));
	connect( m_bt_close, SIGNAL(clicked()), this, SLOT(close()));

	qDebug() << "QDevice_Collect_dlg constructor:" << m_irecord;
}

QDevice_Collect_dlg::~QDevice_Collect_dlg()
{
	m_irecord--;
	qDebug() << "QDevice_Collect_dlg constructor:" << m_irecord;
}
void QDevice_Collect_dlg::H2StoZeroSlot()
{
	setZeroCollect();
}
void QDevice_Collect_dlg::H2StoFullSlot()
{
	setFullCollect();
}
void QDevice_Collect_dlg::SO2toFullSlot()
{
	setFullCollect();
}
void QDevice_Collect_dlg::SO2toZeroSlot()
{
	setZeroCollect();
}
void QDevice_Collect_dlg::setCommId(QString id)
{
	m_commid = id;
	m_port = new QSerialPort(m_commid, this );
	if( !m_port->open(QIODevice::ReadWrite) )
	{
		QMessageBox box(QMessageBox::Question,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("串口打开失败，请确认是否已打开！"));
		QPushButton *okButton = (box.addButton(QString::fromLocal8Bit("确定"),QMessageBox::AcceptRole));		
		box.exec();
		QDialog::reject();
	}
	else
	{
		connect( m_port, SIGNAL(readyRead()), this, SLOT(ReadCommInfo()));
	}
}
void QDevice_Collect_dlg::ReadCommInfo()
{
	QByteArray ba = m_port->readAll();
	QString msg = QString::fromLocal8Bit("设备ID:");
	msg += ba.data();
	qDebug() << msg;
	m_lb_id->setText( msg );	
}
void QDevice_Collect_dlg::setZeroCollect()
{

}
void QDevice_Collect_dlg::setFullCollect()
{

}
void QDevice_Collect_dlg::paintEvent(QPaintEvent *e)
{
	QRect rc = geometry();
	QPainter painter( this );
	QImage image;
	image.load(":/login/images/login/collect_main.png");
	painter.drawImage(QRect(0, 0, rc.width(), rc.height()), image);

}