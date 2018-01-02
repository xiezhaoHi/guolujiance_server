#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_transform.h"

class transform : public QMainWindow
{
	Q_OBJECT

public:
	transform(QWidget *parent = Q_NULLPTR);
	
	public slots:
	void on_clicked(bool checked = false);
	void on_old_clicked(bool checked = false);
private:
	Ui::transformClass ui;
};
