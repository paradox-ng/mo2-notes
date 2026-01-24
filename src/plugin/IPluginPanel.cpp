#include "IPluginPanel.h"
#include "MOPanelInterface.h"

#include <QTabWidget>

using namespace Qt::Literals::StringLiterals;

IPluginPanel::Position IPluginPanel::Position::before(const QString& panelName)
{
    return { Order::Before, panelName };
}

IPluginPanel::Position IPluginPanel::Position::after(const QString& panelName)
{
    return { Order::After, panelName };
}

IPluginPanel::Position IPluginPanel::Position::inPlaceOf(const QString& panelName)
{
    return { Order::InPlaceOf, panelName };
}

IPluginPanel::Position IPluginPanel::Position::atStart()
{
    return { Order::AtStart, {} };
}

IPluginPanel::Position IPluginPanel::Position::atEnd()
{
    return { Order::AtEnd, {} };
}

bool IPluginPanel::init(MOBase::IOrganizer* organizer)
{
    if (organizer == nullptr) {
        return false;
    }

    organizer->onUserInterfaceInitialized([this, organizer](QMainWindow* mainWindow) {
        const auto tabWidget = mainWindow->findChild<QTabWidget*>(u"tabWidget"_s);

        if (tabWidget == nullptr) {
            return;
        }

        const auto intfc    = new MOPanelInterface(organizer, mainWindow);
        const auto profile  = organizer->profile()->absolutePath();
        const auto widget   = this->createWidget(intfc, tabWidget, profile);
        const auto label    = this->label();
        const auto position = this->position();

        // Save the currently selected widget (not just the index) so we can
        // restore the selection after inserting the new tab. This prevents
        // the plugin from interfering with MO2's saved window state.
        QWidget* const currentWidget = tabWidget->currentWidget();

        switch (position.order_) {
        case Order::Before: {
            const auto refTab = !position.reference_.isNull()
                ? tabWidget->findChild<QWidget*>(position.reference_)
                : nullptr;
            if (refTab != nullptr) {
                if (const int index = tabWidget->indexOf(refTab); index != -1) {
                    tabWidget->insertTab(index, widget, label);
                }
                break;
            }
        }
            [[fallthrough]];
        case Order::AtStart: {
            tabWidget->insertTab(0, widget, label);
        }
        break;
        case Order::InPlaceOf:
        case Order::After: {
            const auto refTab = !position.reference_.isNull()
                ? tabWidget->findChild<QWidget*>(position.reference_)
                : nullptr;

            if (refTab != nullptr) {
                if (const int index = tabWidget->indexOf(refTab); index != -1) {
                    tabWidget->insertTab(index + 1, widget, label);
                    if (position.order_ == Order::InPlaceOf) {
                        tabWidget->removeTab(index);
                    }
                }
                break;
            }
        }
            [[fallthrough]];
        case Order::AtEnd: {
            tabWidget->addTab(widget, label);
        }
        break;
        }

        // Restore the original tab selection. By restoring based on the widget
        // pointer rather than the index, we correctly handle index shifts caused
        // by tab insertion.
        if (currentWidget != nullptr) {
            const int restoredIndex = tabWidget->indexOf(currentWidget);
            if (restoredIndex >= 0) {
                tabWidget->setCurrentIndex(restoredIndex);
            }
        }

        intfc->assignWidget(tabWidget, widget);
    });

    return this->initPlugin(organizer);
}