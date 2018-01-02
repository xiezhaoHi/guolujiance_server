/*******************************************************************************
* Copyright:   2011-2012 by Arsaisy.lin
* File name:   LayoutManager.h
* Description: ���ֹ�����ģ�飬���ڹ���MS Windowsƽ̨������Ӧ�ó�����沼�֣���
*              ����Win32API��MFC��ATL���봰�ھ��(HWND)��صĿⴴ������������
*              �򡣱�ģ��������MS Platform SDK�ͱ�׼C++STL��
*              ���ļ�Ϊ�����Դ������Ӧ���ڿ�Դ����Ѻ���ҵ�����Ӧ�ã�������ȡ
*              �κη��ã�����Ҫע�����������
* Author:      Arsaisy.lin
* Contact:     arsaisy@126.com
* Version:     1.0
* Date:        2011/10/26
* History:     
*******************************************************************************/

#ifndef __LAYOUTMANAGER_H__
#define __LAYOUTMANAGER_H__

#include <Windows.h>
#include <list>
#include <vector>

namespace Win32Layout
{

/*******************************************************************************
* SizeType�ߴ����ͣ������е�ռλ��ʽ��
*   PIXEL:      �����ط�ʽռλ
*   PERCENTAGE: �Ա�����ʽռλ
#   FILL:       ����䷽ʽռλ
*******************************************************************************/
enum SizeType {
	PIXEL,
	PERCENTAGE,
	FILL
};

/*******************************************************************************
* BoxType�������ͣ����ֶ���Ķ��뷽ʽ��
*   HORIZONTAL: ˮƽ�ֲ�����
*   VERTICAL:   ��ֱ�ֲ�����
*******************************************************************************/
enum BoxType {
	HORIZONTAL,
	VERTICAL
};

/*******************************************************************************
* LayoutObject���ֶ�����
*   LayoutObject�������п��Խ��в�����Ļ��࣬��Ҫ���в��ֵ��඼��Ҫ�̳д��ࡣ
*******************************************************************************/
class LayoutObject {
public:
	LayoutObject() :_w(0), _h(0), _stw(PIXEL), _sth(PIXEL) {}
	virtual ~LayoutObject() {}
	virtual void Layout(int x, int y, int w, int h) = 0;
	int GetW() { return _w; }
	int GetH() { return _h; }
	SizeType GetWST() { return _stw; }
	SizeType GetHST() { return _sth; }

	/*
	* SetSize���������ò��ֶ���ĳߴ�ͳߴ����͡�
	*   w:   ���
	*   h:   �߶�
	*   stw: ��ȳߴ�����
	*   sth: �߶ȳߴ�����
	*   �����߶��ɳߴ����Ͷ�̬�������С������ߴ�����Ϊ����PIXEL����Ȼ�߶�
	* ��ʾʵ�ʵ����صĴ�С������ߴ�����Ϊ����PERCENTAGE����Ȼ�߶ȵĴ�С��ʾ
	* �ܿ�Ȼ��ܸ߶ȵ���Ӧ����ֵ������ߴ�����ΪFILL����Ȼ�߶��򾡿����������
	* ��ʣ�෶Χ��
	*/
	void SetSize(int w, int h, SizeType stw, SizeType sth) {
		_w = w;
		_h = h;
		_stw = stw;
		_sth = sth;
	}

protected:
	int _w;
	int _h;
	SizeType _stw;
	SizeType _sth;
};


/*******************************************************************************
* Window�����࣬�̳���LayoutObject�࣬����������ڵ�λ�á�
*   ͨ�����캯���봰�ھ���󶨣�Ҳ���Դ��������ʹ��Attach�봰�ھ���󶨡�����
* ��ʱ�������ٴ��ڣ�����Ҫ����󶨡�������ʹ�ù����ж�̬���������ڣ���������
* ��������
*******************************************************************************/
class Window : public LayoutObject {
public:
	Window(HWND hWnd = NULL) { _hWnd = hWnd; }
	virtual ~Window() {}
	
	/*
	* Attach�������봰�ھ�����а󶨡�������캯��û��ָ�����ھ������������Ҫ
	* ʱ���ô˺������а󶨡�Ҳ���Բ��󶨣�����ʱ�������κβ�����
	*/
	void Attach(HWND hWnd) { _hWnd = hWnd; }

