/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Csaba Pinter, PerkLab, Queen's University
  and was supported through the Applied Cancer Research Unit program of Cancer Care
  Ontario with funds provided by the Ontario Ministry of Health and Long-Term Care

==============================================================================*/

// SubjectHierarchy MRML includes
#include "vtkMRMLSubjectHierarchyNode.h"
#include "vtkMRMLSubjectHierarchyConstants.h"

// SubjectHierarchy Plugins includes
#include "qSlicerSubjectHierarchyPluginHandler.h"
#include "qSlicerSubjectHierarchyMarkupsPlugin.h"
#include "qSlicerSubjectHierarchyDefaultPlugin.h"

// MRML includes
#include <vtkMRMLNode.h>
#include <vtkMRMLScene.h>

// MRML widgets includes
#include "qMRMLNodeComboBox.h"

// VTK includes
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>

// Qt includes
#include <QDebug>
#include <QStandardItem>
#include <QAction>

// SlicerQt includes
#include "qSlicerAbstractModuleWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_SubjectHierarchy_Plugins
class qSlicerSubjectHierarchyMarkupsPluginPrivate: public QObject
{
  Q_DECLARE_PUBLIC(qSlicerSubjectHierarchyMarkupsPlugin);
protected:
  qSlicerSubjectHierarchyMarkupsPlugin* const q_ptr;
public:
  qSlicerSubjectHierarchyMarkupsPluginPrivate(qSlicerSubjectHierarchyMarkupsPlugin& object);
  ~qSlicerSubjectHierarchyMarkupsPluginPrivate();
  void init();
public:
  QIcon MarkupIcon;
};

//-----------------------------------------------------------------------------
// qSlicerSubjectHierarchyMarkupsPluginPrivate methods

//-----------------------------------------------------------------------------
qSlicerSubjectHierarchyMarkupsPluginPrivate::qSlicerSubjectHierarchyMarkupsPluginPrivate(qSlicerSubjectHierarchyMarkupsPlugin& object)
: q_ptr(&object)
{
  this->MarkupIcon = QIcon(":Icons/Markup.png");
}

//-----------------------------------------------------------------------------
qSlicerSubjectHierarchyMarkupsPluginPrivate::~qSlicerSubjectHierarchyMarkupsPluginPrivate()
{
}

//-----------------------------------------------------------------------------
qSlicerSubjectHierarchyMarkupsPlugin::qSlicerSubjectHierarchyMarkupsPlugin(QObject* parent)
 : Superclass(parent)
 , d_ptr( new qSlicerSubjectHierarchyMarkupsPluginPrivate(*this) )
{
  this->m_Name = QString("Markups");

  Q_D(qSlicerSubjectHierarchyMarkupsPlugin);
  d->init();
}

//------------------------------------------------------------------------------
void qSlicerSubjectHierarchyMarkupsPluginPrivate::init()
{
}

//-----------------------------------------------------------------------------
qSlicerSubjectHierarchyMarkupsPlugin::~qSlicerSubjectHierarchyMarkupsPlugin()
{
}

//----------------------------------------------------------------------------
double qSlicerSubjectHierarchyMarkupsPlugin::canAddNodeToSubjectHierarchy(vtkMRMLNode* node, vtkMRMLSubjectHierarchyNode* parent/*=NULL*/)const
{
  Q_UNUSED(parent);
  if (!node)
    {
    qCritical() << "qSlicerSubjectHierarchyMarkupsPlugin::canAddNodeToSubjectHierarchy: Input node is NULL!";
    return 0.0;
    }
  else if (node->IsA("vtkMRMLMarkupsFiducialNode"))
    {
    // Node is a markup fiducial
    return 0.5;
    }
  return 0.0;
}

//---------------------------------------------------------------------------
double qSlicerSubjectHierarchyMarkupsPlugin::canOwnSubjectHierarchyNode(vtkMRMLSubjectHierarchyNode* node)const
{
  if (!node)
    {
    qCritical() << "qSlicerSubjectHierarchyMarkupsPlugin::canOwnSubjectHierarchyNode: Input node is NULL!";
    return 0.0;
    }

  // Chart
  vtkMRMLNode* associatedNode = node->GetAssociatedNode();
  if (associatedNode && associatedNode->IsA("vtkMRMLMarkupsFiducialNode"))
    {
    return 0.5; // There may be other plugins that can handle special charts better
    }

  return 0.0;
}

//---------------------------------------------------------------------------
const QString qSlicerSubjectHierarchyMarkupsPlugin::roleForPlugin()const
{
  return "Markup";
}

//---------------------------------------------------------------------------
bool qSlicerSubjectHierarchyMarkupsPlugin::setIcon(vtkMRMLSubjectHierarchyNode* node, QStandardItem* item)
{
  if (!node || !item)
    {
    qCritical() << "qSlicerSubjectHierarchyMarkupsPlugin::setIcon: NULL node or item given!";
    return false;
    }

  Q_D(qSlicerSubjectHierarchyMarkupsPlugin);

  if (this->canOwnSubjectHierarchyNode(node))
    {
    item->setIcon(d->MarkupIcon);
    return true;
    }

  // Node unknown by plugin
  return false;
}

//---------------------------------------------------------------------------
void qSlicerSubjectHierarchyMarkupsPlugin::setVisibilityIcon(vtkMRMLSubjectHierarchyNode* node, QStandardItem* item)
{
  // Have the default plugin (which is not registered) take care of this
  qSlicerSubjectHierarchyPluginHandler::instance()->defaultPlugin()->setVisibilityIcon(node, item);
}

//---------------------------------------------------------------------------
void qSlicerSubjectHierarchyMarkupsPlugin::editProperties(vtkMRMLSubjectHierarchyNode* node)
{
  // Switch to markups module
  qSlicerAbstractModuleWidget* moduleWidget = qSlicerSubjectHierarchyAbstractPlugin::switchToModule("Markups");
  if (moduleWidget)
    {
    // Get node selector combobox
    qMRMLNodeComboBox* nodeSelector = moduleWidget->findChild<qMRMLNodeComboBox*>("activeMarkupMRMLNodeComboBox");

    // Choose current data node
    if (nodeSelector)
      {
      nodeSelector->setCurrentNode(node->GetAssociatedNode());
      }
    }
}
