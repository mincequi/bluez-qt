#include "adapter_p.h"
#include "adapter.h"
#include "utils_p.h"

namespace QBluez
{

AdapterPrivate::AdapterPrivate(const QString &path, const QVariantMap &properties, Adapter *parent)
    : QObject(parent)
    , q(parent)
    , m_dbusProperties(0)
    , m_adapterClass(0)
    , m_powered(0)
    , m_discoverable(false)
    , m_discoverableTimeout(0)
    , m_pairable(false)
    , m_pairableTimeout(0)
{
    m_bluezAdapter = new BluezAdapter(Strings::orgBluez(), path, QDBusConnection::systemBus(), this);

    init(properties);
}

void AdapterPrivate::init(const QVariantMap &properties)
{
    m_dbusProperties = new DBusProperties(Strings::orgBluez(), m_bluezAdapter->path(),
                                          QDBusConnection::systemBus(), this);

    // QueuedConnection is important here to be able to perform actions, that depend on
    // a previously set property, directly from slot connected to propertyChanged signal.
    // Eg. Powering on adapter and then starting discovery.
    //  * with DirectConnection the StartDiscovery would fail because the adapter is still
    //    powered off when the PropertiesChanged signal is emitted ...
    connect(m_dbusProperties, &DBusProperties::PropertiesChanged,
            this, &AdapterPrivate::propertiesChanged, Qt::QueuedConnection);

    // Init properties
    m_address = properties.value(QStringLiteral("Address")).toString();
    m_name = properties.value(QStringLiteral("Name")).toString();
    m_alias = properties.value(QStringLiteral("Alias")).toString();
    m_adapterClass = properties.value(QStringLiteral("Class")).toUInt();
    m_powered = properties.value(QStringLiteral("Powered")).toBool();
    m_discoverable = properties.value(QStringLiteral("Discoverable")).toBool();
    m_discoverableTimeout = properties.value(QStringLiteral("DiscoverableTimeout")).toUInt();
    m_pairable = properties.value(QStringLiteral("Pairable")).toBool();
    m_pairableTimeout = properties.value(QStringLiteral("PairableTimeout")).toUInt();
    m_discovering = properties.value(QStringLiteral("Discovering")).toBool();
    m_uuids = stringListToUpper(properties.value(QStringLiteral("UUIDs")).toStringList());
    m_modalias = properties.value(QStringLiteral("Modalias")).toString();
}

void AdapterPrivate::addDevice(Device *device)
{
    m_devices.append(device);
    Q_EMIT q->deviceFound(device);
}

void AdapterPrivate::removeDevice(Device *device)
{
    m_devices.removeOne(device);
    Q_EMIT q->deviceRemoved(device);
}

QDBusPendingReply<> AdapterPrivate::setDBusProperty(const QString &name, const QVariant &value)
{
    return m_dbusProperties->Set(Strings::orgBluezAdapter1(), name, QDBusVariant(value));
}

// Make sure not to emit propertyChanged signal when the property already contains changed value
#define PROPERTY_CHANGED(var, type_cast, signal) \
    if (var != value.type_cast()) { \
        var = value.type_cast(); \
        Q_EMIT q->signal(var); \
    }

void AdapterPrivate::propertiesChanged(const QString &interface, const QVariantMap &changed, const QStringList &invalidated)
{
    Q_UNUSED(interface)
    Q_UNUSED(invalidated)

    QVariantMap::const_iterator i;
    for (i = changed.constBegin(); i != changed.constEnd(); ++i) {
        const QVariant &value = i.value();
        const QString &property = i.key();

        if (property == QLatin1String("Name")) {
            PROPERTY_CHANGED(m_name, toString, nameChanged);
        } else if (property == QLatin1String("Alias")) {
            PROPERTY_CHANGED(m_alias, toString, aliasChanged);
        } else if (property == QLatin1String("Class")) {
            PROPERTY_CHANGED(m_adapterClass, toUInt, adapterClassChanged);
        } else if (property == QLatin1String("Powered")) {
            PROPERTY_CHANGED(m_powered, toBool, poweredChanged);
        } else if (property == QLatin1String("Discoverable")) {
            PROPERTY_CHANGED(m_discoverable, toBool, discoverableChanged);
        } else if (property == QLatin1String("DiscoverableTimeout")) {
            PROPERTY_CHANGED(m_discoverableTimeout, toUInt, discoverableTimeoutChanged);
        } else if (property == QLatin1String("Pairable")) {
            PROPERTY_CHANGED(m_pairable, toBool, pairableChanged);
        } else if (property == QLatin1String("PairableTimeout")) {
            PROPERTY_CHANGED(m_pairableTimeout, toUInt, pairableTimeoutChanged);
        } else if (property == QLatin1String("Discovering")) {
            PROPERTY_CHANGED(m_discovering, toBool, discoveringChanged);
        } else if (property == QLatin1String("Modalias")) {
            PROPERTY_CHANGED(m_modalias, toString, modaliasChanged);
        } else if (property == QLatin1String("UUIDs")) {
            uuidsPropertyChanged(stringListToUpper(value.toStringList()));
        }
    }

    Q_EMIT q->adapterChanged(q);
}

void AdapterPrivate::uuidsPropertyChanged(const QStringList &value)
{
    if (m_uuids != value) {
        m_uuids = value;
        Q_EMIT q->uuidsChanged(m_uuids);
    }
}

#undef PROPERTY_CHANGED

} // namespace QBluez
