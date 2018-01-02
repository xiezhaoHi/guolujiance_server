#include "qrunstateinfo_widget.h"
#include <QHBoxLayout>
#include <QHeaderView>
#include <QDebug>

QRunStateInfo_Widget::QRunStateInfo_Widget(QWidget *parent)
	: QWidget(parent)
{
	m_view = new QTableView( this );
	m_model = new QStateInfoModel(this );
	m_view->setModel( m_model );
	m_view->setShowGrid( false );
	m_view->verticalHeader()->setDefaultSectionSize(15);
	m_view->verticalHeader()->setVisible(false );
	m_view->horizontalHeader()->setVisible( true );
	m_view->setColumnWidth(0, 90);
	m_view->setColumnWidth(1, 300);
	m_view->setRowHeight(0, 10);
	QHBoxLayout *layout = new QHBoxLayout( this );
	layout->addWidget( m_view );
	setLayout(layout);
}

QRunStateInfo_Widget::~QRunStateInfo_Widget()
{
	qDebug() << "QRunStateInfo_Widget release";
}
void QRunStateInfo_Widget::setMsg(QString msg)
{
	
	QStringList strlist = msg.split("#");
	Q_ASSERT( strlist.count() == 2 );
	QModelIndex index = m_model->index(0, 0, QModelIndex() );
	Run_Info *info = new Run_Info( strlist[0], strlist[1], this );               //消息在模型中出队列时析构
	m_model->setData(index, QVariant::fromValue(info), Qt::UserRole);
}
