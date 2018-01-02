#ifndef FORM_RIG_DETAIL_H
#define FORM_RIG_DETAIL_H

#include <QWidget>
#include <QTableView>
#include "table_model/device_table_model.h"
#include "table_model/user_table_model.h"
#include <QLabel>
#include <QSplitter>


class CFormRigDetail : public QWidget
{
    Q_OBJECT

public:
    CFormRigDetail(QWidget *parent);
    ~CFormRigDetail();

private:
    // �����豸������ʾ��
    void BuildUI();

    // ��������
    void BeautifyUI();

    // �������Ԫ��
    void TranslateUI();

    // ���ź� 
    void BindSlots();


public:
    CDeviceTableModel * m_pDeviceTableModel;
    CUserTableModel   * m_pUserTableModel;

private:
    QSplitter         * m_pSplitter;
    QLabel            * m_pLabelDeviceHeader;
    QTableView        * m_pDeviceTableView;
    
    QLabel            * m_pLabelUserHeader;
    QTableView        * m_pUserTableView;
    
};

#endif // FORM_RIG_DETAIL_H
