/*
  webinspector.cpp

  This file is part of GammaRay, the Qt application inspection and
  manipulation tool.

  Copyright (C) 2010-2013 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Volker Krause <volker.krause@kdab.com>

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

#include "webinspector.h"
#include "webviewmodel.h"

#include "include/objecttypefilterproxymodel.h"
#include "include/probeinterface.h"
#include "include/singlecolumnobjectproxymodel.h"

#include <QtPlugin>

using namespace GammaRay;

WebInspector::WebInspector(ProbeInterface *probe, QObject *parent)
  : QObject(parent)
{
  WebViewModel *webViewModel = new WebViewModel(this);
  webViewModel->setSourceModel(probe->objectListModel());
  probe->registerModel("com.kdab.GammaRay.WebPages", webViewModel);

  qputenv("QTWEBKIT_INSPECTOR_SERVER", "0.0.0.0:11733"); // TODO set based on Server address/port
}

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN(WebInspectorFactory)
#endif

#include "webinspector.moc"
