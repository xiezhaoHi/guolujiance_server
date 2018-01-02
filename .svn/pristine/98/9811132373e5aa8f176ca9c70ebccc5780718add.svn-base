#ifndef QALL_PANNEL_WIDGET_
#define QALL_PANNEL_WIDGET_

#include <QWidget>
#include "qstatebutton.h"
#include <QPushButton>
#include "qshow_struct.h"
#include <QVector>
//#include "qdialwidget.h"
#include<QTableWidget>
#include <QTableWidget>
//#include <qwt_dial.h>

class QLabel;

class QAll_pannel_widget : public QWidget
{
    Q_OBJECT

public:
    QAll_pannel_widget(QWidget *parent = 0);
    ~QAll_pannel_widget();
    /*
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);*/
    void InitIntrumentInfo(QVector<BUTTON_INSTRUMENT> &infos );
    void UpdateInstrumentInfo(PANNEL_DATA st );
    void resizeEvent(QResizeEvent *e);
    //void cleanAllPanel();
//	QAllPanelTableWidget *CreateTableWidget();
signals:
    void mouseDown(QPoint mosuePos);
    void mouseUp(QPoint mosuePos);
    void mouseMove(QPoint mosuePos);
private:
//	QVector<QDialWidget*> m_dial;
    QVector<BUTTON_INSTRUMENT> m_infos;
    QVector<PANNEL_DATA> m_datas;
    int m_icount;
    QTableWidget *m_tableWidget;
    QLabel       * m_pLabelUnit;// ≤‚¡ø÷µ
};

#endif // QINNERWIDGET_H
