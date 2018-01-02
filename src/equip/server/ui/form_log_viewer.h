#ifndef FORM_LOG_VIEWER_H
#define FORM_LOG_VIEWER_H

#include <QWidget>
#include <QStringList>
#include <QList>
#include <QMutex>
#include <QAbstractItemModel>
#include "service/log_service/log_service.h"

class QGroupBox;
class QCheckBox;
class QListWidget;
class QListView;
class QStringListModel;


typedef struct _T_LEVEL_LOG_PAIR
{
    E_LOG_LEVEL level;
    QString     log;

    _T_LEVEL_LOG_PAIR(E_LOG_LEVEL level, QString log)
    {
        this->level = level;
        this->log   = log;
    }

    _T_LEVEL_LOG_PAIR(const _T_LEVEL_LOG_PAIR & val)
    {
        this->level = val.level;
        this->log   = val.log;
    }

    _T_LEVEL_LOG_PAIR & operator= (const _T_LEVEL_LOG_PAIR & val)
    {
        this->level = val.level;
        this->log   = val.log;
        return *this;
    }

}T_LEVEL_LOG_PAIR;

// 日志查看器
class CFormLogViewer : public QWidget, public ILogObserver
{
    Q_OBJECT

public:
    
    CFormLogViewer(QWidget *parent);

    ~CFormLogViewer();

    // 构建日志视图显示区
    void BuildUI();

    // 美化界面
    void BeautifyUI();

    // 翻译界面元素
    void TranslateUI();

    // 绑定各种事件、信号处理函数
    void BindEventHandlers();

    // 初始化界面数据
    void InitUIData();

    virtual void HandleLog(E_LOG_LEVEL level, const QString & log);

    virtual void timerEvent(QTimerEvent *);

    virtual QSize sizeHint() const;

signals:

    void LogSignal(/*E_LOG_LEVEL level,*/ const QString & log);

public slots:

    void LogSlot(/*E_LOG_LEVEL level, */const QString & log);

    void UpdateLogLevel();

private:
    QWidget                 * m_pWidgetFilters;
    QGroupBox               * m_pGroupBox;
    QCheckBox               * m_pCheckBoxShowDebugLevel;    // 调试级别日志显示开关
    QCheckBox               * m_pCheckBoxShowInfoLevel;     // 信息级别日志显示开关
    QCheckBox               * m_pCheckBoxShowWarningLevel;  // 警告级别日志显示开关
    QCheckBox               * m_pCheckBoxShowErrorLevel;    // 错误级别日志显示开关
    QListWidget             * m_pListWidgetLogView;         // 日志显示区

    QMutex                    m_mutex;
    QList<T_LEVEL_LOG_PAIR> * m_listCache;
    QList<T_LEVEL_LOG_PAIR> * m_listUI;

    int                       m_nTimerID;
};



#endif // FORM_LOG_VIEWER_H
