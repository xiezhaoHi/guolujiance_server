#include "user_table_model.h"
#include "service/business_handle_service/business_model/user.h"
#include <QVariant>

#define COLUMN_USER_NAME        0
#define COLUMN_RIG_NAME         1
#define COLUMN_USER_SESSION     2
#define COLUMN_USER_LOGIN_TIME  3



CUserTableModel::CUserTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_header << QStringLiteral("用户") << QStringLiteral("井队") 
        << QStringLiteral("连接会话") << QStringLiteral("最近一次登录时间");
}

CUserTableModel::~CUserTableModel()
{

}

int CUserTableModel::rowCount(const QModelIndex &parent /*= QModelIndex() */) const
{
    int row = 0;
    for (rig_ptr ptr : m_data) {
        if (ptr) {
            row += ptr->GetUserCount();
        }
    }
    return row;
}

int CUserTableModel::columnCount(const QModelIndex &parent /*= QModelIndex() */) const
{
    return m_header.length();
}

QVariant CUserTableModel::data(const QModelIndex &index, 
                               int role /*= Qt::DisplayRole*/) const
{
    if (role == Qt::DisplayRole) {
        int row = index.row();
        user_ptr pUser;
        rig_ptr pRig_;
        int i = 0;
        for (rig_ptr pRig : m_data) {
            if (!pRig) {
                continue;
            }

            if (i + pRig->GetUserCount() > row) {
                pUser = pRig->GetUser(row - i);
                pRig_ = pRig;
                break;
            } else {
                i += pRig->GetUserCount();
            }
        }

        if (!pUser) {
            return QVariant();
        }

        switch (index.column())
        {
        case COLUMN_USER_NAME:
            return pUser->name;

        case COLUMN_RIG_NAME:
            return pRig_->name;

        case COLUMN_USER_SESSION:
            {
                tcp_session_ptr ptr;
                if (pUser && 
                    (ptr = pUser->GetSession()) != NULL) {
                        return ptr->get_display_text();
                } else {
                    return QStringLiteral("N/A");
                }
            }

        case COLUMN_USER_LOGIN_TIME:
            if (pUser && pUser->GetLoginTime()) {
                date_time_ptr ptr = pUser->GetLoginTime();
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
        if (column == COLUMN_USER_SESSION
            || column == COLUMN_USER_LOGIN_TIME) {
                return Qt::AlignCenter;
        }
    }

    return QVariant();
}

bool CUserTableModel::setData(const QModelIndex &index, const QVariant &value, 
                              int role /*= Qt::EditRole*/)
{
    return true;
}

QVariant CUserTableModel::headerData(int section, Qt::Orientation orientation, 
                                     int role /*= Qt::DisplayRole*/) const
{
    if (role == Qt::DisplayRole && 
        orientation == Qt::Horizontal) {
        if (section >= 0 && section <= m_header.size()) {
            return m_header.at(section);
        }
    }

    return QVariant();
}

Qt::ItemFlags CUserTableModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index);
}

void CUserTableModel::SetInitialData(QList<rig_ptr> & lst)
{
    disconnect(this, SLOT(UpdateUI()));

    beginResetModel();
    m_data.clear();
    m_data.append(lst);
    endResetModel();

    for (rig_ptr ptr : m_data) {
        connect(ptr.get(), SIGNAL(rigUserUpdated()), this, SLOT(UpdateUI()));
    }
}

void CUserTableModel::UpdateUI()
{
    beginResetModel();
    endResetModel();
}

