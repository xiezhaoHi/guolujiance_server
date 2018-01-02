#ifndef QSHOWWARNDLG_H
#define QSHOWWARNDLG_H

#include <QDialog>
#include <QTableView>
#include "qshowwarnmodel.h"
#include <QSqlDatabase>
#include <QMutex>

typedef enum WARNING_LEVEL
{
	ONE_LEVEL,
	TWO_LEVEL,
}WARNING_LEVEL;

class QShowWarnDlg : public QDialog
{
	Q_OBJECT

public:
	QShowWarnDlg(WARNING_LEVEL level, QWidget *parent = 0);
	~QShowWarnDlg();
	void AddMsg(QString);
	void CleanMsg();
	void setMsgdb(QSqlDatabase db, QMutex * lock);
	void setConnectCount( int icount);
	QStringList getMsg();
private:
	QTableView *m_view;
	QShowWarnModel *m_model;
	QSqlDatabase m_db;
	QMutex * m_pLockForDB;
	QStringList m_strList;
	int m_connectCount;

};

#endif // QSHOWWARNDLG_H
