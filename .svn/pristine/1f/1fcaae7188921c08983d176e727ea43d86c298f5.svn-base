#ifndef FORM_REGISTRATION_H
#define FORM_REGISTRATION_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QDialog>



class CFormRegistration : public QDialog
{
    Q_OBJECT

public:
    CFormRegistration(QWidget *parent = NULL);
    ~CFormRegistration();

    // 
    void BuildUI();

    // 
    void BeautifyUI();


    void TranslateUI();


    void BindSlots();


    // œ‘ æ∂‘ª∞øÚ
    void ShowRegistDialog(QString serialNumber);

private slots:

    void ButtonApplyClicked();

    void UpdateButtonState();

private:
    QLabel      * m_pLabelSerialNumber;
    QLineEdit   * m_pLineEditSerialNumber;
    QLabel      * m_pLabelRegistrationCode;
    QTextEdit   * m_pTextEditRegistrationCode;
    QPushButton * m_pButtonApply;
};

#endif // FORM_REGISTRATION_H
