#include "form_rig_detail.h"
#include <QVBoxLayout>

CFormRigDetail::CFormRigDetail(QWidget *parent)
    : QWidget(parent)
{
    BuildUI();
    BindSlots();
}

CFormRigDetail::~CFormRigDetail()
{

}

void CFormRigDetail::BuildUI()
{
    QVBoxLayout * pLayoutSelf = new QVBoxLayout();
    pLayoutSelf->setMargin(0);
    pLayoutSelf->addSpacing(5);

    m_pSplitter = new QSplitter(Qt::Vertical, this);
    m_pSplitter->setContentsMargins(0, 0, 0, 0);

    QWidget * pWidgetTop = new QWidget(this);
    QVBoxLayout * pLayoutOfWidgetTop = new QVBoxLayout(this);
    pLayoutOfWidgetTop->setMargin(0);

    m_pLabelDeviceHeader = new QLabel(this);
    m_pLabelDeviceHeader->setText(QStringLiteral("井队设备"));
    pLayoutOfWidgetTop->addWidget(m_pLabelDeviceHeader);

    m_pDeviceTableView = new QTableView(this);
    m_pDeviceTableModel = new CDeviceTableModel(m_pDeviceTableView);
    m_pDeviceTableView->setModel(m_pDeviceTableModel);
    m_pDeviceTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pDeviceTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pDeviceTableView->setColumnWidth(0, 120);
    m_pDeviceTableView->setColumnWidth(1, 120);
    m_pDeviceTableView->setColumnWidth(2, 120);
    m_pDeviceTableView->setColumnWidth(6, 170);
    m_pDeviceTableView->setColumnWidth(7, 170);
    pLayoutOfWidgetTop->addWidget(m_pDeviceTableView);
    pLayoutOfWidgetTop->addSpacing(5);

    pWidgetTop->setLayout(pLayoutOfWidgetTop);
    m_pSplitter->addWidget(pWidgetTop);

    QWidget * pWidgetBottom = new QWidget(this);
    QVBoxLayout * pLayoutOfWidgetBottom = new QVBoxLayout();
    pLayoutOfWidgetBottom->setMargin(0);

    pLayoutOfWidgetBottom->addSpacing(5);
    m_pLabelUserHeader = new QLabel(this);
    m_pLabelUserHeader->setText(QStringLiteral("井队人员"));
    pLayoutOfWidgetBottom->addWidget(m_pLabelUserHeader);

    m_pUserTableView = new QTableView(this);
    m_pUserTableModel = new CUserTableModel(m_pUserTableView);
    m_pUserTableView->setModel(m_pUserTableModel);
    m_pUserTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pUserTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pUserTableView->setColumnWidth(0, 120);
    m_pUserTableView->setColumnWidth(1, 120);
    m_pUserTableView->setColumnWidth(2, 170);
    m_pUserTableView->setColumnWidth(3, 170);
    pLayoutOfWidgetBottom->addWidget(m_pUserTableView);
    pWidgetBottom->setLayout(pLayoutOfWidgetBottom);
    m_pSplitter->addWidget(pWidgetBottom);

    pLayoutSelf->addWidget(m_pSplitter);
    this->setLayout(pLayoutSelf);

    BeautifyUI();
    TranslateUI();
}

void CFormRigDetail::BeautifyUI()
{

}

void CFormRigDetail::TranslateUI()
{

}

void CFormRigDetail::BindSlots()
{

}
