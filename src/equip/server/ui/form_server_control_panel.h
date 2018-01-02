#ifndef FORM_SERVER_CONTROL_PANEL_H
#define FORM_SERVER_CONTROL_PANEL_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QDateTime>

// 服务运行状态切换区
class CFormServerControlPanel : public QWidget
{
    Q_OBJECT

public:
    CFormServerControlPanel(QWidget *parent);
    ~CFormServerControlPanel();

private:
    // 
    void BuildUI();

    // 
    void BeautifyUI();

    // 
    void TranslateUI();

    // 绑定信号
    void BindSlots();

public:

    void serviceStartInterrupted();

signals:
    void SwitchSerivceStatus(bool startService);

private slots:

    void SwitchServiceStatus();

    void UpdateTotalRunTime();

private:
    void ReloadQSS();

private:
    QLabel        * m_pLabelDurationOfServicePrefix;
    QLabel        * m_pLabelDurationOfService;
    QPushButton   * m_pButtonServiceStateSwitch;
    QTimer        * m_pTimerUpdateTotalRunTime;  // 更新累计持续运行时间
    qint64          m_timeStartup;              // 服务启动时间
};

#endif // FORM_SERVER_CONTROL_PANEL_H
