#include "qcheckdatamodel.h"
#include <QVariant>
#include <QDebug>
int Data_Info::m_icount = 0;
Data_Info::Data_Info(QString id,QString times, QString data, QObject *parent ):m_id( id ),m_dates(times), m_data(data),QObject(parent)
{
	m_icount++;
	if( m_icount % 100 == 0 )
	{
		qDebug() << "A_Warn_Info construct:" << m_icount;
	}
}
Data_Info::~Data_Info()
{
	m_icount--;
	if( m_icount % 100 == 0 )
	{
		qDebug() << "A_Warn_Info construct:" << m_icount;
	}
}
QCheckDataModel::QCheckDataModel(QObject *parent)
	: QAbstractTableModel(parent)
{
	headers << QString::fromLocal8Bit("序号") << QString::fromLocal8Bit("时间") << QString::fromLocal8Bit("数据");
}

QCheckDataModel::~QCheckDataModel()
{
	if( m_datas.count() != 0 )
	{
		for( int i = 0; i < m_datas.count(); ++i )
		{
			if( m_datas[i] )
			{
				delete m_datas[i];
				m_datas[i] = NULL;
			}
		}
	}
}

int QCheckDataModel::rowCount(const QModelIndex & parent) const
{
	return m_datas.count();
}
int QCheckDataModel::columnCount(const QModelIndex & parent ) const
{
	return headers.count();
}
QVariant QCheckDataModel::data(const QModelIndex & index, int role) const
{
	if( !index.isValid() )
		return QVariant();
	if (role == Qt::TextAlignmentRole)
	{
		return Qt::AlignCenter | Qt::AlignBottom;
	}
	else if ((role == Qt::DisplayRole || role == Qt::EditRole)
		&& index.row() < m_datas.count())
	{
		Data_Info * info = m_datas.at(index.row());
		switch(index.column())
		{
		case 0:
			return info->m_id;
		case 1:
			return info->m_dates;
		case 2:
			return info->m_data;
		}
	}
	return QVariant();

}

QVariant QCheckDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole
		&& orientation == Qt::Horizontal
		&& section < headers.count())
	{
		return headers.at(section);
	}

	return QVariant();

}
bool QCheckDataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	beginResetModel();
	if (role == Qt::UserRole)
	{
		Data_Info * info = value.value<Data_Info *>();

		if (index.row() >= m_datas.count())
		{
			m_datas.push_front(info);

		}
		else if (index.row() > 0)
		{
			m_datas.insert(index.row(), info);
		}
		else
		{
			m_datas.push_front(info);
		}
		endResetModel();
		return true;
	}
	return false;
}
 Qt::ItemFlags QCheckDataModel::flags(const QModelIndex & index) const
 {
	 return  QAbstractTableModel::flags(index);
 }

 void QCheckDataModel::cleanData()
 {
	 beginResetModel();
	 while(m_datas.count())
	 {
		 Data_Info * warninfo = m_datas.takeFirst();
		 delete warninfo;
	 }	
	 endResetModel();
 }
