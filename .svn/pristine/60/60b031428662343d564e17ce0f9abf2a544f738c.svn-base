#include "qphotoshowwidget.h"
#include "qwt_symbol.h"
#include "qwt_legend.h"
#include <QHBoxLayout>
#include "qwt_plot_grid.h"
#include <QWidget>

QwtScaleMap xMap;
QwtScaleMap yMap;
QPhotoShowWidget::QPhotoShowWidget(QWidget *parent)
    : QWidget(parent)
{
    m_xmin = 0; m_xmax = 0;
    m_iCurvcnt = 0;
    d_curves = NULL;
    m_plot = new QwtPlot( this );
    m_plot->setCanvasBackground(Qt::white);

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->enableXMin( true );
    grid->enableYMin( true );
    grid->setMinorPen(Qt::gray, 1, Qt::DashLine );
    grid->attach( m_plot );

    int i;
    xMap.setScaleInterval( -0.5, 10.5 );
    yMap.setScaleInterval( -1.1, 1.1 );
//	m_xvalue.resize(180);
    
    /*for( i = 0; i < POINT_COUNT; i++ )
    {
        m_xvalue[i]=i;
    }*/
    QwtLegend *legend = new QwtLegend;
    //设置可以点击
    //legend->setDefaultItemMode( QwtLegendData::Checkable );
    m_plot->insertLegend( legend, QwtPlot::LeftLegend );


    m_plot->setAxisScaleDraw(QwtPlot::xBottom,new TimeScaleDraw( ) );
    m_plot->setAxisLabelRotation( QwtPlot::xBottom, -50.0 );
    m_plot->setAxisLabelAlignment( QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom );
    //m_plot->setAxisScale(QwtPlot::xBottom, 0, 180, 20 );
    //m_plot->setAxisLabelRotation( QwtPlot::xBottom, -50.0 );
    m_bt_changeDevice = new QPushButton(QString::fromLocal8Bit("切换设备"), this );
    m_currentDevice = new QLabel(QString::fromLocal8Bit("当前设备："), this );
    m_currentDevice->setGeometry(30, 490, 250, 30 );
    m_bt_changeDevice->setGeometry(600, 490, 100, 30 );
    m_plot->setGeometry(0, 0, 750, 480 );
    connect( m_bt_changeDevice, SIGNAL(clicked()), this, SLOT(ChangeDeviceSlot()));
}

QPhotoShowWidget::~QPhotoShowWidget()
{
    if( d_curves )
    {
        delete[] d_curves;
        d_curves = NULL;
    }
    qDebug() << "QPhotoShowWidget release";
}
void QPhotoShowWidget::ChangeDeviceSlot()
{
    emit clicked();
}
void QPhotoShowWidget::setCurvcnt( QString str, int icount, QVector<BUTTON_INSTRUMENT> &vt_btinfo)
{
    Q_ASSERT(vt_btinfo.count()*2 == icount);
    if( d_curves )
    {
        delete[] d_curves;
        d_curves = NULL;
    }
    QString msg = QString::fromLocal8Bit("当前设备：");
    msg += str;
    m_currentDevice->setText( msg );
//	if( icount != yvalue.count() )
    //	return;
    QColor pens[12] = {  Qt::red, Qt::green, Qt::blue, Qt::cyan, Qt::magenta,
        Qt::yellow, Qt::darkRed, Qt::darkGreen, Qt::darkBlue, Qt::darkCyan,
        Qt::darkMagenta, Qt::darkYellow};
    m_iCurvcnt = icount;
    m_yvalue.resize(m_iCurvcnt );
    m_data.resize( m_iCurvcnt );
    m_plot->setAxisScale( QwtPlot::xBottom,
        0, 0);
    d_curves = new QwtPlotCurve[icount];

    for( int i = 0; i < icount; ++i )
    {
        d_curves[i].setPen( pens[i % 12] );
        d_curves[i].setStyle( QwtPlotCurve::Lines );
        //d_curves[i].setCurveAttribute( QwtPlotCurve::Fitted );

        if( i % 2 == 0)
        {
            msg = QString::fromLocal8Bit("H2S_");
            msg += vt_btinfo[i/2].deviceno;
            d_curves[i].setTitle(msg);
        }
        else 
        {
            msg = QString::fromLocal8Bit("SO2_");
            msg += vt_btinfo[i/2].deviceno;
            d_curves[i].setTitle(msg);
        }
        d_curves[i].attach( m_plot );
        d_curves[i].setSamples( m_data[i].x_value, m_data[i].y_value );		
    }
    m_plot->replot();
}
void QPhotoShowWidget::setHideChangeDevice()
{
    m_bt_changeDevice->hide();
}
/*
void QPhotoShowWidget::UpdateCurvcnt(QVector<double> yvalue )
{
    if( yvalue.count() != m_iCurvcnt )
        return;
    for( int i = 0; i < m_iCurvcnt; i++ )
    {
        m_yvalue[i].push_back( yvalue[i] );
        if(  m_yvalue[i].count() > POINT_COUNT )
        {
            m_yvalue[i].pop_front();			
        }
        d_curves[i].setSamples( m_xvalue, m_yvalue[i] );
    }
    m_plot->replot();
}*/

