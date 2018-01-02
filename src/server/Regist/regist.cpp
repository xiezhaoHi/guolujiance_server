#include "regist.h"
#include <QLabel>
#include <QGridLayout>
#include <QGroupBox>

Regist::Regist(QWidget *parent)
	: QMainWindow(parent)
{
	
	QLabel *lb_local = new QLabel(tr("Local Host"), this );
	m_LocalHost = new QLineEdit( this );
	QLabel *lb_regist = new QLabel(tr("Regist code"), this );
	m_RegistCode = new QLineEdit( this );
	QGridLayout *gridLayout = new QGridLayout( this );
	m_btn_regist = new QPushButton(tr("Regist"), this );
	gridLayout->addWidget( lb_local, 0, 0, 1, 1 );
	gridLayout->addWidget(m_LocalHost, 0, 1 , 1, 2 );
	gridLayout->addWidget(lb_regist, 1, 0, 1, 1 );
	gridLayout->addWidget(m_RegistCode, 1, 1, 1, 2);
	gridLayout->addWidget( m_btn_regist, 2, 2, 1, 1 );
	QGroupBox *gb_box = new QGroupBox( this );
	gb_box->setLayout( gridLayout );
	setCentralWidget( gb_box );

	connect( m_btn_regist, SIGNAL(clicked()), this, SLOT(RegistSlot()));
	
	
}

Regist::~Regist()
{

}
void Regist::RegistSlot()
{
	QString str = m_LocalHost->text();
	yxyDES2* my_des = new yxyDES2();
	//set key
	my_des->InitializeKey("12345678",0);
	//printf("key is :\n 12345678 \n\n");

	//----------------------stand test------------------------------
	//encrypt
	//printf("Test 8 bytes:\n yxyDES2. \n\n");
	QByteArray ba = str.toLatin1();
	my_des->EncryptData(ba.data(),0);

	//Ciphertext in bin
	//printf("Ciphertext in bin:\n %s\n\n",my_des->GetCiphertextInBinary());

	//Ciphertext in Hex
//	printf("Ciphertext in Hex:\n %s\n\n",my_des->GetCiphertextInHex());

	m_RegistCode->setText( my_des->GetCiphertextInHex() );

}