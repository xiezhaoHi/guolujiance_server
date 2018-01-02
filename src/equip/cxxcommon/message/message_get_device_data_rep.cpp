#include "message_get_device_data_rep.h"
#include "message/message_header.h"



CMessageGetDeviceDataRep::CMessageGetDeviceDataRep()
{
    static_assert(sizeof(T_MSG_GET_DEVICE_DATA_REP) == 8, "sizeof(T_MSG_GET_DEVICE_DATA_REP) != 8");
    memset(&m_struct, 0x00, sizeof(m_struct));
}

CMessageGetDeviceDataRep::~CMessageGetDeviceDataRep()
{
    for (CMessageDeviceDataItem * item : m_lstDeviceData)
    {
        if (item)
        {
            delete item;
        }
    }
    m_lstDeviceData.clear();
}

std::size_t CMessageGetDeviceDataRep::Length()
{
    std::size_t size = sizeof(m_struct);
    for (CMessageDeviceDataItem * item : m_lstDeviceData)
    {
        size += item->Length();
    }
    return size;
}

bool CMessageGetDeviceDataRep::WriteToOutputByteArray(OutputByteArray & out)
{
    out.WriteU32(m_struct.pageIndex);
    out.WriteU32(m_struct.rowsOfCurrentPage);
    for (int i = 0; i < m_struct.rowsOfCurrentPage; i++)
    {
        m_lstDeviceData.at(i)->WriteToOutputByteArray(out);
    }
    return true;
}

bool CMessageGetDeviceDataRep::ReadFromInputByteArray(InputByteArray & in)
{
    m_struct.pageIndex = in.ReadU32();
    m_struct.rowsOfCurrentPage = in.ReadU32();

    m_lstDeviceData.clear();
    for (int i = 0; i < m_struct.rowsOfCurrentPage; i++)
    {
        CMessageDeviceDataItem * item = new CMessageDeviceDataItem();
        item->ReadFromInputByteArray(in);
        m_lstDeviceData.push_back(item);
    }
    return true;
}

QString CMessageGetDeviceDataRep::ToString()
{
    QString qstr;
    QTextStream stream(&qstr);

    stream << "{ ";
    stream << "[ pageIndex:" << m_struct.pageIndex << "], ";
    stream << "[ dataCountOfCurrentPage:" << m_struct.rowsOfCurrentPage << "] ";
    stream << "}";

    return qstr;
}

IMessage * CMessageGetDeviceDataRep::CreateFromHeader(const CMessageHeader & header)
{
    if (header.m_struct.bodyLength >= sizeof(T_MSG_GET_DEVICE_DATA_REP)) {
        return new CMessageGetDeviceDataRep();
    }
    return NULL;
}
