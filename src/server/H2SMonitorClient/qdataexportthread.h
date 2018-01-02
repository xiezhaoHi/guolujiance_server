#ifndef QDATAEXPORTTHREAD_H
#define QDATAEXPORTTHREAD_H

#include <QThread>
#include <QSqlDatabase>
#include <QMutex>
#include "qshow_struct.h"
#include <QFile>
#include <QDate>
#include "qshow_struct.h"
#include <QVector>
#include "qwt_symbol.h"
#include "qwt_legend.h"
#include "qwt_plot_grid.h"
#include "qwt_plot_curve.h"
#include "qwt_scale_map.h"
#include "qwt_plot.h"
#include <qwt_scale_draw.h>
#include <QSqlError>
#include <QSqlQuery>

class QDataExportThread : public QThread
{
    Q_OBJECT

public:
    QDataExportThread(QSqlDatabase db, QObject *parent);
    ~QDataExportThread();
   void setDate(QDateTime Startdatetime,QDateTime Stoptime, int AveTm );
    void GetDataInfo(QVector<EXPORT_IMAGE_INFO> &vt, bool isave = false);
    void setIsExportInfo(bool, bool ,bool );

    // 设置导出目录
    void SetExportDir(const QString & exportDir);

private:
    void run();
    void AveExportCalculator();

    // 获取导出目录
    QString GetExportDir();
	void H2SAndSO2();

signals:
    void QueryDataCompelete();
    void ExportTestImage();
    void ExportAveImage();

private:
    QString     m_qstrExportDir;// 数据导出目录
    bool m_isImage;
    bool m_isave;
    bool m_istest;
    QSqlDatabase m_db;
    QMutex m_mutex;
    QDateTime m_Starttime;
	QDateTime m_Stoptime;
    QVector<QStringList> m_dataInfo;
    QVector<QStringList> m_aveDataInfo;
    QVector<EXPORT_IMAGE_INFO> m_test_imageInfo;
    QVector<EXPORT_IMAGE_INFO> m_ave_imageInfo;
	QVector<QStringList> m_H2SdataInfo;
	QVector<QStringList> m_SO2aveDataInfo;
	QVector<QStringList> m_h2sAndSo2Info;
	
    int m_aveTime;
};

#endif // QDATAEXPORTTHREAD_H
