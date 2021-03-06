/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
#include "stdafx.h"

#include "EditorCommon.h"

PropertiesWrapper::PropertiesWrapper(HierarchyWidget* hierarchy,
    EditorWindow* parent)
    : QWidget(parent)
    , m_editorWindow(parent)
    , m_properties(new PropertiesWidget(parent, this))
    , m_componentButton(new ComponentButton(hierarchy, this))
{
    AZ_Assert(parent, "Parent EditorWindow is null");

    QVBoxLayout* outerLayout = new QVBoxLayout(this);

    QVBoxLayout* innerLayout = new QVBoxLayout();
    {
        innerLayout->setContentsMargins(4, 4, 4, 4);
        innerLayout->setSpacing(2);

        QLabel* elementName = new QLabel(this);
        elementName->setObjectName(QStringLiteral("m_elementName"));
        elementName->setText("No Canvas Loaded");

        innerLayout->addWidget(elementName);

        m_properties->SetSelectedEntityDisplayNameWidget(elementName);

        innerLayout->addWidget(m_componentButton);
    }
    outerLayout->addLayout(innerLayout);

    outerLayout->addWidget(m_properties);

    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(0);

    m_properties->setEnabled(false);
    m_componentButton->setEnabled(false);
}

PropertiesWidget* PropertiesWrapper::GetProperties()
{
    return m_properties;
}

void PropertiesWrapper::ActiveCanvasChanged()
{
    // Entity shown in the properties pane has been deleted and a new entity is selected, so trigger an immediate refresh
    m_properties->TriggerImmediateRefresh();

    bool canvasLoaded = m_editorWindow->GetCanvas().IsValid();
    m_properties->setEnabled(canvasLoaded);
    m_componentButton->setEnabled(canvasLoaded);
}

#include <PropertiesWrapper.moc>
