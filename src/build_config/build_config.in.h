/******************************************************************************
License: BSD Zero Clause License
Copyright:
	(C) 2008 - 2021 Alexander Shaduri <ashaduri@gmail.com>
******************************************************************************/
/// \file
/// \author Alexander Shaduri
/// \ingroup build_config
/// \weakgroup build_config
/// @{

#ifndef BUILD_CONFIG_H
#define BUILD_CONFIG_H

// NOTE: Parts of this file are replaced by cmake

#include <stdexcept>

#cmakedefine CONFIG_KERNEL_WINDOWS32
#cmakedefine CONFIG_KERNEL_WINDOWS64
#cmakedefine CONFIG_KERNEL_LINUX
#cmakedefine CONFIG_KERNEL_FREEBSD
#cmakedefine CONFIG_KERNEL_OPENBSD
#cmakedefine CONFIG_KERNEL_NETBSD
#cmakedefine CONFIG_KERNEL_DRAGONFLY
#cmakedefine CONFIG_KERNEL_SOLARIS
#cmakedefine CONFIG_KERNEL_DARWIN
#cmakedefine CONFIG_KERNEL_QNX

#if defined CONFIG_KERNEL_WINDOWS32 || defined CONFIG_KERNEL_WINDOWS64
	#define CONFIG_KERNEL_FAMILY_WINDOWS
#endif


/// Build environment constexpr variables
struct BuildEnv {

	/// See if this is a debug build.
	static constexpr bool debug_build()
	{
	#ifdef DEBUG_BUILD
		return true;
	#else
		return false;
	#endif
	}

	/// CMake package name
	static constexpr const char* package_name() { return "@CMAKE_PROJECT_NAME@"; }

	/// CMake package version
	static constexpr const char* package_version() { return "@CMAKE_PROJECT_VERSION@"; }

	/// pkgdata directory - /usr/share
	static constexpr const char* package_pkgdata_dir() { return "@CMAKE_INSTALL_FULL_DATADIR@"; }

	/// sysconf directory - /etc
	static constexpr const char* package_sysconf_dir() { return "@CMAKE_INSTALL_FULL_SYSCONFDIR@"; }

	/// locale directory - /usr/share/locale
	static constexpr const char* package_locale_dir() { return "@CMAKE_INSTALL_FULL_LOCALEDIR@"; }

	/// doc directory - /usr/share/doc/package_name
	static constexpr const char* package_doc_dir() { return "@CMAKE_INSTALL_FULL_DOCDIR@"; }

	/// Top source directory. Available in debug build only.
	static constexpr const char* package_top_source_dir()
	{
	#ifdef DEBUG_BUILD
		// Hide it in ifdef so that the binary does not contain it unless required.
		return "@CMAKE_SOURCE_DIR@";
	#else
		throw std::logic_error("This function is unavailable in non-debug builds.");
	#endif
	}


	/// Check if target kernel is 32-bit Windows
	static constexpr bool is_kernel_windows32()
	{
	#ifdef CONFIG_KERNEL_WINDOWS32
		return true;
	#else
		return false;
	#endif
	}

	/// Check if target kernel is 64-bit Windows
	static constexpr bool is_kernel_windows64()
	{
	#ifdef CONFIG_KERNEL_WINDOWS64
		return true;
	#else
		return false;
	#endif
	}

	/// Check if target kernel is Windows.
	/// Note: This slightly differs from _WIN32 - the macro indicates that win32 API is available.
	static constexpr bool is_kernel_family_windows()
	{
		return is_kernel_windows32() || is_kernel_windows64();
	}


	/// Check if target kernel is Linux.
	static constexpr bool is_kernel_linux()
	{
	#ifdef CONFIG_KERNEL_LINUX
		return true;
	#else
		return false;
	#endif
	}

	/// Check if target kernel is FreeBSD.
	static constexpr bool is_kernel_freebsd()
	{
	#ifdef CONFIG_KERNEL_FREEBSD
		return true;
	#else
		return false;
	#endif
	}

	/// Check if target kernel is OpenBSD.
	static constexpr bool is_kernel_openbsd()
	{
	#ifdef CONFIG_KERNEL_OPENBSD
		return true;
	#else
		return false;
	#endif
	}

	/// Check if target kernel is NetBSD.
	static constexpr bool is_kernel_netbsd()
	{
	#ifdef CONFIG_KERNEL_NETBSD
		return true;
	#else
		return false;
	#endif
	}

	/// Check if target kernel is DragonflyBSD.
	static constexpr bool is_kernel_dragonfly()
	{
	#ifdef CONFIG_KERNEL_DRAGONFLY
		return true;
	#else
		return false;
	#endif
	}

	/// Check if target kernel is Solaris.
	static constexpr bool is_kernel_solaris()
	{
	#ifdef CONFIG_KERNEL_SOLARIS
		return true;
	#else
		return false;
	#endif
	}

	/// Check if target kernel is Darwin.
	static constexpr bool is_kernel_darwin()
	{
	#ifdef CONFIG_KERNEL_DARWIN
		return true;
	#else
		return false;
	#endif
	}

	/// Check if target kernel is QNX.
	static constexpr bool is_kernel_qnx()
	{
	#ifdef CONFIG_KERNEL_QNX
		return true;
	#else
		return false;
	#endif
	}

};




#endif

/// @}