void QPhotoShowWidget::UpdateCurvcnt(double yvalue, int index ,double times )
{
    if( yvalue > m_xmax )
        m_xmax = yvalue;
    if( yvalue < m_xmin )
        m_xmin = yvalue;
    m_plot->setAxisScale(QwtPlot::yLeft, m_xmin, m_xmax );
    if( index > m_iCurvcnt )
        return;
    if( m_xvalue.count() > 180 )
        m_xvalue.pop_back();
    m_xvalue.push_front(times);

    m_plot->setAxisScale( QwtPlot::xBottom,
        m_xvalue.last(), m_xvalue[0]);

    if( m_data[index].x_value.count() >180 )
        m_data[index].x_value.pop_back();
    if( m_data[index].y_value.count() > 180 )
        m_data[index].y_value.pop_back();

    m_data[index].y_value.push_front(yvalue);
    m_data[index].x_value.push_front( times );

    for( int i = 0; i < m_data.count(); ++i )
    {
        
        if( m_data[i].x_value.count() != 0 && m_data[i].y_value.count() != 0 )
        {
            if( m_data[i].x_value.first() < m_xvalue.last() )
            {
                m_data[i].x_value.clear();
                m_data[i].y_value.clear();
                continue;
            }
            d_curves[i].setSamples(m_data[i].x_value, m_data[i].y_value );
        }
        
    }
    /*m_yvalue[index].push_front( yvalue );
    if(  m_yvalue[index].count() > POINT_COUNT )
    {
        m_yvalue[index].pop_back();			
    }
    d_curves[index].setSamples( m_xvalue, m_yvalue[index] );
    for( int i = 0; i < m_iCurvcnt; i++ )
    {
        if( i == index )
            continue;
        d_curves[i].setSamples( m_xvalue, m_yvalue[i] );
    }*/

    m_plot->replot();
    
}

void QPhotoShowWidget::ShowSpline(QString str, QVector<XY_VALUE> vt_info )
{
    Q_ASSERT(vt_info.count() == m_iCurvcnt );
    
    QString msg = QString::fromLocal8Bit("当前设备：");
    msg += str;
    m_currentDevice->setText( msg );
    double xvalue = 0;
    double xmin = 0; 
    double xmax = 0;
    for( int i = 0; i < m_iCurvcnt; i++ )
    {
        if( vt_info[i].x_value.isEmpty() || vt_info[i].y_value.isEmpty() )
            continue;
        else
        {
            xvalue = vt_info[i].x_value.last();
            if(vt_info[i].y_value.first()> xmax )
                xmax = vt_info[i].y_value.first();
            if( xmin > xmax )
                xmin = xmax;
            break;
        }
    }
    
    for( int i = 0; i < m_iCurvcnt; i++ )
    {
        if( vt_info[i].x_value.isEmpty() )
            continue;
         if( xvalue < vt_info[i].x_value.last() );
         {
             xvalue = vt_info[i].x_value.last();
             m_xvalue = vt_info[i].x_value;
         }
         for( int j = 0; j < vt_info[i].y_value.count();++j )
         {
             if( vt_info[i].y_value[j] > xmax )
             {
                 xmax = vt_info[i].y_value[j];
             }
             else if( vt_info[i].y_value[j] < xmin )
             {
                 xmin = vt_info[i].y_value[j];
             }
         }
    }
    m_plot->setAxisScale(QwtPlot::yLeft, xmin, xmax );
    if( xvalue == 0 )
    {
        m_plot->setAxisScale( QwtPlot::xBottom,
            0, 0);
    }	
    else
    {
        m_plot->setAxisScale( QwtPlot::xBottom,
            m_xvalue[0], m_xvalue.last());
    }
    
    for( int i = 0; i < vt_info.count(); ++i )
    {
        if( vt_info[i].x_value.count() != 0 && vt_info[i].y_value.count() != 0 )
        {
            d_curves[i].setSamples(vt_info[i].x_value, vt_info[i].y_value );
        }

    }
    m_plot->replot();
}

void QPhotoShowWidget::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent( e );
    QRect rc = geometry();	
    m_plot->setGeometry(0, 0, rc.width()*0.98, rc.height()*0.83 );
    m_currentDevice->setGeometry(30, rc.height()*0.87, 250, 30 );
    m_bt_changeDevice->setGeometry(rc.width()*0.76, rc.height()*0.87, 100, 30 );
}