#ifndef _MESSAGE_DEVICES_LIST_ITEM_H_
#define _MESSAGE_DEVICES_LIST_ITEM_H_

#include "message/i_message.h"

class CMessageDevicesListItem : public IMessage
{
public:
    CMessageDevicesListItem();
    
    virtual ~CMessageDevicesListItem();

    virtual std::size_t Length();

    virtual bool WriteToOutputByteArray(OutputByteArray & out);

    virtual bool ReadFromInputByteArray(InputByteArray & in);

    virtual QString ToString();

    void SetSampleRate(QString sampleRate);

    QString GetSampleRate();

public:
    T_MSG_DEVICES_LIST_ITEM m_struct;

private:
    QString                 m_qstrSampleRate;
};



#endif//!_MESSAGE_DEVICES_LIST_ITEM_H_