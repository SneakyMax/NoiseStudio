#ifndef GLOBAL_CONTEXT_H
#define GLOBAL_CONTEXT_H

#include <QObject>

namespace noise_studio
{
    class PackageViewModel;

    class GlobalContext : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QList packages READ opened_packages NOTIFY opened_packages_changed)

    public:
        GlobalContext();

        void new_package();

        QList<PackageViewModel*> opened_packages();

    signals:
        void opened_packages_changed();

    private:
        QList<PackageViewModel*> packages_;
    };
}



#endif // GLOBAL_CONTEXT_H
