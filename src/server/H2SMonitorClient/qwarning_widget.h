#ifndef QWARNING_WIDGET_H
#define QWARNING_WIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QMovie>
#include <QPushButton>
#include <QLabel>


typedef enum WARN_STATE
{
	WARNING_1ST_STATE,
	WARNING_2ND_STATE,
}WARN_STATE;

class QWarning_Widget: public QWidget
{
	Q_OBJECT

public:
	QWarning_Widget(WARN_STATE states,  QWidget *parent = 0);
	~QWarning_Widget();
	void set1stWarning();
	void set2ndWarning();
	void setNormal();
	void mouseReleaseEvent(QMouseEvent *);
	//void paintEvent(QPaintEvent *);
private:
	QMovie *m_mv_warn1st;
	QMovie *m_mv_warn2nd;
	WARN_STATE m_state;
	QLabel *m_lb_mv;
	bool m_isSendSignal;
	

signals:
	void clicked();
};

#endif // QWARNING_WIDGET_H
