#pragma once
#include <windows.h>
#include <QDir>
#include <QFile>
#include <QMenu>
#include <QStyle>
#include <QLabel>
#include <QTimer>
#include <QWidget>
#include <QPalette>
#include <QTextEdit>
#include <QStatusBar>
#include <QTextStream>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDirIterator>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QProcessEnvironment>

#include "utilWidgetsBases.h"


namespace utilWidgets {

    inline QPalette darkModePalete() {
        QPalette palette;

        palette.setColor(QPalette::Window, QColor(53, 53, 53));
        palette.setColor(QPalette::WindowText, Qt::white);
        palette.setColor(QPalette::Base, QColor(42, 42, 42));
        palette.setColor(QPalette::AlternateBase, QColor(66, 66, 66));
        palette.setColor(QPalette::ToolTipBase, Qt::white);
        palette.setColor(QPalette::ToolTipText, Qt::white);
        palette.setColor(QPalette::Text, Qt::white);
        palette.setColor(QPalette::Button, QColor(53, 53, 53));
        palette.setColor(QPalette::ButtonText, Qt::white);
        palette.setColor(QPalette::BrightText, Qt::red);

        palette.setColor(QPalette::Highlight, QColor(142, 45, 197));
        palette.setColor(QPalette::HighlightedText, Qt::green);

        palette.setColor(QPalette::Light, QColor(70,70,70));
        palette.setColor(QPalette::Mid, QColor(30,30,30));
        palette.setColor(QPalette::Dark, QColor(20,20,20));
        palette.setColor(QPalette::Shadow, Qt::black);

        return palette;
    }

    inline QString darkModeStyleSheet = R"(
        QScrollBar:vertical {
            background: #2b2b2b;
            width: 12px;
            margin: 0px 0px 0px 0px;
        }
        QScrollBar::handle:vertical {
            background: #5a5a5a;
            min-height: 20px;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical:hover {
            background: #787878;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
            background: none;
            border: none;
        }
        QScrollBar:horizontal {
            background: #2b2b2b;
            height: 12px;
            margin: 0px 0px 0px 0px;
        }
        QScrollBar::handle:horizontal {
            background: #5a5a5a;
            min-width: 20px;
            border-radius: 4px;
        }
        QScrollBar::handle:horizontal:hover {
            background: #787878;
        }
        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
            width: 0px;
            background: none;
            border: none;
        }

        QLabel:disabled {
            background-color: #2a2a2a;
            color: #777777;
        }

        QPushButton {
            background-color: #3c3f41;
            color: white;
            border: 1px solid #5a5a5a;
            padding: 5px;
            border-radius: 3px;
        }
        QPushButton:hover {
            background-color: #4c5052;
            border: 1px solid #6a6a6a;
        }
        QPushButton:pressed {
            background-color: #2c2f31;
        }
        QPushButton:disabled {
            background-color: #2a2a2a;
            color: #777777;
            border: 1px solid #444444;
        }
        QPushButton:focus {
            outline: none;
            border: 2px solid #8e2dc5;
        }

        QLineEdit {
            background-color: #3c3f41;
            color: white;
            border: 1px solid #5a5a5a;
            border-radius: 3px;
            padding: 4px;
        }
        QLineEdit:focus {
            border: 1px solid #8e2dc5;
            background-color: #444;
        }
        QLineEdit:disabled {
            background-color: #2a2a2a;
            color: #777777;
            border: 1px solid #444444;
        }

        QTextEdit {
            background-color: #2b2b2b;
            color: white;
            border: 1px solid #5a5a5a;
            border-radius: 4px;
        }

        QComboBox {
            background-color: #3c3f41;
            color: white;
            border: 1px solid #5a5a5a;
            padding: 4px;
            border-radius: 3px;
        }
        QComboBox:hover {
            background-color: #4c5052;
        }
        QComboBox QAbstractItemView {
            background-color: #2f2f2f;
            selection-background-color: #8e2dc5;
            color: white;
            border: 1px solid #5a5a5a;
        }
        QComboBox:disabled {
            background-color: #2a2a2a;
            color: #777777;
            border: 1px solid #444444;
        }

        QToolButton {
            background-color: transparent;
        }
        QToolButton:hover {
            background-color: #444444;
            border: 1px solid #888;
            border-radius: 3px;
        }

        QSplitter::handle {
            background-color: #5a5a5a;
        }

