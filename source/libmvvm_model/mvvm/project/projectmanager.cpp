// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include <mvvm/interfaces/applicationmodelsinterface.h>
#include <mvvm/interfaces/projectinterface.h>
#include <mvvm/project/projectmanager.h>
#include <mvvm/project/projectutils.h>

using namespace ModelView;

namespace
{
const bool succeeded = true;
const bool failed = false;
} // namespace

struct ProjectManager::ProjectManagerImpl {
    ApplicationModelsInterface* app_models{nullptr};
    std::unique_ptr<ProjectInterface> current_project;
    callback_t m_project_changed;

    ProjectManagerImpl(ApplicationModelsInterface* models, callback_t project_changed)
        : app_models(models), m_project_changed(project_changed)
    {
        createNewProject();
    }

    //! Closes current project. Used in assumption that project was already saved.
    void createNewProject()
    {
        current_project = ProjectUtils::CreateUntitledProject(app_models, m_project_changed);
    }

    //! Returns true if the project has directory already defined.
    bool projectHasDir() const { return !current_project->projectDir().empty(); }

    //! Saves project in project directory. If directory is not defined
    bool saveCurrentProject() { return saveCurrentProjectAs(current_project->projectDir()); }

    //! Saves the project into a given directory.
    bool saveCurrentProjectAs(const std::string& dirname) { return current_project->save(dirname); }

    //! Loads the project from a given directory.
    bool loadFrom(const std::string& dirname) { return current_project->load(dirname); }

    //! Returns true if project has been modified after the last save.
    bool isModified() const { return current_project->isModified(); }
};

//! Constructor for ProjectManager. Requires ApplicationModels and two callbacks to open projects,
//! and create new projects.

ProjectManager::ProjectManager(ApplicationModelsInterface* app_models, callback_t project_changed)
    : p_impl(std::make_unique<ProjectManagerImpl>(app_models, project_changed))
{
}

ProjectManager::~ProjectManager() = default;

//! Creates a new project, returns 'true' in the case of success.
//! Current project has to be in a saved state, otherwise will return false.

bool ProjectManager::createNewProject(const std::string& dirname)
{
    if (p_impl->isModified())
        return failed;
    p_impl->createNewProject();
    return p_impl->saveCurrentProjectAs(dirname);
}

//! Saves current project, returns 'true' in the case of success.
//! The project should have a project directory defined to succeed.

bool ProjectManager::saveCurrentProject()
{
    if (!p_impl->projectHasDir())
        return failed;
    return p_impl->saveCurrentProject();
}

//! Saves the project under a given directory, returns true in the case of success.
//! The directory should exist already.

bool ProjectManager::saveProjectAs(const std::string& dirname)
{
    return p_impl->saveCurrentProjectAs(dirname);
}

//! Opens existing project, returns 'true' in the case of success.
//! Current project should be in a saved state, new project should exist.

bool ProjectManager::openExistingProject(const std::string& dirname)
{
    if (p_impl->isModified())
        return failed;
    p_impl->createNewProject();
    return p_impl->loadFrom(dirname);
}

//! Returns current project directory.

std::string ProjectManager::currentProjectDir() const
{
    return p_impl->current_project ? p_impl->current_project->projectDir() : std::string();
}

//! Returns true if project was modified since last save.

bool ProjectManager::isModified() const
{
    return p_impl->isModified();
}

//! Closes current project (without saving).
//! No checks whether it is modified or not being performed.

bool ProjectManager::closeCurrentProject() const
{
    // no special operation is required to close the project
    p_impl->createNewProject(); // ready for further actions
    return succeeded;
}
