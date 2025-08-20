#pragma once
#include <QLabel>
#include <QObject>
#include <QString>
#include <QPainter>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include "utils.h"


class TreeWidgetViewCollapseButton : public QPushButton
{
Q_OBJECT

signals:
    void toggled(bool checked);
    void expanded();
    void collapsed();

private:
    bool m_collapsed = false;
    QString m_expandedMark = "v";
    QString m_collapsedMark = ">";

public:
    explicit TreeWidgetViewCollapseButton(QWidget* parent = nullptr) : QPushButton(parent) {
        initUI();
    }

    ~TreeWidgetViewCollapseButton() override = default;

    void initUI() {
        setStyleSheet(R"(
            QPushButton {
                background-color: #3c3f41;
                color: white;
                border: 2px solid #555;
                border-radius: 4px;
                padding: 2px;
            }
            QPushButton:hover {
                background-color: #4c5052;
            }
        )");

        setText(m_expandedMark);
        setMinimumWidth(30);
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

        connect(this, &QPushButton::clicked, this, [this]() {
            m_collapsed = !m_collapsed;
            if (m_collapsed) {
                emit collapsed();
            } else {
                emit expanded();
            }
            emit(toggled(m_collapsed));
        });

        connect(this, &TreeWidgetViewCollapseButton::toggled, this, [this](bool toggled) {
            updateBtnMark();
        });
    }

    virtual QSize sizeHint() const override { return QSize(30, 30); }

    void updateBtnMark() {
        setText(m_collapsed ? m_collapsedMark : m_expandedMark);
    }

    bool isCollapsed() const { return m_collapsed; }

    void setCollapsed(bool status) {
        if (m_collapsed == status) return;
        m_collapsed = status;

        updateBtnMark();
        if (m_collapsed) {
            emit collapsed();
        } else {
            emit expanded();
        }
        emit toggled(m_collapsed);
    }

};


class TreeWidgetViewItem : public QWidget
{
Q_OBJECT

signals:
    void collapsed(int index);
    void expanded(int index);
    void indexChanged(int oldIndex, int index);
    void localIndexChanged(int oldIndex, int index);
    void parentIndexChanged(int oldIndex, int index);
    void itemRemoved(TreeWidgetViewItem* item);
    void itemCleared();

private:
    int m_index = -1;
    int m_localIndex = -1;
    int m_parentIndex = -1;

protected:
    QColor m_bgColor = QColor("#1f1f1f");
    bool m_isHovered = false;
    bool m_alternateRowColors = true;

    TreeWidgetViewCollapseButton* m_collapseBtn = nullptr;
    QLabel* m_label = nullptr;

    QHBoxLayout* m_lay = nullptr;
    QVBoxLayout* m_childrenLay = nullptr;

public:
    explicit TreeWidgetViewItem(QWidget* parent = nullptr) : QWidget(parent) {
        initUI();
        updateCollapseBtnVis();
    }

    explicit TreeWidgetViewItem(const QString& text, QWidget* parent = nullptr) : QWidget(parent) {
        initUI();
        updateCollapseBtnVis();
        if (!text.isEmpty()) {
            m_label->setText(text);
            m_label->setHidden(false);
        }
    }

    ~TreeWidgetViewItem() override = default;

    int getIndex() const { return m_index; }
    void setIndex(int index) {
        if (m_index == index) return;
        int oldIndex = m_index;
        m_index = index;
        update();
        emit indexChanged(oldIndex, m_index);
    }
    int getLocalIndex() const { return m_localIndex; }
    void setLocalIndex(int index) {
        if (m_localIndex == index) return;
        int oldIndex = m_localIndex;
        m_localIndex = index;
        emit localIndexChanged(oldIndex, m_localIndex);
    }
    int getParentIndex() const { return m_parentIndex; }
    void setParentIndex(int index) {
        if (m_parentIndex == index) return;
        int oldIndex = m_parentIndex;
        m_parentIndex = index;
        emit parentIndexChanged(oldIndex, m_parentIndex);
    }

