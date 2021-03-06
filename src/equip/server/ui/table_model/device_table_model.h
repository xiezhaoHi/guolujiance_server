#ifndef DEVICE_TABLE_MODEL_H
#define DEVICE_TABLE_MODEL_H

#include <QAbstractTableModel>
#include <QStringList>
#include <QList>
#include "service/business_handle_service/business_model/rig.h"



class CDeviceTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    CDeviceTableModel(QObject *parent);
    ~CDeviceTableModel();

    virtual int rowCount(const QModelIndex &parent = QModelIndex() ) const;

    virtual int columnCount(const QModelIndex &parent = QModelIndex() ) const;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

public slots:

    void SetInitialData(QList<rig_ptr> & lst);

    void UpdateUI();

private:
    QStringList         m_header;
    QList<rig_ptr>      m_data;
};

#endif // DEVICE_TABLE_MODEL_H
