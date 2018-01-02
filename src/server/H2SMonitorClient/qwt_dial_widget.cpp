#include "qwt_dial_widget.h"


Qwt_Dial_Widget::Qwt_Dial_Widget(QWidget *parent)
	: QWidget(parent)
{
	m_needle = NULL;
	m_dial = new QwtDial( this );
	
	QPalette palette;
	//范围
	m_dial->setScale(-10, 10 );
	//角度
	m_dial->setScaleArc(0, 270.0 );
	//将100分为10格
	m_dial->setScaleMaxMajor(10);
	//再将第一小格分为2小格
	m_dial->setScaleMaxMinor( 2 );
	m_dial->setOrigin( 135.0 );
	QColor needleColor( Qt::red );
	//QColor baseNeedle( QColor( Qt::darkGray ).dark( 300 ) );
//	m_needle = new QwtCompassMagnetNeedle   ( QwtCompassMagnetNeedle  ::Style::ThinStyle , baseNeedle);
	//m_needle->setWidth( 5 );
	//m_dial->setNeedle( m_needle );
	

	QwtDialSimpleNeedle *needle = new QwtDialSimpleNeedle(
		QwtDialSimpleNeedle::Arrow, true, needleColor,
		QColor( Qt::gray ).light( 130 ) );
	//设置颜色
	m_dial->setNeedle( needle );
	//m_dial->setMode(QwtDial::RotateScale);

    
	m_dial->setLineWidth( 4 );
	//设置边框凹陷
	QColor base( QColor( Qt::darkGray ).dark( 150 ) );
	m_dial->setFrameShadow( QwtDial::Sunken );
	palette.setColor( QPalette::Base, base );
	palette.setColor( QPalette::Window, base.dark( 150 ) );
	palette.setColor( QPalette::Mid, base.dark( 110 ) );
	palette.setColor( QPalette::Light, base.light( 170 ) );
	palette.setColor( QPalette::Dark, base.dark( 170 ) );
	palette.setColor( QPalette::Text, Qt::green);
	palette.setColor( QPalette::WindowText, base.dark( 200 ) );

	m_dial->setPalette( palette );
	m_lcd_value= new QLCDNumber( this );
	m_lcd_value->setSmallDecimalPoint(true);
	//10进制模式
	m_lcd_value->setDecMode();
	//可显示10位
	m_lcd_value->setDigitCount(10);
	//设置字体颜色

	m_lcd_value->setSegmentStyle(QLCDNumber::Flat);
	palette = m_lcd_value ->palette();
	palette.setColor(QPalette::Normal,QPalette::WindowText,Qt::red);
	m_lcd_value->setPalette(palette);
	//设置背景颜色
	// m_lcd_value->setStyleSheet("background-color: white");

	m_lb_probe = new QLabel("xx", this );
	m_lb_ID = new QLabel( "xx",this );
	m_lb_name = new QLabel( "xx",this );
	QRect thisRect = geometry();
	m_dial->setGeometry(thisRect.x(), thisRect.y(), 100, 100 );
	m_dial->show();
	m_lcd_value->setGeometry(120, 10, 100, 30 );
	m_lb_probe->setGeometry( 120, 35, 200, 30 );
	m_lb_name->setGeometry( 120, 50, 200, 30 );
	m_lb_ID->setGeometry( 120, 65, 200, 30 );

	resize( 300, 200 );

	connect( m_dial, SIGNAL(valueChanged(double)), this, SLOT(setNum(double)));
	//setNum( m_dial->value() );
}

Qwt_Dial_Widget::~Qwt_Dial_Widget()
{

}

void Qwt_Dial_Widget::setNum(double f )
{
	m_lcd_value->display( f );
}

void Qwt_Dial_Widget::setInfo(PANNEL_DATA2 st )
{
	m_dial->setValue( st.value );
	m_lb_ID->setText( st.strNo );
	m_lb_name->setText( st.strName );
	m_lb_probe->setText( QString::fromLocal8Bit("探头%1").arg(st.probe ));
}

void Qwt_Dial_Widget::UpdateValue(double f)
{
	m_dial->setValue( f );
}
void Qwt_Dial_Widget::setDialParam(double max )
{
	m_dial->setScale(0, max );
	//角度
	m_dial->setScaleArc(0, 270.0 );
	//将100分为10格
	m_dial->setScaleMaxMajor(10);
	//再将第一小格分为2小格
	m_dial->setScaleMaxMinor( 2 );
}
void Qwt_Dial_Widget::deleteDial()
{

}
