#include "stdafx.h"
#include "mount_manager.h"
#include "../../common/tables.h"
#include "item.h"

#ifdef MOUNT_BONUS_SYSTEM
//////////////////////////////////////////////////////////////////////////
// CMountProto
//////////////////////////////////////////////////////////////////////////

CMountProto::CMountProto()
{
	memset(&m_table, 0, sizeof(m_table));
}

CMountProto::~CMountProto()
{
}

const TItemApply* CMountProto::GetApply(int index) const
{
	if (index < 0 || index >= MOUNT_APPLY_MAX_NUM)
		return NULL;

	const TItemApply* apply = &m_table.aApplies[index];

	// Return NULL if this apply is empty (APPLY_NONE)
	if (apply->bType == APPLY_NONE)
		return NULL;

	return apply;
}

int CMountProto::GetApplyCount() const
{
	int count = 0;

	for (int i = 0; i < MOUNT_APPLY_MAX_NUM; ++i)
	{
		if (m_table.aApplies[i].bType != APPLY_NONE)
			count++;
	}

	return count;
}

//////////////////////////////////////////////////////////////////////////
// CMountManager
//////////////////////////////////////////////////////////////////////////

CMountManager::CMountManager()
{
}

CMountManager::~CMountManager()
{
	Destroy();
}

bool CMountManager::Initialize(TMountTable* table, int size)
{
	// Clear existing data
	Destroy();

	sys_log(0, "MOUNT_MANAGER: Initializing with %d mount protos", size);

	// Load each mount proto
	for (int i = 0; i < size; ++i)
	{
		CMountProto* proto = new CMountProto();
		if (!proto)
		{
			sys_err("MOUNT_MANAGER: Failed to allocate CMountProto");
			continue;
		}

		// Copy table data
		memcpy(&proto->m_table, &table[i], sizeof(TMountTable));

		// Index by vnum
		m_map_pkMountByVnum[proto->GetVnum()] = proto;

		// Index by name (for debugging/lookup)
		if (proto->GetName() && strlen(proto->GetName()) > 0)
			m_map_pkMountByName[proto->GetName()] = proto;

		// Log mount details
		sys_log(0, "MOUNT_MANAGER: Loaded mount #%u '%s' (level: %d, speed: %d, applies: %d)",
			proto->GetVnum(),
			proto->GetName(),
			proto->GetLevel(),
			proto->GetMovSpeed(),
			proto->GetApplyCount()
		);

		// Log individual applies for debugging
		for (int j = 0; j < MOUNT_APPLY_MAX_NUM; ++j)
		{
			const TItemApply* apply = proto->GetApply(j);
			if (apply)
			{
				sys_log(0, "MOUNT_MANAGER:   - Apply[%d]: type=%d, value=%d",
					j, apply->bType, apply->lValue);
			}
		}
	}

	sys_log(0, "MOUNT_MANAGER: Initialized successfully (%d mounts loaded)", m_map_pkMountByVnum.size());
	return true;
}

void CMountManager::Destroy()
{
	// Free all mount protos
	for (iterator it = m_map_pkMountByVnum.begin(); it != m_map_pkMountByVnum.end(); ++it)
	{
		CMountProto* proto = it->second;
		if (proto)
			delete proto;
	}

	m_map_pkMountByVnum.clear();
	m_map_pkMountByName.clear();
}

const CMountProto* CMountManager::Get(DWORD dwVnum) const
{
	std::map<DWORD, CMountProto*>::const_iterator it = m_map_pkMountByVnum.find(dwVnum);

	if (it == m_map_pkMountByVnum.end())
		return NULL;

	return it->second;
}

const CMountProto* CMountManager::Get(const char* name) const
{
	if (!name || strlen(name) == 0)
		return NULL;

	std::map<std::string, CMountProto*>::const_iterator it = m_map_pkMountByName.find(name);

	if (it == m_map_pkMountByName.end())
		return NULL;

	return it->second;
}

#endif