    QColor getBgColor() const {
        return m_bgColor;
    }
    void setBgColor(QColor color) {
        m_bgColor = color;
        update();
    }

    bool alternateRowColors() const {
        return m_alternateRowColors;
    }
    void setAlternateRowColors(bool status) {
        m_alternateRowColors = status;
        update();
    }

    int getIndent() const {
        if (m_lay == nullptr) return 0;
        return m_lay->contentsMargins().left();
    }
    void setIndent(int indent) {
        if (m_lay == nullptr) return;
        m_lay->setContentsMargins(indent, 0, 0, 0);
        update();
    }

    virtual void initUI() {
        setAutoFillBackground(m_index >= 0);

        // controls
        m_collapseBtn = new TreeWidgetViewCollapseButton(this);

        m_label = new QLabel(this);
        m_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        m_label->setFixedHeight(40);
        QFont font = m_label->font();
        font.setBold(true);
        m_label->setFont(font);
        m_label->setHidden(true);

        // layouts
        m_lay = new QHBoxLayout(this);
        m_lay->setSpacing(5);
        m_lay->setContentsMargins(4, 0, 0, 0);
        m_lay->setAlignment(Qt::AlignLeft);

        m_lay->addWidget(m_collapseBtn);
        m_lay->setAlignment(m_collapseBtn, Qt::AlignLeft|Qt::AlignTop);

        m_childrenLay = new QVBoxLayout(this);
        m_childrenLay->setSpacing(2);
        m_childrenLay->setContentsMargins(0, 0, 0, 0);
        m_childrenLay->setAlignment(Qt::AlignTop|Qt::AlignLeft);
        m_lay->addLayout(m_childrenLay);

        m_childrenLay->addWidget(m_label);

        // signals
        connect(m_collapseBtn, &TreeWidgetViewCollapseButton::toggled, this, [this](bool toggled) {
            for (TreeWidgetViewItem* child : getChildren()) {
                if (child)
                    child->setVisible(!toggled);
            }

            if (toggled) {
                emit collapsed(m_index);
            } else {
                emit expanded(m_index);
            }

            update();
        });

    }

