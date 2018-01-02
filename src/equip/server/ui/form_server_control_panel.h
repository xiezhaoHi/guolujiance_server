#ifndef FORM_SERVER_CONTROL_PANEL_H
#define FORM_SERVER_CONTROL_PANEL_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QDateTime>

// ��������״̬�л���
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

    // ���ź�
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
    QTimer        * m_pTimerUpdateTotalRunTime;  // �����ۼƳ�������ʱ��
    qint64          m_timeStartup;              // ��������ʱ��
};

#endif // FORM_SERVER_CONTROL_PANEL_H