	/*
	* Handle��������ȡ�Ѿ��󶨵Ĵ��ھ����
	*/
	HWND Handle() { return _hWnd; }

	/*
	* Layout���ֺ������Ѷ������������(x, y)����СΪw*h�ķ�Χ�ڡ��Դ��������
	* �ĵ���Win32API����MoveWindow���ö���λ�á�
	*/
	void Layout(int x, int y, int w, int h) {
		if (::IsWindow(_hWnd))
			MoveWindow(_hWnd, x, y, w, h, TRUE);
	}

protected:
	/*
	* ���ھ��������ΪNULL��
	*/
	HWND _hWnd;
};

/*******************************************************************************
* Box���ֺ����࣬��ģ������Ҫ���࣬���в��������ɴ�����ɡ�
*   ����һ����WINDOWS��Ϣ��ͬʹ�á�ʹ�÷���Ϊ1.�����ö���2.����AddWindowָ��Ҫ
* ���в��ֵĴ��ھ�������AddHBox��AddVBox����Ƕ�׵Ĳ��ֺ��ӣ�3.����Ҫ��ʱ����
# WM_SIZE��Ϣ��ʱ���ú��ӵ�Layout�������в��ֹ���
*/
class Box : public LayoutObject {
public:
	Box(BoxType boxType = HORIZONTAL) {
		_separatorSize = 3;
		_boxType = boxType;
	}

	virtual ~Box() {
		std::list<LayoutObject*>::iterator it = _sublist.begin();
		for (; it != _sublist.end(); it++)
			delete (*it);
		_sublist.clear();
	}

	/*
	* SetType���������ú��ӵĶ��뷽ʽ��������ˮƽ��ֱ��
	*/
	void SetType(BoxType boxType) {
		_boxType = boxType;
	}

	/*
	* SetSeparatorSize���������÷ָ�����С��Ĭ��Ϊ3�����ء�
	*/
	void SetSeparatorSize(int size) {
		_separatorSize = size;
	}

	/*
	* AddHBox������������Ƕ��ˮƽ���ӡ�
	*   ����ʱ����ָ�����ӵĴ�С����뷽ʽ��������ɺ󷵻������ӵĺ��ӣ�����ʹ��
	* ���صĺ��Ӽ������Ӷ����ɱ�������������Ƕ���Ӳ����Ե���deleteɾ��������ϵ
	* ͳ�Զ����ա�
	*/
	Box* AddHBox(int w, int h, SizeType stw, SizeType sth) {
		Box* box = new Box(HORIZONTAL);
		box->SetSize(w, h, stw, sth);
		_sublist.push_back(box);
		return box;
	}

	/*
	* AddVBox������������Ƕ�Ĵ�ֱ���ӡ�
	*   ������AddHBox�������ơ�
	*/
	Box* AddVBox(int w, int h, SizeType stw, SizeType sth) {
		Box* box = new Box(VERTICAL);
		box->SetSize(w, h, stw, sth);
		_sublist.push_back(box);
		return box;
	}

	/*
	* AddWindow������������Ҫ���ֵĴ��ڡ�
	*   ���øú�������һ����Ҫ���в��ֵĴ��ھ����ͬʱָ����С��ߴ����ͣ�������
	* ��Ӧ�Ĵ��ڲ��ֶ��󣬿��Ա��淽���Ժ���Ĵ�С�������������Ķ��󲻿��Ե���
	* deleteɾ��������ϵͳ�Զ����ա�
	*/
	Window* AddWindow(HWND hWnd, int w, int h, SizeType stw, SizeType sth) {
		Window* window = new Window(hWnd);
		window->SetSize(w, h, stw, sth);
		_sublist.push_back(window);
		return window;
	}

