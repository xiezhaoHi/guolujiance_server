#include "qshowwarndlg.h"
#include <QHBoxLayout>
#include <QDateTime>
#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

QShowWarnDlg::QShowWarnDlg(WARNING_LEVEL level, QWidget *parent)
    : QDialog(parent)
{
    if( level == ONE_LEVEL )
        setWindowTitle(QStringLiteral("一级报警"));
    else if( level == TWO_LEVEL )
        setWindowTitle(QStringLiteral("二级报警"));
    m_view = new QTableView( this );
    m_model = new QShowWarnModel( this );
    m_view->setModel( m_model );
    m_view->setColumnWidth(2, 100 );
    m_view->setColumnWidth(3, 150);
    QHBoxLayout *layout = new QHBoxLayout( this );
    layout->addWidget( m_view );
    setLayout( layout );
    resize(800, 400);
    m_connectCount = 1;

    
}

QShowWarnDlg::~QShowWarnDlg()
{
    m_pLockForDB->lock();
    if( m_strList.count() > 0 )
    {
        if( m_db.isOpen() )
        {
            QSqlQuery *m_pQuery = new QSqlQuery(m_db);
            for( int i = 0; i < m_strList.count(); ++i )
            {
                QString strSql;
                strSql  = QString::fromLocal8Bit("insert into WarnMsg value('");
                strSql += m_strList[i];
                strSql +="');";
                if( !m_pQuery->exec( strSql ))
                {
                    QSqlError error = m_pQuery->lastError();
                    QString str = error.text();
                    delete m_pQuery;
                    m_pLockForDB->unlock();
                    QMessageBox::information( this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("警报信息添加到数据库失败!"));
                    return;
                }
            }
        m_strList.clear();
        delete m_pQuery;
        }
    }
    m_pLockForDB->unlock();
}
void QShowWarnDlg::AddMsg( QString msg )
{
    m_pLockForDB->lock();
    m_strList.push_back( msg );
    m_pLockForDB->unlock();

    QStringList listinfo = msg.split("_");
    Q_ASSERT( listinfo.count() == 7 );
    QString strdates = QDateTime::fromMSecsSinceEpoch(listinfo[6].toULongLong()).toString("yyyy_MM_dd hh:mm:ss");
    
    Warn_Info *info = new Warn_Info( listinfo[0], listinfo[1],listinfo[2],strdates, listinfo[3], listinfo[4],listinfo[5],this );
    QModelIndex index = m_model->index(0, 0, QModelIndex());
    m_model->setData(index, QVariant::fromValue(info), Qt::UserRole);

    m_pLockForDB->lock();
    if( m_strList.count() > 5 )
    {
        
        if( m_db.isOpen() )
        {
            QSqlQuery *m_pQuery = new QSqlQuery(m_db);
            
            for( int i = 0; i < m_strList.count(); ++i )
            {
                QString strSql;
                strSql  = QString::fromLocal8Bit("insert into WarnMsg values('");
                strSql += m_strList[i];
                strSql += "');"; 
                if( !m_pQuery->exec( strSql ))
                {
                    QSqlError error = m_pQuery->lastError();
                    QString str = error.text();
                    delete m_pQuery;
                    m_pLockForDB->unlock();
                    QMessageBox::information( this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("警报信息添加到数据库失败!"));
                    return;
                }
            }
            m_strList.clear();
            delete m_pQuery;
        }
    }
    m_pLockForDB->unlock();

}
void QShowWarnDlg::CleanMsg()
{
    m_model->cleanData();
}
void QShowWarnDlg::setMsgdb(QSqlDatabase db, QMutex * lock)
{
    if (lock) {
        m_pLockForDB = lock;
        m_db = db;
    }
    
}
void QShowWarnDlg::setConnectCount(int icount)
{
    m_connectCount = icount;
}
QStringList QShowWarnDlg::getMsg()
{
	return m_strList;
}