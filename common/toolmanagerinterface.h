/*
  toolmanagerinterface.h

  This file is part of GammaRay, the Qt application inspection and
  manipulation tool.

  Copyright (C) 2013-2016 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Anton Kreuzkamp <anton.kreuzkamp@kdab.com>

  Licensees holding valid commercial KDAB GammaRay licenses may use this file in
  acuordance with GammaRay Commercial License Agreement provided with the Software.

  Contact info@kdab.com if any conditions of this licensing are not clear to you.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GAMMARAY_TOOLMANAGERINTERFACE_H
#define GAMMARAY_TOOLMANAGERINTERFACE_H

#include "objectid.h"

#include <QObject>
#include <QDataStream>
#include <QDebug>
#include <QMetaType>
#include <QVector>

namespace GammaRay {
/** @brief GammaRay tool identifier. */
struct ToolInfo
{
    QString id;
    bool hasUi;
    bool enabled;
};
typedef QVector<ToolInfo> ToolInfos;

/** @brief Probe and host process remote control functions. */
class ToolManagerInterface : public QObject
{
    Q_OBJECT

public:
    explicit ToolManagerInterface(QObject *parent = nullptr);
    virtual ~ToolManagerInterface();

    virtual void selectObject(ObjectId id, const QString &toolId) = 0;
    virtual void requestToolsForObject(ObjectId id) = 0;
    virtual void requestAvailableTools() = 0;

Q_SIGNALS:
    void toolsForObjectResponse(GammaRay::ObjectId id, const GammaRay::ToolInfos &toolInfos);
    void availableToolsResponse(const GammaRay::ToolInfos &toolInfos);
    void toolEnabled(const QString &toolId);
    void toolSelected(const QString &toolId);

private:
    Q_DISABLE_COPY(ToolManagerInterface)
};

inline QDataStream &operator<<(QDataStream &out, const ToolInfo &toolInfo)
{
    out << toolInfo.id;
    out << toolInfo.hasUi;
    out << toolInfo.enabled;
    return out;
}

inline QDataStream &operator>>(QDataStream &in, ToolInfo &toolInfo)
{
    in >> toolInfo.id;
    in >> toolInfo.hasUi;
    in >> toolInfo.enabled;
    return in;
}
}

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(GammaRay::ToolManagerInterface, "com.kdab.GammaRay.ToolManagerInterface")
QT_END_NAMESPACE
Q_DECLARE_METATYPE(GammaRay::ToolInfo)
Q_DECLARE_METATYPE(GammaRay::ToolInfos)

#endif // GAMMARAY_TOOLMANAGERINTERFACE_H