	/*
	* Layout�����������Ѿ�ָ�ɵĿؼ�����
	*   ��ָ���ؼ�����������(x, y)��w*h��Χ�ڡ�
	*/
	void Layout(int x, int y, int w, int h) {
		if (_boxType == HORIZONTAL)
			LayoutHorizontal(x, y, w, h);
		else
			LayoutVertical(x, y, w, h);
	}

protected:
	/*
	* LayoutHorizontal������ˮƽ�����ؼ�λ��
	*/
	void LayoutHorizontal(int x, int y, int w, int h) {
		int objCount = (int)_sublist.size();
		int spaceWidth = w - _separatorSize * (objCount - 1);
		int residualWidth = spaceWidth;
		int objx, objy, objw, objh;
		std::vector<int> wlist;
		std::vector<int> fillIndexs;

		if (spaceWidth <= 0)
			spaceWidth = 0;
		
		std::list<LayoutObject*>::iterator it = _sublist.begin();
		for (int index = 0; it != _sublist.end(); it++, index++) {
			LayoutObject* lo = (*it);

			if (lo->GetWST() == PIXEL) {
				objw = lo->GetW();
			} else if (lo->GetWST() == PERCENTAGE) {
				objw = (int)((double)spaceWidth * ((double)lo->GetW() / 100.0));
			} else if (lo->GetWST() == FILL) {
				objw = -1;
				fillIndexs.push_back(index);
			} else {
				objw = 0;
			}
			
			if (objw > residualWidth)
				objw = residualWidth;
			wlist.push_back(objw);
			residualWidth -= objw;
		}

		if (fillIndexs.size() > 0) {
			for (int i = 0; i < residualWidth; i++) {
				int index = i % (int)fillIndexs.size();
				wlist[fillIndexs[index]] += 1;
			}
		}

		objx = x;
		objy = y;
		it = _sublist.begin();
		for (int index = 0; it != _sublist.end(); it++, index++) {
			LayoutObject* lo = (*it);
			objw = wlist[index];

			if (lo->GetHST() == PIXEL) {
				objh = lo->GetH();
			} else if (lo->GetHST() == PERCENTAGE) {
				objh = (int)((double)h * ((double)lo->GetH() / 100.0));
			} else if (lo->GetHST() == FILL) {
				objh = h;
			}
			if (objh > h)
				objh = h;

			lo->Layout(objx, objy, objw, objh);

			objx += objw;
			objx += _separatorSize;
		}
	}

	/*
	* LayoutHorizontal��������ֱ�����ؼ�λ��
	*/
	void LayoutVertical(int x, int y, int w, int h) {
		int objCount = (int)_sublist.size();
		int spaceHeight = h - _separatorSize * (objCount - 1);
		int residualHeight = spaceHeight;
		int objx, objy, objw, objh;
		std::vector<int> hlist;
		std::vector<int> fillIndexs;

		if (spaceHeight <= 0)
			spaceHeight = 0;
		
		std::list<LayoutObject*>::iterator it = _sublist.begin();
		for (int index = 0; it != _sublist.end(); it++, index++) {
			LayoutObject* lo = (*it);

			if (lo->GetHST() == PIXEL) {
				objh = lo->GetH();
			} else if (lo->GetHST() == PERCENTAGE) {
				objh = (int)((double)spaceHeight * ((double)lo->GetH() / 100.0));
			} else if (lo->GetHST() == FILL) {
				objh = -1;
				fillIndexs.push_back(index);
			} else {
				objh = 0;
			}
			
			if (objh > residualHeight)
				objh = residualHeight;
			hlist.push_back(objh);
			residualHeight -= objh;
		}

		if (fillIndexs.size() > 0) {
			for (int i = 0; i < residualHeight; i++) {
				int index = i % (int)fillIndexs.size();
				hlist[fillIndexs[index]] += 1;
			}
		}

		objx = x;
		objy = y;
		it = _sublist.begin();
		for (int index = 0; it != _sublist.end(); it++, index++) {
			LayoutObject* lo = (*it);
			objh = hlist[index];

			if (lo->GetWST() == PIXEL) {
				objw = lo->GetW();
			} else if (lo->GetWST() == PERCENTAGE) {
				objw = (int)((double)h * ((double)lo->GetW() / 100.0));
			} else if (lo->GetWST() == FILL) {
				objw = w;
			} else {
				objw = 0;
			}
			if (objw > w)
				objw = w;

			lo->Layout(objx, objy, objw, objh);

			objy += objh;
			objy += _separatorSize;
		}
	}

protected:
	std::list<LayoutObject*> _sublist; /*�����б�*/
	int _separatorSize;                /*�ָ�����С*/
	BoxType _boxType;                  /*��������*/
};

}

#endif
