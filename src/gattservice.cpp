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

#include "gattservice.h"
#include "gattapplication.h"

namespace BluezQt
{

GattService::GattService(const QString &uuid, bool isPrimary, GattApplication *parent)
    : QObject(parent),
      m_uuid(uuid),
      m_isPrimary(isPrimary)
{
    static uint8_t serviceNumber = 0;
    m_objectPath.setPath(parent->objectPath().path() + QStringLiteral("/service") + QString::number(serviceNumber++));
}

GattService::~GattService()
{
}

QString GattService::uuid() const
{
    return m_uuid;
}

bool GattService::isPrimary() const
{
    return m_isPrimary;
}

QDBusObjectPath GattService::objectPath() const
{
    return m_objectPath;
}

} // namespace BluezQt
