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
    // 构建设备详情显示区
    void BuildUI();

    // 美化界面
    void BeautifyUI();

    // 翻译界面元素
    void TranslateUI();

    // 绑定信号 
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
