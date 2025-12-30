#include "../../common/singleton.h"
#include "../../common/tables.h"

#ifdef MOUNT_BONUS_SYSTEM

/**
 * @brief Mount prototype wrapper - contains parsed mount data
 *
 * This class wraps the TMountTable structure and provides helper methods
 * for accessing mount properties. Mount protos are loaded from mount_proto.txt
 * at server startup and managed by CMountManager.
 */
class CMountProto
{
	public:
		CMountProto();
		~CMountProto();

		TMountTable	m_table;	// The proto table data

		// Helper methods for accessing mount properties
		DWORD		GetVnum() const			{ return m_table.dwVnum; }
		const char*	GetName() const			{ return m_table.szName; }
		const char*	GetLocaleName() const	{ return m_table.szLocaleName; }
		uint8_t		GetMovSpeed() const		{ return m_table.bMovementSpeed; }
		uint8_t		GetLevel() const		{ return m_table.bLevel; }

		/**
		 * Get specific apply bonus by index
		 * @param index Index (0 to MOUNT_APPLY_MAX_NUM-1)
		 * @return Pointer to TItemApply or NULL if index invalid or apply is APPLY_NONE
		 */
		const TItemApply* GetApply(int index) const;

		/**
		 * Count non-APPLY_NONE bonuses
		 * @return Number of active bonuses
		 */
		int GetApplyCount() const;
};

/**
 * @brief Mount manager singleton - manages all mount prototypes
 *
 * This manager is responsible for loading and maintaining mount proto data.
 * Mount protos are loaded from mount_proto.txt during server boot and can be
 * looked up by vnum or name at runtime.
 *
 * Usage:
 *   const CMountProto* proto = CMountManager::instance().Get(20110);
 *   if (proto)
 *       sys_log(0, "Mount speed: %d", proto->GetMovSpeed());
 */
class CMountManager : public singleton<CMountManager>
{
	public:
		typedef std::map<DWORD, CMountProto*>::iterator iterator;

		CMountManager();
		virtual ~CMountManager();

		/**
		 * Initialize mount manager from proto table
		 * Called from input_db.cpp during boot sequence
		 * @param table Array of TMountTable structs
		 * @param size Number of entries in the array
		 * @return true if successful
		 */
		bool Initialize(TMountTable* table, int size);

		/**
		 * Destroy all loaded mount protos
		 * Clears all data and frees memory
		 */
		void Destroy();

		/**
		 * Lookup mount proto by vnum
		 * @param dwVnum Mount vnum (e.g., 20110, 20201, etc.)
		 * @return Pointer to CMountProto or NULL if not found
		 */
		const CMountProto* Get(DWORD dwVnum) const;

		/**
		 * Lookup mount proto by name
		 * @param name Internal mount name
		 * @return Pointer to CMountProto or NULL if not found
		 */
		const CMountProto* Get(const char* name) const;

		/**
		 * Get iterator to beginning of mount map
		 * @return Iterator to first mount
		 */
		iterator begin() { return m_map_pkMountByVnum.begin(); }

		/**
		 * Get iterator to end of mount map
		 * @return Iterator to end
		 */
		iterator end() { return m_map_pkMountByVnum.end(); }

	private:
		std::map<DWORD, CMountProto*>			m_map_pkMountByVnum;	// Lookup by vnum
		std::map<std::string, CMountProto*>		m_map_pkMountByName;	// Lookup by name
};

#endif