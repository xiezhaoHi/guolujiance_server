#include "qconnectstate.h"
QConnectState* QConnectState::m_instance = NULL;

QConnectState::QConnectState(QObject *parent)
	: QObject(parent)
{

}

QConnectState::~QConnectState()
{

}
QConnectState *QConnectState::getInstance()
{
	if( m_instance == NULL )
	{
		m_instance = new QConnectState();
	}
	return m_instance;
}
void QConnectState::setConnectState(bool state)
{
	if( state == false )
	{
		emit disConnect();
	}
	else if( state == true )
	{
		emit connected();
	}
}