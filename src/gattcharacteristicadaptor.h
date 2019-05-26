/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
 * Copyright (C) 2019 Manuel Weichselbaumer
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

#pragma once

#include <QDBusAbstractAdaptor>

class QDBusObjectPath;

namespace BluezQt
{

class GattCharacteristic;

class GattCharacteristicAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT 
    Q_CLASSINFO("D-Bus Interface", "org.bluez.GattCharacteristic1")
    Q_PROPERTY(QString UUID READ uuid)
    Q_PROPERTY(QDBusObjectPath Service READ service)
    Q_PROPERTY(QStringList Flags READ flags)

public:
    explicit GattCharacteristicAdaptor(GattCharacteristic *parent);

    QString uuid() const;

    QDBusObjectPath service() const;

    QStringList flags() const;

public Q_SLOTS:
    QByteArray ReadValue(const QVariantMap &options);
    void WriteValue(const QByteArray &value, const QVariantMap &options);
    void StartNotify();
    void StopNotify();

private:
    GattCharacteristic* m_gattCharacteristic;
};

} // namespace BluezQt
