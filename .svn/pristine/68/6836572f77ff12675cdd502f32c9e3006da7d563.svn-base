#include "device_table_model.h"
#include "service/business_handle_service/business_model/device.h"
#include "service/business_handle_service/business_model/rig.h"
#include "service/socket_service/tcp_session.h"

#define COLUMN_DEVICE_NAME          0
#define COLUMN_DEVICE_CODE          1
#define COLUMN_RIG_NAME             2
#define COLUMN_DEVICE_WARNING_1ST   3
#define COLUMN_DEVICE_WARNING_2ND   4
#define COLUMN_DEVICE_MAX_RANGE     5
#define COLUMN_DEVICE_SESSION       6
#define COLUMN_DEVICE_LOGIN_TIME    7


CDeviceTableModel::CDeviceTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_header << QStringLiteral("设备") << QStringLiteral("编号")
        << QStringLiteral("井队")
        << QStringLiteral("一级报警值") << QStringLiteral("二级报警值")
        << QStringLiteral("最大值")
        << QStringLiteral("连接会话") << QStringLiteral("最近一次登录时间");
}

CDeviceTableModel::~CDeviceTableModel()
{

}

int CDeviceTableModel::rowCount(const QModelIndex &parent /*= QModelIndex() */) const
{
    int row = 0;
    for (rig_ptr ptr : m_data) {
        if (ptr) {
            row += ptr->GetDeviceCount();
        }
    }
    return row;
}

int CDeviceTableModel::columnCount(const QModelIndex &parent /*= QModelIndex() */) const
{
    return m_header.length();
}

QVariant CDeviceTableModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
    if (role == Qt::DisplayRole) {
        int row = index.row();
        device_ptr pDevice;
        rig_ptr pRig_;
        int i = 0;
        for (rig_ptr pRig : m_data) {
            if (!pRig) {
                continue;
            }

            if (i + pRig->GetDeviceCount() > row) {
                pDevice = pRig->GetDevice(row - i);
                pRig_ = pRig;
                break;
            } else {
                i += pRig->GetDeviceCount();
            }
        }

        if (!pDevice) {
            return QVariant();
        }

        switch (index.column())
        {
        case COLUMN_DEVICE_NAME:
            return pDevice->name;

        case COLUMN_DEVICE_CODE:
            return pDevice->code;

        case COLUMN_RIG_NAME:
            return pRig_->name;

        case COLUMN_DEVICE_WARNING_1ST:
            return pDevice->warningLine1st;

        case COLUMN_DEVICE_WARNING_2ND:
            return pDevice->warningLine2nd;

        case COLUMN_DEVICE_MAX_RANGE:
            return pDevice->maxRange;

        case COLUMN_DEVICE_SESSION:
            {
                tcp_session_ptr ptr;
                if (pDevice && 
                    (ptr = pDevice->GetSession()) != NULL) {
                        return ptr->get_display_text();
                } else {
                    return QStringLiteral("N/A");
                }
            }

        case COLUMN_DEVICE_LOGIN_TIME:
            if (pDevice && pDevice->GetLoginTime()) {
                date_time_ptr ptr = pDevice->GetLoginTime();
                return ptr->daysTo(QDateTime::currentDateTime()) > 0 ? 
                    ptr->toLocalTime()
                    .toString(QStringLiteral("yyyy-MM-dd HH:mm:ss.zzz"))
                    : ptr->toLocalTime()
                    .toString(QStringLiteral("HH:mm:ss.zzz"));
            } else {
                return QStringLiteral("N/A");
            }

        default:
            return QStringLiteral("N/A");
        }
    } else if (role == Qt::TextAlignmentRole) {
        int column = index.column();
        if (column == COLUMN_DEVICE_SESSION
            || column == COLUMN_DEVICE_LOGIN_TIME) {
                return Qt::AlignCenter;
        }
    }

    return QVariant();
}

bool CDeviceTableModel::setData(const QModelIndex &index, const QVariant &value, int role /*= Qt::EditRole*/)
{
    return true;
}

QVariant CDeviceTableModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
    if (role == Qt::DisplayRole
        && orientation == Qt::Horizontal
        && section < m_header.length()) {
        return m_header.at(section);
    } else if (role == Qt::DisplayRole
        && orientation == Qt::Vertical
        && section < m_header.length()) {
        return section + 1;
    } else {
        return QVariant();
    }
}

Qt::ItemFlags CDeviceTableModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index);
}

void CDeviceTableModel::SetInitialData(QList<rig_ptr> & lst)
{
    disconnect(this, SLOT(UpdateUI()));

    beginResetModel();
    m_data.clear();
    m_data.append(lst);
    endResetModel();

    for (rig_ptr ptr : m_data) {
        connect(ptr.get(), SIGNAL(rigDeviceUpdated()), this, SLOT(UpdateUI()));
    }
}

void CDeviceTableModel::UpdateUI()
{
    beginResetModel();
    endResetModel();
}
