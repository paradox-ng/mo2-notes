#include "MO2Notes.h"
#include "gui/NotesWidget.h"

using namespace Qt::Literals::StringLiterals;

bool MO2Notes::initPlugin(MOBase::IOrganizer* organizer)
{
    m_Organizer = organizer;
    // m_Organizer->onUserInterfaceInitialized([this](QMainWindow*) {
    //     m_Organizer->onProfileChanged([this](MOBase::IProfile*, const MOBase::IProfile* newProfile) {
    //         if (m_NotesWidget) {
    //             const auto newPath = newProfile->absolutePath();
    //             m_NotesWidget->setProfilePath(newPath);
    //         }
    //     });
    // });
    return true;
}

QString MO2Notes::name() const { return NAME; }

std::vector<std::shared_ptr<const MOBase::IPluginRequirement> > MO2Notes::requirements() const
{
    return { Requirements::gameDependency({ u"Oblivion"_s, u"Fallout 3"_s, u"New Vegas"_s, u"Skyrim"_s, u"Enderal"_s,
                                            u"Fallout 4"_s, u"Skyrim Special Edition"_s, u"Enderal Special Edition"_s,
                                            u"Skyrim VR"_s, u"Fallout 4 VR"_s,
                                            u"Starfield"_s, u"Oblivion Remastered"_s }) };
}

QString MO2Notes::author() const { return u"aglowinthefield"_s; }

QString MO2Notes::description() const { return tr("A handy notes panel for MO2"); }

MOBase::VersionInfo MO2Notes::version() const
{
    return MOBase::VersionInfo(0, 0, 1, 0, MOBase::VersionInfo::RELEASE_ALPHA);
}

QList<MOBase::PluginSetting> MO2Notes::settings() const
{
    return {};
}

bool MO2Notes::enabledByDefault() const { return true; }

QWidget* MO2Notes::createWidget(IPanelInterface*, QWidget* parent, const QString& profilePath)
{
    m_NotesWidget = new NotesWidget(parent);
    m_NotesWidget->setProfilePath(profilePath);
    return m_NotesWidget;
}

QString MO2Notes::label() const { return tr("Notes"); }

IPluginPanel::Position MO2Notes::position() const { return Position::atEnd(); }
