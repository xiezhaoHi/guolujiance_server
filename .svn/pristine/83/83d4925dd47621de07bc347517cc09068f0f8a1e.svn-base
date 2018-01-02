#ifndef MESSAGE_HEADER_H
#define MESSAGE_HEADER_H

#include "protocol/protocol_cmd.h"
#include "protocol/protocol_structs.h"
#include "i_message.h"

using namespace std;

class CMessageHeader : public IMessage
{
public:

    CMessageHeader();

    virtual ~CMessageHeader();

    virtual std::size_t Length();

    virtual bool WriteToOutputByteArray(OutputByteArray & out);

    virtual bool ReadFromInputByteArray(InputByteArray & in);

    bool IsValid() const;

    virtual QString ToString();

public:
    T_MESSAGE_HEADER    m_struct;
};

#endif // MESSAGE_HEADER_H
