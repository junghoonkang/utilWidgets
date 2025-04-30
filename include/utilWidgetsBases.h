#pragma once
#include <QWidget>
#include <QPushButton>


class OverrideFieldWidgetBase : public QWidget {
    Q_OBJECT

signals:
    void overrideChanged(bool override);

private:
    bool m_override = false;

public:
    OverrideFieldWidgetBase(QWidget* parent = nullptr) : QWidget(parent) {}

    virtual ~OverrideFieldWidgetBase() {}

    bool isOverride() const {
        return m_override;
    }

    void setOverride(bool override) {
        bool oldOverride = m_override;
        m_override = override;
        if (oldOverride != m_override) {
            emit overrideChanged(m_override);
        }
    }
};


class MessageBoxBase : public QWidget
{
    Q_OBJECT

signals:
    void buttonClicked(QPushButton* btn);

public:
    explicit MessageBoxBase(QWidget* parent = nullptr, const QString& title = "", const QString& msg = "")
        : QWidget(parent) {
        initUI(title, msg);
    }
    ~MessageBoxBase() override = default;

protected:
    virtual void initUI(const QString& title, const QString& msg) {}
};
