/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
 * Copyright (C) 2018 Manuel Weichselbaumer <mincequi@web.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "gattapplication.h"

#include <QDBusObjectPath>
#include <QDebug>
#include <QMetaClassInfo>

#include "gattcharacteristic.h"
#include "gattcharacteristicadaptor.h"
#include "gattservice.h"
#include "gattserviceadaptor.h"

namespace BluezQt
{

GattApplication::GattApplication(QObject *parent) :
    ObjectManager(parent)
{
    m_objectPath.setPath(QStringLiteral("/org/bluez"));
}

GattApplication::~GattApplication()
{
}

DBusManagerStruct GattApplication::getManagedObjects() const
{
    DBusManagerStruct objects;

    auto serviceAdaptors = findChildren<GattServiceAdaptor*>();
    auto charcAdaptors = findChildren<GattCharacteristicAdaptor*>();

    for (const GattServiceAdaptor *serviceAdaptor : serviceAdaptors) {
        QVariantMap properties;
        for (int i = serviceAdaptor->metaObject()->propertyOffset();
             i < serviceAdaptor->metaObject()->propertyCount(); ++i) {
            auto propertyName = serviceAdaptor->metaObject()->property(i).name();
            properties.insert(QString::fromLatin1(propertyName), serviceAdaptor->property(propertyName));
        }

        GattService* service = qobject_cast<GattService*>(serviceAdaptor->parent());
        if (service) {
            objects[service->objectPath()].insert(QStringLiteral("org.bluez.GattService1"), properties);
        }
    }

    for (const GattCharacteristicAdaptor *charcAdaptor : charcAdaptors) {
        QVariantMap properties;
        for (int i = charcAdaptor->metaObject()->propertyOffset();
             i < charcAdaptor->metaObject()->propertyCount(); ++i) {
            auto propertyName = charcAdaptor->metaObject()->property(i).name();
            properties.insert(QString::fromLatin1(propertyName), charcAdaptor->property(propertyName));
        }

        GattCharacteristic *charc = qobject_cast<GattCharacteristic*>(charcAdaptor->parent());
        if (charc) {
            objects[charc->objectPath()].insert(QStringLiteral("org.bluez.GattCharacteristic1"), properties);
        }
    }

    return objects;
}

QDBusObjectPath GattApplication::objectPath() const
{
    return m_objectPath;
}

} // namespace BluezQt
