#ifndef QSHOWWARNMODEL_H
#define QSHOWWARNMODEL_H

#include <QAbstractItemModel>
class Warn_Info : public QObject
{
    Q_OBJECT
public:
    Warn_Info(QString deviceno, QString name,QString probeid, QString dates,QString warn_val, QString test_val,QString level, QObject *parent):
    m_deviceno(deviceno),m_dName(name), m_probeid(probeid), m_warn_set(warn_val),m_dates(dates), m_testvl(test_val),m_level(level),QObject( parent){};
    ~Warn_Info(){};
    QString m_deviceno;
    QString m_dName;
    QString m_probeid;
    QString m_dates;
    QString m_warn_set;
    QString m_testvl;
    QString m_level;
};

class QShowWarnModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    QShowWarnModel(QObject *parent);
    ~QShowWarnModel();
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex & index) const;
    void cleanData();
private:
    QStringList headers;
    QVector<Warn_Info *> datas;
};

#endif // QSHOWWARNMODEL_H
