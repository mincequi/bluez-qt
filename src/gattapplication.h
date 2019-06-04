/*
 * BluezQt - Asynchronous BlueZ wrapper library
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

#pragma once

#include "objectmanager.h"

namespace BluezQt
{

/**
 * @class BluezQt::GattApplication GattApplication.h <BluezQt/GattApplication>
 *
 * Bluetooth GattApplication.
 *
 * This class represents a Bluetooth GattApplication.
 */
class BLUEZQT_EXPORT GattApplication : public ObjectManager
{
    Q_OBJECT

public:
    /**
     * Creates a new GattApplication object.
     *
     * @param parent
     */
    explicit GattApplication(QObject *parent = nullptr);

    /**
     * Destroys a GattApplication object.
     */
    ~GattApplication();

protected:
    /**
     * D-Bus object path of the GATT application.
     *
     * The path where the GATT application will be registered.
     *
     * @note You must provide valid object path!
     *
     * @return object path of GATT application
     */
    virtual QDBusObjectPath objectPath() const;

    DBusManagerStruct getManagedObjects() const override;

private:
    class GattApplicationPrivate *const d;

    friend class GattManager;
    friend class GattService;
};

} // namespace BluezQt