        QToolBar::separator {
            background: #888888;
            width: 2px;
            height: 2px;
            margin: 4px;
        }
    )";

    template <typename T>
    inline void applyDarkMode(T& t) {
        t.setPalette(darkModePalete());
        t.setStyleSheet(darkModeStyleSheet);
    }


    class Separator : public QFrame
    {
    public:
        explicit Separator(QWidget* parent = nullptr) : QFrame(parent) {}
        explicit Separator(const QString& direction, QWidget* parent = nullptr) : QFrame(parent) {
            QFrame::Shape shape;
            if (direction == "v") {
                shape = QFrame::VLine;
            } else if (direction == "h") {
                shape = QFrame::HLine;
            }
            setFrameShape(shape);
            setFrameShadow(QFrame::Sunken);
            setLineWidth(1);
            setMidLineWidth(2);
        }
        ~Separator() override = default;
    };


    using ComboBox = ComboBoxBase;


    template <typename T>
    class OverrideFieldWidget : public OverrideFieldWidgetBase {

    private:
        QLabel* m_label = nullptr;
        T* m_field;

    public:
        explicit OverrideFieldWidget(QWidget* parent = nullptr) : OverrideFieldWidgetBase(parent) {
            initUI();
        }

        explicit OverrideFieldWidget(const QString& name, QWidget* parent = nullptr) : OverrideFieldWidgetBase(parent) {
            initUI();
            m_label->setText(name);
        }

        void initUI() {
            // controls
            m_label = new QLabel(this);
            m_label->setContextMenuPolicy(Qt::CustomContextMenu);
            m_label->setAlignment(Qt::AlignCenter);

            m_field = new T(this);
            m_field->setMinimumWidth(50);
            m_field->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

            // layouts
            QGridLayout* lay = new QGridLayout(this);
            setLayout(lay);
            lay->setSpacing(4);
            lay->setContentsMargins(1,1,1,1);
            lay->setColumnStretch(0, 1);
            lay->setColumnStretch(1, 1);

            lay->addWidget(m_label, 0, 0);
            lay->addWidget(m_field, 0, 1);

            // signals
            connect(this, &OverrideFieldWidgetBase::overrideChanged, this, &OverrideFieldWidget::on_overrideChanged);
            connect(m_label, &QLabel::customContextMenuRequested, this, &OverrideFieldWidget::on_label_customContextMenuRequested);

        }

        QColor getOverrideColor(bool override) const {
            return override ? QColor("#FF8000") : this->palette().color(QPalette::WindowText);
        }

        void on_overrideChanged(bool override) {
            QFont font = m_label->font();
            font.setBold(override);
            font.setItalic(override);
            m_label->setFont(font);

            QPalette palette = m_label->palette();
            palette.setColor(QPalette::WindowText, getOverrideColor(override));
            m_label->setPalette(palette);

            m_label->setToolTip(override ? "Overrided" : "");
        }

        QString getName() const {
            return m_label->text();
        }

        void setName(const QString& name, const QString& tooltip = QString()) {
            m_label->setText(name);
            if (!tooltip.isEmpty())
                m_label->setToolTip(tooltip);
        }

        void setNameAlignment(Qt::Alignment alignment) {
            m_label->setAlignment(alignment);
        }

        void on_label_customContextMenuRequested(const QPoint& pos) {
            QMenu menu(this);

            QString actionName = isOverride() ? "Disable Override" : "Enable Override";
            QAction* overrideAction = menu.addAction(actionName, this, [this]() {
                setOverride(!isOverride());
            });

            menu.exec(QCursor::pos());
        }

        T* getField() { return m_field; }
        const T* getField() const { return m_field; }

        void setFieldAlignment(Qt::Alignment alignment) {
            m_field->setAlignment(alignment);
        }

    };


    class MessageBox : public MessageBoxBase
    {
    private:
        QHBoxLayout* m_btnLay = nullptr;
        QPushButton* m_clickedButton = nullptr;
        QStatusBar* m_statusbar = nullptr;

    public:
        explicit MessageBox(QWidget* parent = nullptr, const QString& title = "", const QString& msg = "")
            : MessageBoxBase(parent) {
            initUI(title, msg);
        }
        ~MessageBox() override = default;

    protected:
        void showEvent(QShowEvent* e) override {
            MessageBoxBase::showEvent(e);
            emit(showRequested());
        }

        void initUI(const QString& title, const QString& msg) override {
            applyDarkMode(*this);

            setAttribute(Qt::WA_DeleteOnClose);
            setWindowFlags(Qt::Window|Qt::WindowCloseButtonHint);
            setWindowModality(Qt::ApplicationModal);
            setWindowTitle(title.isEmpty() ? "No Titled" : title);

            // controls
            QLabel* msgLb = new QLabel(msg);
            m_statusbar = new QStatusBar(this);
            m_statusbar->setHidden(true);

            // layouts
            QVBoxLayout* lay = new QVBoxLayout(this);
            setLayout(lay);

            lay->addWidget(msgLb);

            m_btnLay = new QHBoxLayout(this);
            lay->addLayout(m_btnLay);

            lay->addWidget(new Separator("h", this));
            lay->addWidget(m_statusbar);
        }

    public:
        QPushButton* clickedButton() {
            return m_clickedButton;
        }

        QString clickedButtonText() {
            return m_clickedButton->text();
        }

        void addButton(QPushButton* btn) {
            m_btnLay->addWidget(btn);

            connect(btn, &QPushButton::clicked, this, [this, btn]() {
                m_clickedButton = btn;
                emit(buttonClicked(btn));
                close();
            });
        }

        void setStatusHidden(const bool hidden) {
            m_statusbar->setHidden(hidden);
        }
        void setStatusMessage(const QString& msg) {
            if (m_statusbar->isHidden()) m_statusbar->show();
            m_statusbar->showMessage(msg);
        }

    };

    inline void createDialog(const QString& title,
                             const QString& msg,
                             const QStringList& buttons,
                             const QString& defaultBtn,
                             MessageBox*& box,
                             QPushButton*& defaultButton,
                             QString& result,
                             QEventLoop& loop,
                             QWidget* parent = nullptr) {
        // create dialog box
        box = new MessageBox(parent, title, msg);
        // create buttons
        for (QString btnName : buttons) {
            QPushButton* btn = new QPushButton(btnName, box);
            box->addButton(btn);
            // set default button
            if (btnName == defaultBtn) {
                defaultButton = btn;
            }
        }
        // default button settings
        if (defaultButton) {
            defaultButton->setDefault(true);
            defaultButton->setFocusPolicy(Qt::StrongFocus);
        }

        QObject::connect(box, &MessageBox::buttonClicked, &loop, [&](QPushButton* btn) {
            result = btn->text();
            loop.quit();
        });

        QObject::connect(box, &MessageBox::showRequested, box, [&box, &defaultButton]() {
            box->show();
            box->adjustSize();
            QWidget* parentWidget = box->parentWidget();
            if (parentWidget) {
                QRect parentGeom = parentWidget->geometry();
                QPoint centerPos = parentGeom.center() - QPoint(box->width() / 2, box->height() / 2);
                box->move(centerPos);
            }
            QTimer::singleShot(0, box, [box]() {
                box->activateWindow();
                box->raise();
                box->setFocus();
            });
            if (defaultButton) {
                QTimer::singleShot(0, defaultButton, [defaultButton]() {
                    defaultButton->setFocus();
                });
            }
        });

    }

    inline QString dialog(QWidget* parent = nullptr,
                          const QString& title = "",
                          const QString& msg = "",
                          const QStringList& buttons = {"Ok"},
                          const QString& defaultBtn = "Ok") {
        QEventLoop loop;
        QString result;
        MessageBox* box = nullptr;
        QPushButton* defaultButton = nullptr;

        createDialog(title, msg, buttons, defaultBtn, box, defaultButton, result, loop, parent);
        box->show();

        loop.exec();

        return result;
    }

    inline QString dialog_limited(QWidget* parent = nullptr,
                                  const QString& title = "",
                                  const QString& msg = "",
                                  const QStringList& buttons = {"Ok"},
                                  const QString& defaultBtn = "Ok",
                                  const int timeOut = -1) {
        QEventLoop loop;
        QString result;
        MessageBox* box = nullptr;
        QPushButton* defaultButton = nullptr;

        createDialog(title, msg, buttons, defaultBtn, box, defaultButton, result, loop, parent);
        box->show();

        QString statusMsgHead = QString("Default Selection : [ %1 ] - ").arg(defaultBtn);
        int remaining = timeOut / 1000;
        int* remainingPtr = &remaining;
        box->setStatusMessage(statusMsgHead + QString("Closing in %1").arg(remaining));

        QTimer* timer = new QTimer(box);
        QObject::connect(timer, &QTimer::timeout, [&loop, &result, box, &statusMsgHead, &defaultBtn, remainingPtr]() {
            (*remainingPtr) --;
            if ((*remainingPtr) <= 0) {
                result = defaultBtn;
                box->close();
                loop.quit();
            } else {
                box->setStatusMessage(statusMsgHead + QString("Closing in %1").arg((*remainingPtr)));
            }
        });

        timer->start(1000);
        loop.exec();

        return result;
    }


    class LogView : public QWidget
    {
    private:
        QTextEdit* m_logField = nullptr;

    public:
        explicit LogView(QWidget* parent = nullptr) : QWidget(parent) {
            initUI();
        }
        ~LogView() = default;

        void initUI() {
            // controls
            QPushButton* clearBtn = new QPushButton(this);
            clearBtn->setIcon(this->style()->standardPixmap(QStyle::SP_TrashIcon));
            clearBtn->setFixedSize(QSize(20,20));
            clearBtn->setToolTip("Clear Log Field");

            m_logField = new QTextEdit(this);
            m_logField->setLineWrapMode(QTextEdit::NoWrap);
            m_logField->setReadOnly(true);

            // layouts
            QVBoxLayout* lay = new QVBoxLayout(this);
            lay->setSpacing(2);
            lay->setContentsMargins(1,1,1,1);
            setLayout(lay);

            QHBoxLayout* topLay = new QHBoxLayout(this);
            lay->addLayout(topLay);

            topLay->addWidget(new QLabel(" Log ", this));
            topLay->addWidget(clearBtn);

            lay->addWidget(m_logField);

            // connections
            connect(clearBtn, &QPushButton::clicked, m_logField, &QTextEdit::clear);
        }

        QTextEdit* getField() const {
            return m_logField;
        }

    };

}   // namespace utilWidgets


