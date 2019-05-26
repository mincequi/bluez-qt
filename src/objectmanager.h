/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * Copyright (C) 2019 Manuel Weichselbaumer <mincequi@web.de>
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

#include <QObject>

#include "bluezqt_dbustypes.h"
#include "bluezqt_export.h"

namespace BluezQt
{

/**
 * @class BluezQt::ObjectManager ObjectManager.h <BluezQt/ObjectManager>
 *
 * Bluetooth ObjectManager.
 *
 * An API can optionally make use of this interface for one or more sub-trees of
 * objects. The root of each sub-tree implements this interface so other
 * applications can get all objects, interfaces and properties in a single
 * method call. It is appropriate to use this interface if users of the tree of
 * objects are expected to be interested in all interfaces of all objects in the
 * tree; a more granular API should be used if users of the objects are expected
 * to be interested in a small subset of the objects, a small subset of their
 * interfaces, or both.
 */
class BLUEZQT_EXPORT ObjectManager : public QObject
{
public:
    /**
     * Destroys a ObjectManager object.
     */
    virtual ~ObjectManager();

    /**
      * Gets all objects, interfaces and properties.
      *
      * The return value of this method is a dict whose keys are object paths.
      * All returned object paths are children of the object path implementing
      * this interface, i.e. their object paths start with the ObjectManager's
      * object path plus '/'.
      *
      * Each value is a dict whose keys are interfaces names. Each value in this
      * inner dict is another dict with property names (as key) and property
      * values (as value).
     */
    virtual DBusManagerStruct getManagedObjects() const = 0;

protected:
    explicit ObjectManager(QObject *parent = nullptr);
};

} // namespace BluezQt
