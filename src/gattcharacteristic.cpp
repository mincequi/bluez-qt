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

#include "gattcharacteristic.h"
#include "gattservice.h"

namespace BluezQt
{

GattCharacteristic::GattCharacteristic(const QString &uuid, GattService *service)
    : QObject(service),
      m_uuid(uuid),
      m_service(service)
{
    static uint8_t charcNumber = 0;
    m_objectPath.setPath(m_service->objectPath().path() + QStringLiteral("/char") + QString::number(charcNumber++));
}

GattCharacteristic::~GattCharacteristic()
{
}

QByteArray GattCharacteristic::readValue()
{
    if (m_readCallback) {
        m_value = m_readCallback();
    }

    return m_value;
}

void GattCharacteristic::writeValue(const QByteArray& value)
{
    m_value = value;
    emit valueWritten(m_value);
}

QString GattCharacteristic::uuid() const
{
    return m_uuid;
}

QDBusObjectPath GattCharacteristic::serviceObjectPath() const
{
    return m_service->objectPath();
}

QDBusObjectPath GattCharacteristic::objectPath() const
{
    return m_objectPath;
}

void GattCharacteristic::setReadCallback(ReadCallback callback)
{
    m_readCallback = callback;
}

} // namespace BluezQt
