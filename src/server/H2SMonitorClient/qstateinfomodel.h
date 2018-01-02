#ifndef QSTATEINFOMODEL_H
#define QSTATEINFOMODEL_H

#include <QAbstractTableModel>
class Run_Info: public QObject
{
	Q_OBJECT
public:
		Run_Info(QString id, QString msg, QObject *parent ):m_id(id), m_msg( msg ),QObject(parent){};
		~Run_Info(){};
		QString m_id;
		QString m_msg;
};
class QStateInfoModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	QStateInfoModel(QObject *parent);
	~QStateInfoModel();
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	int columnCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	 Qt::ItemFlags flags(const QModelIndex & index) const;

private:
	QStringList headers;
	QVector<Run_Info*> m_datas;
	
};

#endif // QSTATEINFOMODEL_H
