/******************************************************************************
License: GNU General Public License v3.0 only
Copyright:
	(C) 2008 - 2021 Alexander Shaduri <ashaduri@gmail.com>
******************************************************************************/
/// \file
/// \author Alexander Shaduri
/// \ingroup applib
/// \weakgroup applib
/// @{

#ifndef ATA_STORAGE_PROPERTY_DESCR_H
#define ATA_STORAGE_PROPERTY_DESCR_H

#include "ata_storage_property.h"



class StoragePropertyProcessor {
	public:

		/// Set descriptions, warnings, etc... on properties, and return them.
		static std::vector<AtaStorageProperty> process_properties(std::vector<AtaStorageProperty> properties, AtaStorageAttribute::DiskType disk_type);

};




#endif

/// @}
