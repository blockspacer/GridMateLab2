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
// Original file Copyright Crytek GMBH or its affiliates, used under license.

#include "Maestro_precompiled.h"
#include <AzCore/Serialization/SerializeContext.h>
#include "ScreenFaderTrack.h"
#include <IRenderer.h>

//-----------------------------------------------------------------------------
CScreenFaderTrack::CScreenFaderTrack()
{
    m_lastTextureID = -1;
    SetScreenFaderTrackDefaults();
}

//-----------------------------------------------------------------------------
CScreenFaderTrack::~CScreenFaderTrack()
{
    ReleasePreloadedTextures();
}

//-----------------------------------------------------------------------------
void CScreenFaderTrack::GetKeyInfo(int key, const char*& description, float& duration)
{
    static char desc[32];
    assert(key >= 0 && key < (int)m_keys.size());
    CheckValid();
    description = 0;
    duration = m_keys[key].m_fadeTime;
    cry_strcpy(desc, m_keys[key].m_fadeType == IScreenFaderKey::eFT_FadeIn ? "In" : "Out");

    description = desc;
}

//-----------------------------------------------------------------------------
void CScreenFaderTrack::SerializeKey(IScreenFaderKey& key, XmlNodeRef& keyNode, bool bLoading)
{
    if (bLoading)
    {
        keyNode->getAttr("fadeTime", key.m_fadeTime);
        Vec3 color(0, 0, 0);
        keyNode->getAttr("fadeColor", color);
        key.m_fadeColor = AZ::Color(color.x, color.y, color.z, 1.f);
        int fadeType = 0;
        keyNode->getAttr("fadeType", fadeType);
        key.m_fadeType = IScreenFaderKey::EFadeType(fadeType);
        int fadeChangeType(0);
        if (keyNode->getAttr("fadeChangeType", fadeChangeType))
        {
            key.m_fadeChangeType = IScreenFaderKey::EFadeChangeType(fadeChangeType);
        }
        else
        {
            key.m_fadeChangeType = IScreenFaderKey::eFCT_Linear;
        }
        const char* str;
        str = keyNode->getAttr("texture");
        key.m_strTexture = str;
        keyNode->getAttr("useCurColor", key.m_bUseCurColor);
    }
    else
    {
        keyNode->setAttr("fadeTime", key.m_fadeTime);
        Vec3 color(key.m_fadeColor.GetR(), key.m_fadeColor.GetG(), key.m_fadeColor.GetB());
        keyNode->setAttr("fadeColor", color);
        keyNode->setAttr("fadeType", (int)key.m_fadeType);
        keyNode->setAttr("fadeChangeType", (int)key.m_fadeChangeType);
        keyNode->setAttr("texture", key.m_strTexture.c_str());
        keyNode->setAttr("useCurColor", key.m_bUseCurColor);
    }
}

//-----------------------------------------------------------------------------
void CScreenFaderTrack::SetFlags(int flags)
{
    // call base class implementation. I'm avoiding the use of the Microsoft specific __super::SetFlags(flags) because it is not
    // platform agnostic
    TAnimTrack<IScreenFaderKey>::SetFlags(flags);

    if (flags & IAnimTrack::eAnimTrackFlags_Disabled)
    {
        // when we disable, 'clear' the screen fader effect to avoid the possibility of leaving the Editor in a faded state
        m_bTextureVisible = false;
        m_drawColor = Vec4(.0f, .0f, .0f, .0f);
    }
}

//-----------------------------------------------------------------------------
void CScreenFaderTrack::PreloadTextures()
{
    if (!m_preloadedTextures.empty())
    {
        ReleasePreloadedTextures();
    }

    const int nKeysCount = GetNumKeys();
    if (nKeysCount > 0)
    {
        m_preloadedTextures.reserve(nKeysCount);
        for (int nKeyIndex = 0; nKeyIndex < nKeysCount; ++nKeyIndex)
        {
            IScreenFaderKey key;
            GetKey(nKeyIndex, &key);
            if (!key.m_strTexture.empty())
            {
                ITexture* pTexture = gEnv->pRenderer->EF_LoadTexture(key.m_strTexture.c_str(), FT_DONT_STREAM | FT_STATE_CLAMP);
                if (pTexture)
                {
                    pTexture->SetClamp(true);
                    m_preloadedTextures.push_back(pTexture);
                }
            }
            else
            {
                m_preloadedTextures.push_back(NULL);
            }
        }
    }
}

//-----------------------------------------------------------------------------
ITexture* CScreenFaderTrack::GetActiveTexture() const
{
    int index = GetLastTextureID();
    return (index >= 0 && (size_t) index < m_preloadedTextures.size()) ? m_preloadedTextures[index] : 0;
}

void CScreenFaderTrack::SetScreenFaderTrackDefaults()
{
    m_bTextureVisible = false;
    m_drawColor = Vec4(1, 1, 1, 1);
}
//-----------------------------------------------------------------------------
void CScreenFaderTrack::ReleasePreloadedTextures()
{
    const size_t size = m_preloadedTextures.size();
    for (size_t i = 0; i < size; ++i)
    {
        SAFE_RELEASE(m_preloadedTextures[i]);
    }
    m_preloadedTextures.resize(0);
}

//-----------------------------------------------------------------------------
bool CScreenFaderTrack::SetActiveTexture(int index)
{
    ITexture* pTexture = GetActiveTexture();

    SetLastTextureID(index);

    // Check if textures should be reloaded.
    bool bNeedTexReload = pTexture == NULL; // Not yet loaded
    if (pTexture)
    {
        IScreenFaderKey key;
        GetKey(index, &key);
        if (strcmp(key.m_strTexture.c_str(), pTexture->GetName()) != 0)
        {
            bNeedTexReload = true;                // Loaded, but a different texture
        }
    }
    if (bNeedTexReload)
    {
        // Ok, try to reload.
        PreloadTextures();
        pTexture = GetActiveTexture();
    }
    return pTexture != 0;
}

//////////////////////////////////////////////////////////////////////////
template<>
inline void TAnimTrack<IScreenFaderKey>::Reflect(AZ::SerializeContext* serializeContext)
{
    serializeContext->Class<TAnimTrack<IScreenFaderKey> >()
        ->Version(1)
        ->Field("Flags", &TAnimTrack<IScreenFaderKey>::m_flags)
        ->Field("Range", &TAnimTrack<IScreenFaderKey>::m_timeRange)
        ->Field("ParamType", &TAnimTrack<IScreenFaderKey>::m_nParamType)
        ->Field("Keys", &TAnimTrack<IScreenFaderKey>::m_keys);
}

//////////////////////////////////////////////////////////////////////////
void CScreenFaderTrack::Reflect(AZ::SerializeContext* serializeContext)
{
    TAnimTrack<IScreenFaderKey>::Reflect(serializeContext);

    serializeContext->Class<CScreenFaderTrack, TAnimTrack<IScreenFaderKey> >()
        ->Version(1);
}