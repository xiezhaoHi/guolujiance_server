#ifndef FORM_DEVICE_DETAIL_H
#define FORM_DEVICE_DETAIL_H

#include <QWidget>
#include <QTableView>
#include <QList>
#include "service/business_handle_service/business_model/rig.h"
#include "table_model/device_table_model.h"
#include "ui/form_rig_detail.h"


class CFormRigDeviceDetail : public QWidget
{
    Q_OBJECT

public:
    CFormRigDeviceDetail(QWidget *parent);
    ~CFormRigDeviceDetail();

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
    CFormRigDetail       * m_pFormRigDetail;
};

#endif // FORM_DEVICE_DETAIL_H
