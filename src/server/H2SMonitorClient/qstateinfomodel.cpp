#include "qstateinfomodel.h"
#include <QVariant>

QStateInfoModel::QStateInfoModel(QObject *parent)
	: QAbstractTableModel(parent)
{
	headers << QStringLiteral("来源") << QStringLiteral("消息");
}

QStateInfoModel::~QStateInfoModel()
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

int QStateInfoModel::rowCount(const QModelIndex & parent) const
{
	return m_datas.count();
}
int QStateInfoModel::columnCount(const QModelIndex & parent ) const
{
	return headers.count();
}
QVariant QStateInfoModel::data(const QModelIndex & index, int role) const
{
	if( !index.isValid() )
		return QVariant();
	if (role == Qt::TextAlignmentRole)
	{
		return Qt::AlignLeft | Qt::AlignBottom;
	}
	else if ((role == Qt::DisplayRole || role == Qt::EditRole)
		&& index.row() < m_datas.count())
	{
		Run_Info * info = m_datas.at(index.row());
		switch(index.column())
		{
		case 0:
			return info->m_id;
		case 1:
			return info->m_msg;
		}
	}
	return QVariant();

}

QVariant QStateInfoModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole
		&& orientation == Qt::Horizontal
		&& section < headers.count())
	{
		return headers.at(section);
	}

	return QVariant();

}
bool QStateInfoModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	beginResetModel();
	if (role == Qt::UserRole)
	{
		Run_Info * info = value.value<Run_Info *>();

		if (index.row() >= m_datas.count())
		{
			m_datas.push_front(info);
			if( m_datas.count() > 200)
			{
				Run_Info *tmp = m_datas.takeLast();
				delete tmp;
				tmp = NULL;
			}
		}
		else if (index.row() > 0)
		{
			m_datas.insert(index.row(), info);
			if( m_datas.count() > 200)
			{
				Run_Info *tmp = m_datas.takeLast();
				delete tmp;
				tmp = NULL;
			}
		}
		else
		{
			m_datas.push_front(info);
			if( m_datas.count() > 200)
			{
				Run_Info *tmp = m_datas.takeLast();
				delete tmp;
				tmp = NULL;
			}
		}
		endResetModel();
		return true;
	}
	return false;
}
 Qt::ItemFlags QStateInfoModel::flags(const QModelIndex & index) const
 {
	 return  QAbstractTableModel::flags(index);
 }