    virtual void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);

        // bg color
        QColor viewBgColor = m_bgColor;
        QColor bgColor("#393939");
        QColor hoverColor("#b08b10");
        QColor lineColor = m_isHovered ? hoverColor : viewBgColor;

        QPen linePen(lineColor, m_isHovered ? 2 : 1);
        // linePen.setStyle(Qt::DotLine);
        painter.setPen(linePen);

        if (alternateRowColors()) {
            bgColor = m_index % 2 == 0 ? bgColor : QColor("#2f2f2f");
        }
        painter.fillRect(rect(), bgColor);

        // indent bar
        if (hasParentItem()) {
            int indent = getIndent();
            QRect indentRect = QRect(0, 0, indent, height());
            painter.fillRect(indentRect, lineColor);
        }

        // tree line
        if (hasChildren() && !m_collapseBtn->isCollapsed()) {
            QRect collapseBtnRect = m_collapseBtn->rect();
            int x = m_collapseBtn->mapTo(this, collapseBtnRect.center()).x();
            int Y = m_collapseBtn->mapTo(this, collapseBtnRect.bottomLeft()).y();
            int y = Y;
            // draw line bg
            QRect lineBgRect = QRect(0, y, x+15, height());
            painter.fillRect(lineBgRect, viewBgColor);
            // draw line
            for (auto* child : getChildren()) {
                y = child->y() + child->height() / 2;
                painter.drawLine(x, y, x + 20, y);
            }
            painter.drawLine(x, Y, x, y);
        }

        if (m_isHovered && m_index > -1) {
            painter.drawRect(rect());
        }

        QWidget::paintEvent(event);
    }

    virtual void enterEvent(QEnterEvent* event) override {
        QWidget::enterEvent(event);
        m_isHovered = true;
        update();
    }

    virtual void leaveEvent(QEvent* event) override {
        QWidget::leaveEvent(event);
        m_isHovered = false;
        update();
    }

    void addWidget(QWidget* widget, const bool parentToThis = true) {
        if (widget == nullptr) return;
        m_childrenLay->addWidget(widget);
        if (parentToThis && widget->parent() != this) {
            widget->setParent(this);
        }
    }

    QList<QWidget*> getWidgets() {
        QList<QWidget*> result;
        for (int i = 0; i < m_childrenLay->count(); ++i) {
            QLayoutItem* item = m_childrenLay->itemAt(i);
            if (!item) continue;

            QWidget* widget = item->widget();
            if (widget) {
                result.append(widget);
            }
        }
        return result;
    }

    template<typename T>
    QList<T*> getWidgetsByType() {
        QList<T*> result;
        for (int i = 0; i < m_childrenLay->count(); ++i) {
            if (auto* w = qobject_cast<T*>(m_childrenLay->itemAt(i)->widget())) {
                result.append(w);
            }
        }
        return result;
    }

    void addLayout(QLayout* layout) {
        if (layout == nullptr) return;
        m_childrenLay->addLayout(layout);
    }

    void updateCollapseBtnVis() {
        if (m_index < 0 || !getChildren().count()) {
            m_collapseBtn->setHidden(true);
            update();
            return;
        }
        m_collapseBtn->setHidden(false);
        update();
    }

    void setParentItem(TreeWidgetViewItem* parent) {
        if (parent == nullptr || parent->isChild(this)) return;

        if (this->parent() != parent)
            parent->appendRow(this);
    }

    bool hasParentItem() const {
        return parentWidget() != nullptr && qobject_cast<TreeWidgetViewItem*>(parentWidget()) != nullptr;
    }

    QList<TreeWidgetViewItem*> getChildren() const {
        QList<TreeWidgetViewItem*> result;
        for (int i = 0; i < m_childrenLay->count(); ++i) {
            if (auto child = qobject_cast<TreeWidgetViewItem*>(m_childrenLay->itemAt(i)->widget())) {
                result.append(child);
            }
        }
        return result;
    }

    bool hasChildren() const {
        return !getChildren().isEmpty();
    }

    QList<TreeWidgetViewItem*> getAllChildren() const {
        QList<TreeWidgetViewItem*> result;

        for (TreeWidgetViewItem* child : getChildren()) {
            if (!child) continue;

            result.append(child);
            result.append(child->getAllChildren());
        }

        return result;
    }

    bool isChild(TreeWidgetViewItem* child) const {
        if (child == nullptr) return false;
        for (auto* c : getChildren()) {
            if (c == child) return true;
        }
        return false;
    }

    void updateChildrenIndex() {
        int i = 0;
        for (auto* child : getChildren()) {
            child->setIndex(m_index + i);
            child->setLocalIndex(i);
            child->setParentIndex(m_index);
            i ++;
        }
    }

    int rowCount() const {
        return getChildren().count();
    }

    void appendRow(TreeWidgetViewItem* child) {
        if (!child || isChild(child)) return;

        if (child->parent() != this)
            child->setParent(this);

        m_childrenLay->addWidget(child);

        int row = rowCount() - 1;
        child->setIndex(m_index + row + 1);
        child->setLocalIndex(row);
        child->setParentIndex(m_index);

        updateCollapseBtnVis();
    }

    void removeRow(TreeWidgetViewItem* child) {
        if (!child || !isChild(child)) return;

        emit(itemRemoved(child));
        child->clear();
        m_childrenLay->removeWidget(child);
        child->setParent(nullptr);
        child->deleteLater();

        updateCollapseBtnVis();
        updateChildrenIndex();
    }

    void clear() {
        QList<TreeWidgetViewItem*> children = getChildren();
        for (TreeWidgetViewItem* child : children) {
            child->clear();
            m_childrenLay->removeWidget(child);
            child->setParent(nullptr);
            child->deleteLater();
        }
        updateCollapseBtnVis();
        emit(itemCleared());
    }

    QList<TreeWidgetViewItem*> getParents(bool includeInvisibleRootItem = false) const {
        QList<TreeWidgetViewItem*> parents;
        QWidget* current = parentWidget();

        while (auto* p = qobject_cast<TreeWidgetViewItem*>(current)) {
            if (p->getIndex() == -1 && !includeInvisibleRootItem)
                break;

            parents.append(p);
            current = p->parentWidget();
        }

        return parents;
    }

};


