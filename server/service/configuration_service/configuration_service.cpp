#include "configuration_service.h"
#include "state_collector/path_collector.h"
#include <QSettings>
#include <QDir>
#include <QFile>


static unsigned short UNSHORT_TCP_LISTENING_PORT             = 17024;
static unsigned int   UN_DEVICE_MAX_IDLE_TIME                = 600;// ��10������δ�յ��豸���͵��κ����ݣ��������Ͽ����豸֮���socket���� tangqiao 2016/05/17
static QString        QSTR_DB_CON_STR                        = QStringLiteral("DRIVER={SQL SERVER};SERVER=182.131.2.216,1335\\SQL2005;DATABASE=GASMonitor");
static QString        QSTR_DB_USER                           = QStringLiteral("sa");
static QString        QSTR_DB_PASSWORD                       = QStringLiteral("rtmc@@508001");
static QString        QSTR_DB_HOSTNAME                       = QStringLiteral("182.131.2.216");
static int            N_DB_PORT                              = 1335;
static bool           BOOL_DB_CONFIG_ENCRYPTED               = false;
static QString        QSTR_TYPECODE_OF_PORTABLE_H2S_DETECTOR = QStringLiteral("623D151B-7927-4CA0-B513-140BE84F4B22");
static QString        QSTR_TYPECODE_OF_STATIC_H2S_DETECTOR   = QStringLiteral("88360BA6-D0C7-4EC2-8DAB-906426623363");
static int            N_LOG_LEVEL                            = 1;

// ���ý�
static QString SECTION_TCP_SERVER_OPTIONS   = QStringLiteral("TcpServer");
static QString SECTION_DB_OPTIONS           = QStringLiteral("DB");
static QString SECTION_LOG_OPTIONS          = QStringLiteral("Log");
static QString SECTION_DETECTOR_OPTIONS     = QStringLiteral("Detector");

// ���ü�
static QString KEY_TCP_LISTENING_PORT               = QStringLiteral("ListeningPort");
static QString KEY_DEVICE_CONN_MAX_IDLE_TIME        = QStringLiteral("DevConnMaxIdleTime");
static QString KEY_DB_CON_STR                       = QStringLiteral("ConStr");
static QString KEY_DB_USER                          = QStringLiteral("DBUser");
static QString KEY_DB_PASSWORD                      = QStringLiteral("DBPassword");
static QString KEY_DB_HOSTNAME                      = QStringLiteral("DBHostName");
static QString KEY_DB_PORT                          = QStringLiteral("DBPort");
static QString KEY_DB_ACCOUNT_ENCRYPT               = QStringLiteral("DBConfigEncrypted");
static QString KEY_TYPECODE_OF_PORTABLE_DETECTOR    = QStringLiteral("TypeCodeOfPortableDetector");
static QString KEY_TYPECODE_OF_SATIC_DETECTOR       = QStringLiteral("TypeCodeOfStaticDetector");
static QString KEY_LOG_LEVEL                        = QStringLiteral("LogLevel");

CConfigrationService * CConfigrationService::m_pInstance = NULL;


CConfigrationService::CConfigrationService()
{
    m_eLogLevel = LEVEL_DEBUG;
    m_unPort = UNSHORT_TCP_LISTENING_PORT;
    m_unDevConnMaxIdleTime = UN_DEVICE_MAX_IDLE_TIME;
    m_qstrDBConnStr = QSTR_DB_CON_STR;
    m_qstrDBUser = QSTR_DB_USER;
    m_qstrDBPassword = QSTR_DB_PASSWORD;
    m_qstrDBHostName = QSTR_DB_HOSTNAME;
    m_nDBPort = N_DB_PORT;
    m_qstrTypeCodeOfPortableH2SDetector = QSTR_TYPECODE_OF_PORTABLE_H2S_DETECTOR;
    m_qstrTypeCodeOfStaticH2SDetector = QSTR_TYPECODE_OF_STATIC_H2S_DETECTOR;
}

CConfigrationService::~CConfigrationService()
{

}

