#ifndef QSTATEBUTTON_H
#define QSTATEBUTTON_H

#include <QWidget>
#include <QImage>
class QStateButton : public QWidget
{
	Q_OBJECT

public:
	QStateButton(QImage mages,QImage mage2 ,QWidget *parent = 0);
	~QStateButton();
	QImage m_image1;
	QImage m_image2;
	bool m_isEnable;
	void setStateEnable( bool);
private:
	void paintEvent(QPaintEvent *);
	void mouseReleaseEvent(QMouseEvent *);
signals:
	void clicked();
};

#endif // QSTATEBUTTON_H