namespace debug_util {

    /* show current apps environment variables in a dialog.
    This is useful for debugging purposes, to check if the environment variables are set correctly.

    @param envName The name of the environment variable to show.
    @param parent The parent widget for the dialog. If nullptr, the dialog will be created without a parent.

    Note: This function uses QProcessEnvironment to get the current environment variables.
    It then checks if the specified environment variable is set and shows its value in a message box.

    Example usage:
        debug_util::ShowEnvironmentDialog("PYTHONPATH", this);
        debug_util::ShowEnvironmentDialog("MAYA_SCRIPT_PATH", this);
    */
    inline void showEnvironmentDialog(const QString envName, QWidget* parent = nullptr) {
        QString msg = "";
        QTextStream msgStream(&msg, QIODevice::WriteOnly);

        QProcessEnvironment currentEnv = QProcessEnvironment::systemEnvironment();
        for (const QString& key : currentEnv.keys()) {
            QString value = qEnvironmentVariable(key.toUtf8().constData(), "");
            if (!value.isEmpty() && key == envName) {
                msgStream << key << " : " << value;
            }
        }
        if (msg.isEmpty()) {
            msgStream << "Environment variable '" << envName << "' is not set.";
        }
        utilWidgets::dialog(parent, "Environment", msg);
    }


}   // namespace debug_util


