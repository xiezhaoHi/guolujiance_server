#ifndef QCHECKDATAMODEL_H
#define QCHECKDATAMODEL_H

#include <QAbstractTableModel>
class Data_Info: public QObject
{
	Q_OBJECT
public:
		Data_Info(QString id,QString times, QString data, QObject *parent );
		~Data_Info();
		QString m_id;
		QString m_dates;
		QString m_data;
		static int m_icount;
};
class QCheckDataModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	QCheckDataModel(QObject *parent);
	~QCheckDataModel();
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	int columnCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	 Qt::ItemFlags flags(const QModelIndex & index) const;
	  void cleanData();

private:
	QStringList headers;
	QVector<Data_Info*> m_datas;
	
};

#endif // QSTATEINFOMODEL_H
