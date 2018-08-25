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

#include "ColorPickerPage.h"
#include <Gallery/ui_ColorPickerPage.h>

#include <AzQtComponents/Components/Widgets/ColorPicker.h>

#include <QDir>
#include <QFileDialog>
#include <QStringList>

namespace
{
    QString getConfigurationName(AzQtComponents::ColorPicker::Configuration configuration)
    {
        switch (configuration)
        {
            case AzQtComponents::ColorPicker::Configuration::A:
                return QStringLiteral("Configuration A");
            case AzQtComponents::ColorPicker::Configuration::B:
                return QStringLiteral("Configuration B");
            case AzQtComponents::ColorPicker::Configuration::C:
                return QStringLiteral("Configuration C");
        }
        Q_UNREACHABLE();
    }
}

ColorPickerPage::ColorPickerPage(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::ColorPickerPage)
    , m_lastColor(0.0f, 1.0f, 0.0f, 0.5f)
{
    ui->setupUi(this);

    QString exampleText = R"(

The ColorPicker is a dialog that allows the user to select a color, with support for a currently selected palette and user managed palettes.<br/>
It can be used as a modal dialog (via ColorPicker::getColor) or as a non-modal dialog (via ColorPicker::show and ColorPicker::selectedColor).<br/>
<br/>
Example:<br/>
<br/>

<pre>
#include &lt;AzQtComponents/Components/Widgets/ColorPicker.h&gt;

// To let the user picker a color:
QColor initialColor { 0, 0, 0, 0 };
QColor newColor = AzQtComponents::ColorPicker::getColor(AzQtComponents::ColorPicker::ConfigurationA, initialColor, "Color Picker");

</pre>

)";

    ui->exampleText->setHtml(exampleText);

    connect(ui->colorPickConfigurationAButton, &QPushButton::released, this, [this] { pickColor(AzQtComponents::ColorPicker::Configuration::A); });
    connect(ui->colorPickConfigurationBButton, &QPushButton::released, this, [this] { pickColor(AzQtComponents::ColorPicker::Configuration::B); });
    connect(ui->colorPickConfigurationCButton, &QPushButton::released, this, [this] { pickColor(AzQtComponents::ColorPicker::Configuration::C); });
    connect(ui->colorPickWithPalettesButton, &QPushButton::clicked, this, &ColorPickerPage::onColorPickWithPalettesButtonClicked);
}

ColorPickerPage::~ColorPickerPage()
{
}

void ColorPickerPage::pickColor(AzQtComponents::ColorPicker::Configuration configuration)
{
    m_lastColor = AzQtComponents::ColorPicker::getColor(configuration, m_lastColor, QStringLiteral("Color Picker (%1)").arg(getConfigurationName(configuration)), QStringList(), this);
}

void ColorPickerPage::onColorPickWithPalettesButtonClicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select a Directory with palettes"),
        QDir::currentPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    m_lastColor = AzQtComponents::ColorPicker::getColor(AzQtComponents::ColorPicker::Configuration::A, m_lastColor, tr("Color Picker With Palettes"), QStringList() << dir, this);
}
