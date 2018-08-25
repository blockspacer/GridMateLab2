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

#ifndef CRYINCLUDE_EDITOR_BASELIBRARY_H
#define CRYINCLUDE_EDITOR_BASELIBRARY_H
#pragma once

#include "Include/IDataBaseLibrary.h"
#include "Include/IBaseLibraryManager.h"
#include "Include/EditorCoreAPI.h"
#include "Util/TRefCountBase.h"

#include <QObject>

// Ensure we don't try to dllimport when moc includes us
#if defined(Q_MOC_BUILD) && !defined(EDITOR_CORE)
#define EDITOR_CORE
#endif

/** This a base class for all Libraries used by Editor.
*/
class EDITOR_CORE_API CBaseLibrary
    : public QObject
    , public TRefCountBase<IDataBaseLibrary>
{
    Q_OBJECT

public:
    explicit CBaseLibrary(IBaseLibraryManager* pManager);
    ~CBaseLibrary();

    //! Set library name.
    virtual void SetName(const QString& name);
    //! Get library name.
    const QString& GetName() const;

    //! Set new filename for this library.
    virtual bool SetFilename(const QString& filename, bool checkForUnique = true) { m_filename = filename; m_filename.toLower(); return true; };
    const QString& GetFilename() const { return m_filename; };

    virtual bool Save() = 0;
    virtual bool Load(const QString& filename) = 0;
    virtual void Serialize(XmlNodeRef& node, bool bLoading) = 0;

    //! Mark library as modified.
    void SetModified(bool bModified = true);
    //! Check if library was modified.
    bool IsModified() const { return m_bModified; };

    //////////////////////////////////////////////////////////////////////////
    // Working with items.
    //////////////////////////////////////////////////////////////////////////
    //! Add a new prototype to library.
    void AddItem(IDataBaseItem* item, bool bRegister = true);
    //! Get number of known prototypes.
    int GetItemCount() const { return m_items.size(); }
    //! Get prototype by index.
    IDataBaseItem* GetItem(int index);

    //! Delete item by pointer of item.
    void RemoveItem(IDataBaseItem* item);

    //! Delete all items from library.
    void RemoveAllItems();

    //! Find library item by name.
    //! Using linear search.
    IDataBaseItem* FindItem(const QString& name);

    //! Check if this library is local level library.
    bool IsLevelLibrary() const { return m_bLevelLib; };

    //! Set library to be level library.
    void SetLevelLibrary(bool bEnable) { m_bLevelLib = bEnable; };

    //////////////////////////////////////////////////////////////////////////
    //! Return manager for this library.
    IBaseLibraryManager* GetManager();

    // Saves the library with the main tag defined by the parameter name
    bool SaveLibrary(const char* name, bool saveEmptyLibrary = false);

    //CONFETTI BEGIN
    // Used to change the library item order
    virtual void ChangeItemOrder(CBaseLibraryItem* item, unsigned int newLocation) override;
    //CONFETTI END

signals:
    void Modified(bool bModified);

private:
    // Add the library to the source control
    bool AddLibraryToSourceControl(const QString& fullPathName) const;

protected:

    //! Name of the library.
    QString m_name;
    //! Filename of the library.
    QString m_filename;

    //! Flag set when library was modified.
    bool m_bModified;

    // Flag set when the library is just created and it's not yet saved for the first time.
    bool m_bNewLibrary;

    //! Level library is saved within the level .ly file and is local for this level.
    bool m_bLevelLib;

    //////////////////////////////////////////////////////////////////////////
    // Manager.
    IBaseLibraryManager* m_pManager;

    // Array of all our library items.
    std::vector<_smart_ptr<CBaseLibraryItem> > m_items;
};

#endif // CRYINCLUDE_EDITOR_BASELIBRARY_H
