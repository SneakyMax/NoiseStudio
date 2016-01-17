#ifndef PACKAGE_VIEW_MODEL_H
#define PACKAGE_VIEW_MODEL_H

#include <QObject>

namespace noise_studio
{
    class PackageViewModel : public QObject
    {
        Q_OBJECT

    public:
        PackageViewModel(QObject* parent = 0);
    };
}

#endif // PACKAGE_VIEW_MODEL_H
