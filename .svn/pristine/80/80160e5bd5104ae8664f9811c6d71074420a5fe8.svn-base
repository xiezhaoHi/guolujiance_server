#ifndef QCONNECTSTATE_H
#define QCONNECTSTATE_H

#include <QObject>

class QConnectState : public QObject
{
	Q_OBJECT

public:
	QConnectState(QObject *parent=0);
	~QConnectState();
	static QConnectState* getInstance();
	void setConnectState(bool);

private:
	static QConnectState *m_instance;
signals:
	void connected();
	void disConnect();
	
};

#endif // QCONNECTSTATE_H
