#include "qshowwarnmodel.h"
#include <QVariant>
QShowWarnModel::QShowWarnModel(QObject *parent)
	: QAbstractTableModel(parent)
{
	headers << QString::fromLocal8Bit("设备编号");
	headers << QString::fromLocal8Bit("设备名称");
	headers << QString::fromLocal8Bit("类型");
	headers << QString::fromLocal8Bit("时间");
	headers << QString::fromLocal8Bit("设定值");
	headers << QString::fromLocal8Bit("实测值");
	headers << QString::fromLocal8Bit("等级");
	
}

QShowWarnModel::~QShowWarnModel()
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

int QShowWarnModel::rowCount(const QModelIndex & parent) const
{
	return datas.count();
}
int QShowWarnModel::columnCount(const QModelIndex & parent ) const
{
	return headers.count();
}
QVariant QShowWarnModel::data(const QModelIndex & index, int role) const
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
		Warn_Info * warninfo = datas.at(index.row());
		switch(index.column())
		{
		case 0:
			return warninfo->m_deviceno;
		case 1:
			return warninfo->m_dName;
		case 2:
			return warninfo->m_probeid;
		case 3:
			return warninfo->m_dates;
		case 4:
			return warninfo->m_warn_set;
		case 5:
			return warninfo->m_testvl;
		case 6:
			return warninfo->m_level;
		}
	}
	return QVariant();

}
QVariant QShowWarnModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole
		&& orientation == Qt::Horizontal
		&& section < headers.count())
	{
		return headers.at(section);
	}

	return QVariant();

}
bool QShowWarnModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	beginResetModel();
	if (role == Qt::UserRole)
	{
		Warn_Info * warninfo = value.value<Warn_Info *>();

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
Qt::ItemFlags QShowWarnModel::flags(const QModelIndex &index) const
{
	return  QAbstractTableModel::flags(index);
}

void QShowWarnModel::cleanData()
{
	beginResetModel();
	while(datas.count())
	{
		Warn_Info * warninfo = datas.takeFirst();
		delete warninfo;
	}	
	endResetModel();
}