namespace general_util {

    /* Function to read an ASCII file and return its content as a QString.
    This function opens the specified file in read-only mode, reads its content,
    and returns it as a QString.
    */
    inline QString readAsciiFile(const QString& path) {
        QString code = "";

        QFile scriptFile(path);
        if (scriptFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&scriptFile);
            code = in.readAll();
            scriptFile.close();
        }

        return code;
    }

    inline QString userDocPath() {
        return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    }

    /**
     * Function to create a directory path and return success status.
     *
     * - If the path already exists and is a directory, this function does nothing and returns true.
     * - If the path points to a file (i.e., has a suffix), it creates the parent directory of the file.
     * - If the path does not have a suffix (i.e., it's a directory), it creates the full path as-is.
     *
     * @param path The target path which may be a file path or directory path.
     * @return true if the directory exists or was successfully created; false otherwise.
     */
    inline bool mkDirPath(const QString& path) {
        QFileInfo info(path);

        QString dirPath;

        if (info.exists() && info.isDir()) {
            // case - exists and is a directory
            dirPath = info.absoluteFilePath();
        } else if (info.exists() && !info.isDir()) {
            // case - exists but not a directory
            dirPath = info.absolutePath();
        } else {
            if (!info.suffix().isEmpty()) {
                // case - not exists and has a suffix
                dirPath = info.absolutePath();
            } else {
                // case - not exists and no suffix
                dirPath = info.absoluteFilePath();
            }
        }

        QDir dir;
        return dir.mkpath(dirPath);
    }

    /* Function to get the current working directory of the application.
    This function returns the directory where the application is running from.
    The result path is < posix > format.
    */
    inline QString getPythonScriptPath() {
        QString exeDir = QCoreApplication::applicationDirPath(); // running path of this app
        QString scriptPath = exeDir + "/python/scripts";
        scriptPath.replace("\\", "/"); // safer for Maya's embedded python
        return scriptPath;
    }

    /* Function to get all Python file(*.py) paths in a directory and its subdirectories.

    Example usage:
        QStringList pythonFiles = getPythonFilePaths("C:/path/to/directory");
        for (const QString& filePath : pythonFiles) {
            qDebug() << filePath;
        }
    */
    inline QStringList getPythonFilePaths(const QString& rootPath,
                                          const bool recursive = false,
                                          const bool skipInit = true) {
        QStringList result;

        QDirIterator::IteratorFlags flags = recursive
                                            ? QDirIterator::Subdirectories
                                            : QDirIterator::NoIteratorFlags;
        QDirIterator it(rootPath, QStringList() << "*.py", QDir::Files, flags);
        while (it.hasNext()) {
            QString filePath = it.next();
            QString fileName = QFileInfo(filePath).fileName();
            // Skip __init__.py files
            if (skipInit && fileName == "__init__.py") {
                continue;
            }
            result << filePath;
        }

        return result;
    }

    /* Function to get file base name only from given file path.
    */
    inline QString getFileBaseNameFromPath(const QString& filePath) {
        return QFileInfo(filePath).baseName();
    }

    /* Get a QProcess object with the current environment variables.
    */
    inline QProcess* getWorkProcess(QWidget* parent = nullptr) {
        QProcess* proc = new QProcess(parent);
        proc->setProcessEnvironment(QProcessEnvironment::systemEnvironment());
        proc->setProcessChannelMode(QProcess::MergedChannels);
        QObject::connect(proc, &QProcess::finished, proc, &QObject::deleteLater);

        return proc;
    }
    /* Get a QProcess object with the current environment variables and PYTHONPATH set to the given paths.
    This function is useful for running Python scripts with specific PYTHONPATH settings.
    */
    inline QProcess* getWorkProcess(const QStringList& pyPaths, QWidget* parent = nullptr) {
        QProcess* proc = new QProcess(parent);

        // expand the PYTHONPATH environment variable with the given paths
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        QString fullPath = env.value("PYTHONPATH");
        for (const QString& pyPath : pyPaths) {
            if (!fullPath.isEmpty())
                fullPath += ";";
            fullPath += pyPath;
        }
        env.insert("PYTHONPATH", fullPath);

        proc->setProcessEnvironment(env);
        proc->setProcessChannelMode(QProcess::MergedChannels);

        return proc;
    }

    inline bool isRunningPID(const qint64 pid) {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, static_cast<DWORD>(pid));
        if (!hProcess) return false;

        DWORD exitCode = 0;
        bool isAlive = false;
        if (GetExitCodeProcess(hProcess, &exitCode)) {
            isAlive = (exitCode == STILL_ACTIVE);
        }
        CloseHandle(hProcess);
        return isAlive;
    }

    inline bool killPID(const qint64 pid) {
        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, static_cast<DWORD>(pid));
        if (!hProcess) {
            DWORD error = GetLastError();
            qWarning() << "Failed to open process for termination. PID:" << pid
                    << "Error code:" << error;
            return false;
        }

        bool result = true;
        if (!TerminateProcess(hProcess, 1)) {
            DWORD error = GetLastError();
            qWarning() << "Failed to terminate process. PID:" << pid
                    << "Error code:" << error;
            result = false;
        }

        CloseHandle(hProcess);
        return result;
    }

    inline QString quote(const QString& string) {
        if (string.isEmpty()) return "\"\"";
        QString result = string;
        result.replace('"', "\\\"");
        if (result.contains(QRegularExpression(R"([\s\"'])"))) {
            result = "\"" + result + "\"";
        }
        return result;
    }

    inline void openInExplorer(const std::wstring& absPath) {
        std::wstring param = L"\"" + absPath + L"\"";
        ShellExecuteW(nullptr, L"open", L"explorer.exe", param.c_str(), nullptr, SW_SHOWDEFAULT);
    }

    inline void selectInExplorer(const std::wstring& absPath) {
        std::wstring param = L"/select,\"" + absPath + L"\"";
        ShellExecuteW(nullptr, L"open", L"explorer.exe", param.c_str(), nullptr, SW_SHOWDEFAULT);
    }

}   // namespace general_util

