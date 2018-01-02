#ifndef ALLWARNMSGMODEL_H
#define ALLWARNMSGMODEL_H

#include <QAbstractItemModel>
class A_Warn_Info : public QObject
{
	Q_OBJECT
public:
	A_Warn_Info(QString id,QString deviceno, QString name,QString probeid, QString dates,QString warn_val, QString test_val,QString level ,QObject *parent);
	~A_Warn_Info();
	QString m_id;
	QString m_dates;
	QString m_dName;
	QString m_deviceno;	
	QString m_probeid;
	
	QString m_warn_set;
	QString m_testvl;
	QString m_level;
	static int m_icount;
};

class AllWarnMsgModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	AllWarnMsgModel(QObject *parent);
	~AllWarnMsgModel();
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	int columnCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex & index) const;
	void cleanData();
private:
	QStringList headers;
	QVector<A_Warn_Info *> datas;
};

#endif // QSHOWWARNMODEL_H