class InvisibleRootItem : public TreeWidgetViewItem
{
Q_OBJECT

protected:
    QList<TreeWidgetViewItem*> m_items;

public:
    explicit InvisibleRootItem(QWidget* parent = nullptr) : TreeWidgetViewItem(parent) {
        setIndex(-1);
        setIndent(0);
        m_collapseBtn->setHidden(true);
    }

    ~InvisibleRootItem() override = default;

    virtual QSize sizeHint() const override { return QSize(100, 40); }

    QList<TreeWidgetViewItem*> getItems() {
        return m_items;
    }

    void updateItemIndex() {
        int i = 0;
        for (auto* item : m_items) {
            item->setIndex(i++);
            item->updateChildrenIndex();
        }
    }

    void setAlternateRowColors(bool status) {
        TreeWidgetViewItem::setAlternateRowColors(status);
        updateItemIndex();
    }

    void appendRow(TreeWidgetViewItem* child) {
        if (!child || m_items.contains(child)) return;
        m_items.append(child);
        child->setIndex(m_items.count() + 1);
        TreeWidgetViewItem::appendRow(child);
    }

    void removeRow(TreeWidgetViewItem* child) {
        if (!child || !m_items.contains(child)) return;

        m_items.removeOne(child);
        TreeWidgetViewItem::removeRow(child);
    }

    void clear() {
        m_items.clear();
        TreeWidgetViewItem::clear();
    }

protected:
    virtual void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        painter.fillRect(rect(), m_bgColor);
    }

};


class TreeView : public QWidget
{
Q_OBJECT

signals:


private:
    QScrollArea* m_mainScroll;
    InvisibleRootItem* m_rootItem;
    QList<TreeWidgetViewItem*> m_items;

public:
    TreeView(QWidget* parent = nullptr) : QWidget(parent) {
        initUI();
    }

    TreeView(const QString& title, QWidget* parent = nullptr) : QWidget(parent) {
        initUI();
        setWindowFlags(Qt::Window);
        setWindowTitle(title);
    }

    ~TreeView() override = default;

    InvisibleRootItem* invisibleRootItem() {
        return m_rootItem;
    }

    QList<TreeWidgetViewItem*> getItems() {
        return invisibleRootItem()->getItems();
    }

    void initUI() {
        // controls
        m_mainScroll = new QScrollArea(this);
        m_mainScroll->setWidgetResizable(true);
        m_mainScroll->setStyleSheet(R"(
            QScrollArea {
                border: 2px solid #8e2dc5;
                border-radius: 5px;
                background-color: #1f1f1f;
            }
        )");

        m_rootItem = new InvisibleRootItem(this);
        m_mainScroll->setWidget(m_rootItem);

        // layouts
        QVBoxLayout* lay = new QVBoxLayout(this);
        lay->setSpacing(2);
        lay->setContentsMargins(4,4,4,4);
        lay->setAlignment(Qt::AlignTop);
        setLayout(lay);

        lay->addWidget(m_mainScroll);

        // signals

    }

    virtual QSize sizeHint() const override { return QSize(400, 400); }

    int rowCount() const {
        return m_rootItem->rowCount();
    }

    bool isEmpty() const {
        return !rowCount();
    }

    void clear() {
        m_rootItem->clear();
        m_items.clear();
    }

    void removeRow(TreeWidgetViewItem* item) {
        m_rootItem->removeRow(item);
    }

    void appendRow(TreeWidgetViewItem* item) {
        m_items.append(item);
        m_rootItem->appendRow(item);
        update();
    }

};
