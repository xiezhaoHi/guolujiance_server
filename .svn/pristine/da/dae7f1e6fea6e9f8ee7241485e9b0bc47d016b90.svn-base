#ifndef FLOW_LAYOUT_H
#define FLOW_LAYOUT_H

#include <QLayout>
#include <QRect>
#include <QStyle>


class FlowLayout : public QLayout
{
public:
    explicit FlowLayout(QWidget *parent, int margin = -1, int hSpacing = -1, int vSpacing = -1);

    explicit FlowLayout(int margin = -1, int hSpacing = -1, int vSpacing = -1);

    ~FlowLayout();

    void addItem(QLayoutItem *item);

    void setHorizontalSpacing(int hSpacing);

    int horizontalSpacing() const;

    void setVerticalSpacing(int vSpacing);

    int verticalSpacing() const;

    Qt::Orientations expandingDirections() const;

    bool hasHeightForWidth() const;

    int heightForWidth(int) const;

    int count() const;

    QLayoutItem *itemAt(int index) const;

    QSize minimumSize() const;

    void setGeometry(const QRect &rect);

    QSize sizeHint() const;

    QLayoutItem *takeAt(int index);

private:

    int doLayout(const QRect &rect, bool testOnly) const;

    int smartSpacing(QStyle::PixelMetric pm) const;

private:
    QList<QLayoutItem *>    m_itemList;
    int                     m_hSpace;
    int                     m_vSpace;
};
#endif // FLOW_LAYOUT_H
