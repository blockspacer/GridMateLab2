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
#include "LmbrCentral_precompiled.h"
#include "LmbrCentralEditor.h"
#include "LmbrCentralReflectionTest.h"
#include "Shape/EditorBoxShapeComponent.h"
#include <AzToolsFramework/Application/ToolsApplication.h>

namespace LmbrCentral
{
    // Serialized legacy EditorBoxShapeComponent v1.
    const char kEditorBoxComponentVersion1[] =
        R"DELIMITER(<ObjectStream version="1">
        <Class name="EditorBoxShapeComponent" field="element" version="1" type="{2ADD9043-48E8-4263-859A-72E0024372BF}">
            <Class name="EditorComponentBase" field="BaseClass1" version="1" type="{D5346BD4-7F20-444E-B370-327ACD03D4A0}">
                <Class name="AZ::Component" field="BaseClass1" type="{EDFCB2CF-F75D-43BE-B26B-F35821B29247}">
                    <Class name="AZ::u64" field="Id" value="7702953324769442676" type="{D6597933-47CD-4FC8-B911-63F3E2B0993A}"/>
                </Class>
            </Class>
            <Class name="BoxShapeConfig" field="Configuration" version="1" type="{F034FBA2-AC2F-4E66-8152-14DFB90D6283}">
                <Class name="Vector3" field="Dimensions" value="0.3700000 0.5700000 0.6600000" type="{8379EB7D-01FA-4538-B64B-A6543B4BE73D}"/>
            </Class>
        </Class>
    </ObjectStream>)DELIMITER";

    class LoadEditorBoxShapeComponentFromVersion1
        : public LoadReflectedObjectTest<AZ::ComponentApplication, LmbrCentralEditorModule, EditorBoxShapeComponent>
    {
    protected:
        const char* GetSourceDataBuffer() const override { return kEditorBoxComponentVersion1; }

        void SetUp() override
        {
            LoadReflectedObjectTest::SetUp();

            if (m_object)
            {
                m_editorBoxShapeComponent = m_object.get();
                m_entity.Init();
                m_entity.AddComponent(m_editorBoxShapeComponent);
                m_entity.Activate();
            }
        }

        void TearDown() override
        {
            m_entity.Deactivate();
            LoadReflectedObjectTest::TearDown();
        }

        AZ::Entity m_entity;
        EditorBoxShapeComponent* m_editorBoxShapeComponent = nullptr;
    };

    TEST_F(LoadEditorBoxShapeComponentFromVersion1, Application_IsRunning)
    {
        ASSERT_NE(GetApplication(), nullptr);
    }

    TEST_F(LoadEditorBoxShapeComponentFromVersion1, Components_Load)
    {
        EXPECT_NE(m_object.get(), nullptr);
    }

    TEST_F(LoadEditorBoxShapeComponentFromVersion1, EditorComponent_Found)
    {
       EXPECT_NE(m_editorBoxShapeComponent, nullptr);
    }

    TEST_F(LoadEditorBoxShapeComponentFromVersion1, Dimensions_MatchesSourceData)
    {
        AZ::Vector3 dimensions = AZ::Vector3::CreateZero();
        BoxShapeComponentRequestsBus::EventResult(
            dimensions, m_entity.GetId(), &BoxShapeComponentRequests::GetBoxDimensions);

       EXPECT_EQ(dimensions, AZ::Vector3(0.37, 0.57, 0.66));
    }
}

