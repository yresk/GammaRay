/*
  mainwindow.cpp

  This file is part of Gammaray, the Qt application inspection and
  manipulation tool.

  Copyright (C) 2010-2011 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
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

#include "config-gammaray-version.h"
#include "mainwindow.h"
#include "probe.h"
#include "objectlistmodel.h"
#include "objecttreemodel.h"
#include "objecttypefilterproxymodel.h"
#include "toolmodel.h"
#include "toolfactory.h"
#include "promolabel.h"

#include "kde/krecursivefilterproxymodel.h"

#include <QCoreApplication>
#include <qdebug.h>
#include <QtGui/QStringListModel>
#include <QtCore/qtextcodec.h>
#include <QtGui/QMessageBox>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>

using namespace Gammaray;

static const char progName[] = PROGRAM_NAME;
static const char progVersion[] = GAMMARAY_VERSION_STRING;
static const char progDesc[] = "The Qt application inspection and manipulation tool";
static const char progURL[] = "http://www.kdab.com/gammaray";

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
  ui.setupUi(this);

  connect(ui.actionRetractProbe, SIGNAL(triggered(bool)), SLOT(close()));
  connect(QApplication::instance(), SIGNAL(aboutToQuit()), SLOT(close()));
  connect(ui.actionQuit, SIGNAL(triggered(bool)),
          QApplication::instance(), SLOT(quit()));
  connect(ui.actionAboutQt, SIGNAL(triggered(bool)),
          QApplication::instance(), SLOT(aboutQt()));
  connect(ui.actionAboutGammaray, SIGNAL(triggered(bool)), SLOT(about()));
  connect(ui.actionAboutKDAB, SIGNAL(triggered(bool)), SLOT(aboutKDAB()));

  setWindowIcon(QIcon(":gammaray/gammaray128.png"));

  m_toolSelector = new QComboBox;
  m_toolSelector->setModel(Probe::instance()->toolModel());
  connect(m_toolSelector, SIGNAL(currentIndexChanged(int)), SLOT(toolSelected()));
  connect(m_toolSelector, SIGNAL(activated(int)), SLOT(toolSelected()));
  ui.mainToolBar->addWidget(new QLabel(tr("Select Probe:")));
  ui.mainToolBar->addWidget(m_toolSelector);

  QWidget* promo = new QWidget;
  QHBoxLayout *promoLayout = new QHBoxLayout;
  promoLayout->setContentsMargins(0, 0, 0, 0);
  promoLayout->addStretch();
  promoLayout->addWidget(new PromoLabel(this));
  promo->setLayout(promoLayout);
  ui.mainToolBar->addWidget(promo);

  toolSelected();

  setWindowTitle(tr("%1 (%2)").arg(progName).arg(qApp->applicationName()));
}

void MainWindow::about()
{
  QMessageBox mb(this);
  mb.setWindowTitle(tr("About %1").arg(progName));
  mb.setText(tr("<b>%1 %2</b><p>%3").arg(progName).arg(progVersion).arg(progDesc));
  mb.setInformativeText(
    tr("<qt>Copyright (C) 2010-2011 Klaralvdalens Datakonsult AB, "
       "a KDAB Group company, <a href='mailto:info@kdab.com'>info@kdab.com</a></qt>"));
  mb.setIconPixmap(QPixmap(":gammaray/gammaray128.png"));
  mb.addButton(QMessageBox::Close);
  mb.exec();
}

void MainWindow::aboutKDAB()
{
  QMessageBox mb(this);
  mb.setWindowTitle(tr("About KDAB"));
  mb.setText(tr("Klaralvdalens Datakonsult AB (KDAB)"));
  mb.setInformativeText(
    tr("<qt><p>%1 is supported and maintained by KDAB</p>"
       "<p>Please visit <a href='http://www.kdab.com'>http://www.kdab.com</a> to meet the people who write code like this."
       "We also offer Qt training courses.</p>"
       "<p>Head Engineer for Gammaray is<br>Volker Krause &lt;<a href='mailto:volker.krause@kdab.com'>volker.krause@kdab.com</a>&gt;</p></qt>").
    arg(progName));
  mb.setIconPixmap(QPixmap(":gammaray/kdablogo160.png"));
  mb.addButton(QMessageBox::Close);
  mb.exec();
}

void MainWindow::toolSelected()
{
  const QModelIndex mi = m_toolSelector->model()->index(m_toolSelector->currentIndex(), 0);
  QWidget *toolWidget = mi.data(ToolModel::ToolWidgetRole).value<QWidget*>();
  if (!toolWidget) {
    ToolFactory *toolIface = mi.data(ToolModel::ToolFactoryRole).value<ToolFactory*>();
    Q_ASSERT(toolIface);
    qDebug() << Q_FUNC_INFO << "creating new probe: "
             << toolIface->name() << toolIface->supportedTypes();
    toolWidget = toolIface->createWidget(Probe::instance(), this);
    ui.toolStack->addWidget(toolWidget);
    m_toolSelector->model()->setData(mi, QVariant::fromValue(toolWidget));
  }
  ui.toolStack->setCurrentIndex(ui.toolStack->indexOf(toolWidget));
}

#include "mainwindow.moc"
