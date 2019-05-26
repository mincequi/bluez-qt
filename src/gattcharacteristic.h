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

#include "bluezqt_export.h"

#include <QDBusObjectPath>

namespace BluezQt
{
class GattService;

class BLUEZQT_EXPORT GattCharacteristic : public QObject
{
    Q_OBJECT

public:
    /**
     * Creates a new GattCharacteristic object.
     *
     * @param parent
     */
    explicit GattCharacteristic(const QString &uuid, GattService *service);

    /**
     * Destroys a GattCharacteristic object.
     */
    ~GattCharacteristic();

    /**
     * Reads the value of the characteristic.
     */
    QByteArray readValue();

    /**
     * Writes the value of the characteristic.
     */
    void writeValue(const QByteArray &value);

    /**
     * Provide a read function to operate in *pull* mode.
     */
    using ReadCallback = std::function<QByteArray()>;
    void setReadCallback(ReadCallback callback);

Q_SIGNALS:
    /**
     * Indicates that a value was written.
     */
    void valueWritten(const QByteArray &value);

private:
    /**
     * 128-bit GATT characteristic UUID.
     *
     * @return uuid of characteristic
     */
    QString uuid() const;

    /**
     * Object path of the GATT service the characteristic belongs to.
     *
     * @return object path of service this characteristic belongs to
     */
    QDBusObjectPath serviceObjectPath() const;
    virtual QDBusObjectPath objectPath() const;

    QString m_uuid;
    const GattService *m_service;
    QDBusObjectPath m_objectPath;
    QByteArray      m_value;
    ReadCallback    m_readCallback = nullptr;

    friend class GattApplication;
    friend class GattCharacteristicAdaptor;
    friend class GattManager;
};

} // namespace BluezQt
