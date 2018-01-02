#include "qall_pannel_widget.h"
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QGridLayout>
#include <QLabel>


enum {
    COLUMN_DEVICE_NAME = 0, // 设备名称
    COLUMN_DEVICE_ID,       // 设备ID
    COLUMN_H2S,             // H2S测量值
    COLUMN_SO2,             // SO2测量值
	COLUMN_ELE,              //电池电量
};


QAll_pannel_widget::QAll_pannel_widget(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, true ); 
    m_pLabelUnit = new QLabel(QStringLiteral("测量单位：PPM  "), this);
    m_tableWidget = new QTableWidget(0,5, this);
    m_tableWidget->setFrameShape(QFrame::NoFrame);
    m_tableWidget->verticalHeader()->setVisible(false );
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QStringList headers;
    headers << QString::fromLocal8Bit("设备名称");
    headers << QString::fromLocal8Bit("设备ID");
    headers << QStringLiteral("H2S 测量值");
    headers << QStringLiteral("SO2 测量值");
	headers <<QString::fromLocal8Bit("电池电量");
    m_tableWidget->setHorizontalHeaderLabels(headers);
    m_tableWidget->setColumnWidth(COLUMN_DEVICE_NAME, 190);
    m_tableWidget->setColumnWidth(COLUMN_DEVICE_ID, 190);
    m_tableWidget->setColumnWidth(COLUMN_H2S, 100);
    m_tableWidget->setColumnWidth(COLUMN_SO2, 100);
	m_tableWidget->setColumnWidth(COLUMN_ELE, 100);

    QGridLayout * pLayout = new QGridLayout(this);
    pLayout->addWidget(m_pLabelUnit, 0, 0, 1, 1, Qt::AlignRight);
    pLayout->addWidget(m_tableWidget, 1, 0, 1, 1);
    pLayout->setRowStretch(0, 0);
    pLayout->setRowStretch(1, 100);
}

QAll_pannel_widget::~QAll_pannel_widget()
{

}

void QAll_pannel_widget::InitIntrumentInfo(QVector<BUTTON_INSTRUMENT> &infos )
{
    m_infos.clear();
    m_infos = infos;

    for (int i = 0, count = m_tableWidget->rowCount(); i < count; i++)
    {
        m_tableWidget->removeRow(0);
    }

    for( int i = 0; i < infos.count(); ++i )
    {
        int icount = i;
        m_tableWidget->insertRow(icount);
        m_tableWidget->setItem(icount, 0, new QTableWidgetItem(infos[i].deviceName));
        QTableWidgetItem *item = m_tableWidget->item(icount, 0);
        item->setTextAlignment(Qt::AlignCenter);

        m_tableWidget->setItem(icount, 1, new QTableWidgetItem(infos[i].deviceno));
        item = m_tableWidget->item(icount, 1);
        item->setTextAlignment(Qt::AlignCenter);
    }
}

void QAll_pannel_widget::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
}

void QAll_pannel_widget::UpdateInstrumentInfo(PANNEL_DATA st)
{
    QString strtime = QDateTime::fromMSecsSinceEpoch(st.times.toULongLong()).toString("yyyy-MM-dd hh:mm:ss");
    for( int i = 0; i < m_infos.count();  ++i)
    {
        if( m_infos[i].deviceno == st.deviceid )
        {
            QTableWidgetItem *item = NULL;
            QString msgtype; 
            // 接收到数据时，将H2S数据设置在通道1，SO2数据设置在通道2，这里根据通道号来判断是什么气体 tangqiao 2015/12/25
            if( st.probe == 1 )// H2S
            {
                item = new QTableWidgetItem(st.value);
                item->setTextAlignment(Qt::AlignCenter);
                m_tableWidget->setItem(i, COLUMN_H2S, item);
            }
            else // SO2
            {
                item = new QTableWidgetItem(st.value);
                item->setTextAlignment(Qt::AlignCenter);
                m_tableWidget->setItem(i, COLUMN_SO2, item);
            }
			item = new QTableWidgetItem( st.battery );
			item->setTextAlignment( Qt::AlignCenter);
			m_tableWidget->setItem(i, COLUMN_ELE, item );
            break;
        }
    }
}