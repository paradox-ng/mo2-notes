#pragma once

#include "IPluginPanel.h"
#include "gui/NotesWidget.h"

class MO2Notes final : public IPluginPanel {
    Q_OBJECT
    Q_INTERFACES(MOBase::IPlugin IPluginPanel)
    Q_PLUGIN_METADATA(IID "org.tannin.MO2Notes" FILE "mo2notes.json")

public:
    inline static const auto NAME = QStringLiteral("MO2 Notes");

    MO2Notes() = default;

    // IPlugin

    [[nodiscard]] QString name() const override;

    [[nodiscard]] std::vector<std::shared_ptr<const MOBase::IPluginRequirement> > requirements() const override;

    [[nodiscard]] QString author() const override;

    [[nodiscard]] QString description() const override;

    [[nodiscard]] MOBase::VersionInfo version() const override;

    [[nodiscard]] QList<MOBase::PluginSetting> settings() const override;

    [[nodiscard]] bool enabledByDefault() const override;

    // IPluginPanel

    bool initPlugin(MOBase::IOrganizer* organizer) override;

    QWidget* createWidget(IPanelInterface* panelInterface, QWidget* parent, const QString& profilePath) override;

    [[nodiscard]] QString label() const override;

    [[nodiscard]] Position position() const override;

private:
    MOBase::IOrganizer* m_Organizer{};
    IPanelInterface* m_PanelInterface{};
    NotesWidget* m_NotesWidget{};
};