CConfigrationService * CConfigrationService::GetInstance()
{
    if (!m_pInstance) {
        m_pInstance = new CConfigrationService();
        m_pInstance->LoadConfiguration();
    }
    return m_pInstance;
}

void CConfigrationService::DestroyInstance()
{
    if (m_pInstance) {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

int CConfigrationService::StartInternalService()
{
    // �������ļ��м�������
    
    return 0;
}

int CConfigrationService::StopInternalService()
{
    // TODO ����ǰ���ø��µ������ļ�
    return 0;
}

int CConfigrationService::NotifyServiceToStop()
{
    return 0;
}

QString CConfigrationService::GetServiceName()
{
    return QStringLiteral("configration service");
}

E_LOG_LEVEL CConfigrationService::GetLogLevel()
{
    return m_eLogLevel;
}

unsigned short CConfigrationService::GetPort()
{
    return m_unPort;
}

int CConfigrationService::GetDevConnMaxIdleTime()
{
    return m_unDevConnMaxIdleTime;
}

QString CConfigrationService::GetDBConnStr()
{
    return m_qstrDBConnStr;
}

QString CConfigrationService::GetDBUser()
{
    return m_qstrDBUser;
}

QString CConfigrationService::GetDBPassword()
{
    return m_qstrDBPassword;
}

QString CConfigrationService::GetDBHostName()
{
    return m_qstrDBHostName;
}

int CConfigrationService::GetDBPort()
{
    return m_nDBPort;
}

QString CConfigrationService::GetTypeCodeOfPortableH2SDetector()
{
    return m_qstrTypeCodeOfPortableH2SDetector;
}

QString CConfigrationService::GetTypeCodeOfStaticH2SDetector()
{
    return m_qstrTypeCodeOfStaticH2SDetector;
}

void CConfigrationService::LoadConfiguration()
{
    // �ж������ļ��Ƿ���ڣ�������ʱ����Ĭ������
    QString qstrIniFile(CPathCollector::m_strAppDirPath + QDir::separator() + CPathCollector::m_strIniFileName);
    QFile iniFile(qstrIniFile);
    if (!iniFile.exists()) {
        QSettings setting(qstrIniFile, QSettings::IniFormat);
        setting.setValue("/" + SECTION_TCP_SERVER_OPTIONS + "/" + KEY_TCP_LISTENING_PORT, UNSHORT_TCP_LISTENING_PORT);
        setting.setValue("/" + SECTION_TCP_SERVER_OPTIONS + "/" + KEY_DEVICE_CONN_MAX_IDLE_TIME, UN_DEVICE_MAX_IDLE_TIME);

        setting.setValue("/" + SECTION_DB_OPTIONS + "/" + KEY_DB_CON_STR, QSTR_DB_CON_STR);
        setting.setValue("/" + SECTION_DB_OPTIONS + "/" + KEY_DB_HOSTNAME, QSTR_DB_HOSTNAME);
        setting.setValue("/" + SECTION_DB_OPTIONS + "/" + KEY_DB_PORT, N_DB_PORT);
        setting.setValue("/" + SECTION_DB_OPTIONS + "/" + KEY_DB_USER, QSTR_DB_USER);
        setting.setValue("/" + SECTION_DB_OPTIONS + "/" + KEY_DB_PASSWORD, QSTR_DB_PASSWORD);
        setting.setValue("/" + SECTION_DB_OPTIONS + "/" + KEY_DB_ACCOUNT_ENCRYPT, BOOL_DB_CONFIG_ENCRYPTED);

        setting.setValue("/" + SECTION_DETECTOR_OPTIONS + "/" + KEY_TYPECODE_OF_PORTABLE_DETECTOR, QSTR_TYPECODE_OF_PORTABLE_H2S_DETECTOR);
        setting.setValue("/" + SECTION_DETECTOR_OPTIONS + "/" + KEY_TYPECODE_OF_SATIC_DETECTOR, QSTR_TYPECODE_OF_STATIC_H2S_DETECTOR);

        setting.setValue("/" + SECTION_LOG_OPTIONS + "/" + KEY_LOG_LEVEL, N_LOG_LEVEL);
    }

    // ��ȡ���� 
    QSettings setting(QStringLiteral("H2SMonitorServer.ini"), QSettings::IniFormat);

    m_unPort = setting.value("/" + SECTION_TCP_SERVER_OPTIONS + "/" + KEY_TCP_LISTENING_PORT, UNSHORT_TCP_LISTENING_PORT).toInt();
    m_unDevConnMaxIdleTime = setting.value("/" + SECTION_TCP_SERVER_OPTIONS + "/" + KEY_DEVICE_CONN_MAX_IDLE_TIME, UN_DEVICE_MAX_IDLE_TIME).toInt();

    m_qstrDBConnStr = setting.value("/" + SECTION_DB_OPTIONS + "/" + KEY_DB_CON_STR, QSTR_DB_CON_STR).toString();
    m_qstrDBHostName = setting.value("/" + SECTION_DB_OPTIONS + "/" + KEY_DB_HOSTNAME, QSTR_DB_HOSTNAME).toString();
    m_nDBPort = setting.value("/" + SECTION_DB_OPTIONS + "/" + KEY_DB_PORT, N_DB_PORT).toInt();
    m_qstrDBUser = setting.value("/" + SECTION_DB_OPTIONS + "/" + KEY_DB_USER, QSTR_DB_USER).toString();
    m_qstrDBPassword = setting.value("/" + SECTION_DB_OPTIONS + "/" + KEY_DB_PASSWORD, QSTR_DB_PASSWORD).toString();

    m_qstrTypeCodeOfPortableH2SDetector = setting.value("/" + SECTION_DETECTOR_OPTIONS + "/" + KEY_TYPECODE_OF_PORTABLE_DETECTOR, QSTR_TYPECODE_OF_PORTABLE_H2S_DETECTOR).toString();
    m_qstrTypeCodeOfStaticH2SDetector = setting.value("/" + SECTION_DETECTOR_OPTIONS + "/" + KEY_TYPECODE_OF_SATIC_DETECTOR, QSTR_TYPECODE_OF_STATIC_H2S_DETECTOR).toString();

    m_eLogLevel = (E_LOG_LEVEL)(LEVEL_NONE + setting.value("/" + SECTION_LOG_OPTIONS + "/" + KEY_LOG_LEVEL, N_LOG_LEVEL).toInt());

    bool encrypted = setting.value("/" + SECTION_DB_OPTIONS + "/" + KEY_DB_ACCOUNT_ENCRYPT, BOOL_DB_CONFIG_ENCRYPTED).toBool();
    if (!encrypted) { // �����ݿ��û���������ʹ��base64���룬�Դﵽ�򵥵ؼ��ܵ�Ŀ��
        QByteArray ba = m_qstrDBUser.toLocal8Bit().toBase64();
        setting.setValue("/" + SECTION_DB_OPTIONS + "/" + KEY_DB_USER, QString(ba));

        ba = m_qstrDBPassword.toLocal8Bit().toBase64();
        setting.setValue("/" + SECTION_DB_OPTIONS + "/" + KEY_DB_PASSWORD, QString(ba));
        setting.setValue("/" + SECTION_DB_OPTIONS + "/" + KEY_DB_ACCOUNT_ENCRYPT, true);
    } else { // ת��Ϊ����
        QByteArray ba = QByteArray::fromBase64(m_qstrDBUser.toLocal8Bit());
        m_qstrDBUser = QString(ba);

        ba = QByteArray::fromBase64(m_qstrDBPassword.toLocal8Bit());
        m_qstrDBPassword = QString(ba);
    }
}

void CConfigrationService::SetLogLevel(E_LOG_LEVEL level)
{
    if (level == m_eLogLevel || level == LEVEL_NONE) {
        return;
    } else {
        QString qstrIniFile(CPathCollector::m_strAppDirPath + QDir::separator() 
            + CPathCollector::m_strIniFileName);
        QSettings setting(qstrIniFile, QSettings::IniFormat);
        setting.setValue("/" + SECTION_LOG_OPTIONS + "/" + KEY_LOG_LEVEL, (int)level);
    }
    this->m_eLogLevel = level;
}
