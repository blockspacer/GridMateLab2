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

#ifndef __EMSTUDIO_GAMECONTROLLERWINDOW_H
#define __EMSTUDIO_GAMECONTROLLERWINDOW_H

#include "../StandardPluginsConfig.h"


#include <MCore/Source/StandardHeaders.h>
#include <MCore/Source/Array.h>
#include <AzCore/Debug/Timer.h>
#include <MCore/Source/UnicodeString.h>
#include <MysticQt/Source/DialogStack.h>
#include <EMotionFX/Source/AnimGraph.h>
#include <EMotionFX/Source/AnimGraphGameControllerSettings.h>

#include "AttributesWindow.h"

#ifdef HAS_GAME_CONTROLLER
#include "GameController.h"
#endif

#include <QWidget>
#include <QSplitter>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QBasicTimer>

#include <EMotionFX/CommandSystem/Source/AnimGraphCommands.h>
#include <EMotionFX/CommandSystem/Source/AnimGraphNodeCommands.h>
#include <EMotionFX/CommandSystem/Source/AnimGraphConnectionCommands.h>

#define NO_GAMECONTROLLER_NAME "None"

QT_FORWARD_DECLARE_CLASS(QCheckBox)

namespace EMStudio
{
    // forward declarations
    class AnimGraphPlugin;

    class GameControllerWindow
        : public QWidget
    {
        Q_OBJECT
                 MCORE_MEMORYOBJECTCATEGORY(GameControllerWindow, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_STANDARDPLUGINS_ANIMGRAPH);

    public:
        GameControllerWindow(AnimGraphPlugin* plugin, QWidget* parent = nullptr);
        ~GameControllerWindow();

        void Init();
        void ReInit();
        void DisablePresetInterface();

        MCORE_INLINE bool GetIsGameControllerValid() const
        {
            #ifdef HAS_GAME_CONTROLLER
            if (mGameController == nullptr)
            {
                return false;
            }
            if (mGameControllerComboBox->currentIndex() == 0)
            {
                return false;
            }
            return mGameController->GetIsValid();
            #else
            return false;
            #endif
        }

    private slots:
        void OnGameControllerComboBox(int value);
        void OnParameterModeComboBox(int value);
        void OnAxisComboBox(int value);
        void OnPresetComboBox(int value);
        void OnInvertCheckBoxChanged(int state);
        void OnAddPresetButton();
        void OnRemovePresetButton();
        void OnPresetNameEdited(const QString& text);
        void OnPresetNameChanged();
        void OnDeadZoneSliderChanged(int value);

        void HardwareChangeDetected();

        void OnButtonModeComboBox(int value);
        void OnButtonParameterComboBox(int value);

        void OnSelectNodeButtonClicked();

    private:
        MCORE_DEFINECOMMANDCALLBACK(CommandCreateBlendParameterCallback);
        MCORE_DEFINECOMMANDCALLBACK(CommandRemoveBlendParameterCallback);
        MCORE_DEFINECOMMANDCALLBACK(CommandAdjustBlendParameterCallback);
        MCORE_DEFINECOMMANDCALLBACK(CommandSelectCallback);
        MCORE_DEFINECOMMANDCALLBACK(CommandUnselectCallback);
        MCORE_DEFINECOMMANDCALLBACK(CommandClearSelectionCallback);

        CommandCreateBlendParameterCallback*    mCreateCallback;
        CommandRemoveBlendParameterCallback*    mRemoveCallback;
        CommandAdjustBlendParameterCallback*    mAdjustCallback;
        CommandSelectCallback*                  mSelectCallback;
        CommandUnselectCallback*                mUnselectCallback;
        CommandClearSelectionCallback*          mClearSelectionCallback;

        struct ParameterInfo
        {
            MCORE_MEMORYOBJECTCATEGORY(GameControllerWindow::ParameterInfo, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_STANDARDPLUGINS_ANIMGRAPH);

            MCore::AttributeSettings*           mAttributeSettings;
            MysticQt::ComboBox*                 mAxis;
            MysticQt::ComboBox*                 mMode;
            QCheckBox*                          mInvert;
            QLineEdit*                          mValue;
        };

        struct ButtonInfo
        {
            MCORE_MEMORYOBJECTCATEGORY(GameControllerWindow::ButtonInfo, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_STANDARDPLUGINS_ANIMGRAPH);
            ButtonInfo(uint32 index, QWidget* widget)
            {
                mButtonIndex    = index;
                mWidget         = widget;
            }

            uint32                              mButtonIndex;
            QWidget*                            mWidget;
        };

        ParameterInfo* FindParamInfoByModeComboBox(MysticQt::ComboBox* comboBox);
        ParameterInfo* FindParamInfoByAxisComboBox(MysticQt::ComboBox* comboBox);
        ParameterInfo* FindParamInfoByCheckBox(QCheckBox* checkBox);
        ParameterInfo* FindButtonInfoByAttributeInfo(MCore::AttributeSettings* attributeSettings);
        ButtonInfo* FindButtonInfo(QWidget* widget);

        void ReInitButtonInterface(uint32 buttonIndex);
        void UpdateParameterInterface(ParameterInfo* parameterInfo);
        void UpdateGameControllerComboBox();

        AnimGraphPlugin*               mPlugin;
        MCore::Array<QLabel*>           mPreviewLabels;
        MCore::Array<ParameterInfo>     mParameterInfos;
        MCore::Array<ButtonInfo>        mButtonInfos;
        QBasicTimer                     mInterfaceTimer;
        QBasicTimer                     mGameControllerTimer;
        AZ::Debug::Timer                mDeltaTimer;
        int                             mInterfaceTimerID;
        int                             mGameControllerTimerID;

        #ifdef HAS_GAME_CONTROLLER
        GameController*             mGameController;
        #endif
        EMotionFX::AnimGraph*          mAnimGraph;

        MysticQt::DialogStack*          mDialogStack;

        QWidget*                        mDynamicWidget;
        MysticQt::Slider*               mDeadZoneSlider;
        QLabel*                         mDeadZoneValueLabel;
        QGridLayout*                    mParameterGridLayout;
        QGridLayout*                    mButtonGridLayout;
        MysticQt::ComboBox*             mGameControllerComboBox;

        // preset interface elements
        MysticQt::ComboBox*             mPresetComboBox;
        QLineEdit*                      mPresetNameLineEdit;
        QPushButton*                    mAddPresetButton;
        QPushButton*                    mRemovePresetButton;

        MCore::String                   mString;

        void timerEvent(QTimerEvent* event);
        void InitGameController();
        void AutoSelectGameController();
    };
} // namespace EMStudio

#endif
