#include "allwarnmsgmodel.h"
#include <QVariant>
#include <QDebug>
int A_Warn_Info::m_icount = 0;
A_Warn_Info::A_Warn_Info(QString id,QString deviceno, QString name,QString probeid, QString dates,QString warn_val, QString test_val,QString level ,QObject *parent) 
	:m_deviceno(deviceno),m_dName(name), m_probeid(probeid), m_warn_set(warn_val),m_dates(dates), m_testvl(test_val),m_level(level),m_id( id ),QObject( parent)
{
	m_icount++;
	if( m_icount % 100 == 0 )
	{
		qDebug() << "A_Warn_Info construct:" << m_icount;
	}
}

A_Warn_Info::~A_Warn_Info()
{
	m_icount--;
	if( m_icount % 100 == 0 )
	{
		qDebug() << "A_Warn_Info construct:" << m_icount;
	}
}

AllWarnMsgModel::AllWarnMsgModel(QObject *parent)
	: QAbstractTableModel(parent)
{
	headers << QString::fromLocal8Bit("序号");
	headers << QString::fromLocal8Bit("时间");
	headers << QString::fromLocal8Bit("设备名称");
	headers << QString::fromLocal8Bit("设备编号");
	
	headers << QString::fromLocal8Bit("类型");

	headers << QString::fromLocal8Bit("设定值");
	headers << QString::fromLocal8Bit("实测值");
	headers << QString::fromLocal8Bit("报警等级");
	
}

AllWarnMsgModel::~AllWarnMsgModel()
{
	if( datas.count() != 0 )
	{
		for( int i = 0; i < datas.count(); ++i )
		{
			if( datas[i] )
			{
				delete datas[i];
				datas[i] = NULL;
			}
		}
	}
}

int AllWarnMsgModel::rowCount(const QModelIndex & parent) const
{
	return datas.count();
}
int AllWarnMsgModel::columnCount(const QModelIndex & parent ) const
{
	return headers.count();
}
QVariant AllWarnMsgModel::data(const QModelIndex & index, int role) const
{
	if( !index.isValid() )
		return QVariant();
	if (role == Qt::TextAlignmentRole)
	{
		return Qt::AlignHCenter | Qt::AlignBottom;
	}
	else if ((role == Qt::DisplayRole || role == Qt::EditRole)
		&& index.row() < datas.count())
	{
		A_Warn_Info * warninfo = datas.at(index.row());
		switch(index.column())
		{
		case 0: 
			return warninfo->m_id;
		case 1:
			return warninfo->m_dates;
			
		case 2:
			return warninfo->m_dName;
		case 3:
			return warninfo->m_deviceno;
			
		case 4:
			return warninfo->m_probeid;
		case 5:
			return warninfo->m_warn_set;
		case 6:
			return warninfo->m_testvl;
		case 7:
			return warninfo->m_level;
		}
	}
	return QVariant();

}
QVariant AllWarnMsgModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole
		&& orientation == Qt::Horizontal
		&& section < headers.count())
	{
		return headers.at(section);
	}

	return QVariant();

}
bool AllWarnMsgModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	beginResetModel();
	if (role == Qt::UserRole)
	{
		A_Warn_Info * warninfo = value.value<A_Warn_Info *>();

		if (index.row() >= datas.count())
		{
			datas.push_front(warninfo);
		}
		else if (index.row() > 0)
		{
			datas.insert(index.row(), warninfo);
		}
		else
		{
			datas.push_front(warninfo);
		}
		endResetModel();
		return true;
	}
	return false;
}
Qt::ItemFlags AllWarnMsgModel::flags(const QModelIndex &index) const
{
	return  QAbstractTableModel::flags(index);
}

void AllWarnMsgModel::cleanData()
{
	beginResetModel();
	while(datas.count())
	{
		A_Warn_Info * warninfo = datas.takeFirst();
		delete warninfo;
	}	
	endResetModel();
}