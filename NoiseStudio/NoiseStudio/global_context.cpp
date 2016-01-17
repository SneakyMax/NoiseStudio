#include "global_context.h"

#include "package_view_model.h"

using namespace noise_studio;

GlobalContext::GlobalContext()
{
}

void GlobalContext::new_package()
{
    PackageViewModel* package = new PackageViewModel(this);

    packages_.push_back(package);
}

QList<PackageViewModel*> GlobalContext::opened_packages()
{
    return packages_;
}

