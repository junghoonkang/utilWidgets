# utilWidgets

**Reusable Qt UI utilities and custom widgets** for Qt5/Qt6 projects.  
Includes override-enabled widgets, dark mode helpers, dialog utilities, and a flexible tree widget system.

---

## 📦 Features

- 🎨 **Dark Mode Support** — palette & stylesheet
- 🧩 **OverrideFieldWidget<T>** — templated override-ready input field
- 💬 **Inline MessageBox** — `utilWidgets::dialog()` for modal prompts
- 🌲 **CustomTreeWidget** — lightweight tree UI: `TreeWidgetViewItem`, `TreeView`
- 🔧 Header-only, moc-safe design for easy integration

---

## 📥 Installation

### ✅ Option 1: Git Submodule

```bash
git submodule add https://github.com/yourname/utilWidgets external/utilWidgets
```

### ✅ Option 2: Manual
Download or clone the repository

Place under your project's external/ or thirdparty/ directory


⚙️ Build System Integration
▶ QMake (Qt Creator, qmake projects)
1. Add to your .pro file:
include(path/to/utilWidgets/utilWidgets.pri)
2. Done! All headers will be available via:
#include "utils.h"

▶ CMake (Modern Qt Projects)
1. Add to your CMakeLists.txt:
  add_subdirectory(external/utilWidgets)
  target_link_libraries(MyApp PRIVATE utilWidgets)
2. The library is header-only and links to Qt::Widgets automatically.

▶ Visual Studio (MSBuild, Qt VS Tools)
1. Add external/utilWidgets/include/ to your project's Additional Include Directories
2. Add needed headers manually:
  #include "utils.h"
  #include "CustomTreeWidget.h"
|Note: .pri is not used in MSBuild. Use include path + manual header management.

🧪 Example Usage
#include "utils.h"

auto* field = new utilWidgets::OverrideFieldWidget<QLineEdit>("Name", this);
field->getField()->setText("Overrided Value");
field->setOverride(true);

QString result = utilWidgets::dialog(this, "Confirm", "Apply this value?", {"Yes", "No"});

