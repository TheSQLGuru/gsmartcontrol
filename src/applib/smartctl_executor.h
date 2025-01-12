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

#ifndef SMARTCTL_EXECUTOR_H
#define SMARTCTL_EXECUTOR_H

#include "local_glibmm.h"
#include <vector>

#include "async_command_executor.h"
#include "command_executor.h"
#include "hz/fs_ns.h"



/// Smartctl executor template.
template<class ExecutorSync>
class SmartctlExecutorGeneric : public ExecutorSync {
	public:

		/// Constructor
		SmartctlExecutorGeneric(const std::string& cmd, const std::string& cmdargs)
			: ExecutorSync(cmd, cmdargs)
		{
			this->construct();
		}


		/// Constructor
		SmartctlExecutorGeneric()
		{
			this->construct();
		}



	protected:

		/// Called by constructors
		void construct()
		{
			ExecutorSync::get_async_executor().set_exit_status_translator(&SmartctlExecutorGeneric::translate_exit_status);
			this->set_error_header(std::string(_("An error occurred while executing smartctl:")) + "\n\n");
		}


		enum {
			exit_cant_parse = 1 << 0,  ///< Smartctl error code bit
			exit_open_failed = 1 << 1,  ///< Smartctl error code bit
			exit_smart_failed = 1 << 2,  ///< Smartctl error code bit
			exit_disk_failing = 1 << 3,  ///< Smartctl error code bit
			exit_prefail_threshold = 1 << 4,  ///< Smartctl error code bit
			exit_threshold_in_past = 1 << 5,  ///< Smartctl error code bit
			exit_error_log = 1 << 6,  ///< Smartctl error code bit
			exit_self_test_log = 1 << 7  ///< Smartctl error code bit
		};


		/// Translate smartctl error code to a readable message
		static std::string translate_exit_status(int status)
		{
			static const std::vector<std::string> table = {
				_("Command line did not parse."),
				_("Device open failed, or device did not return an IDENTIFY DEVICE structure."),
				_("Some SMART command to the disk failed, or there was a checksum error in a SMART data structure"),
				_("SMART status check returned \"DISK FAILING\""),
				_("SMART status check returned \"DISK OK\" but some prefail Attributes are less than threshold."),
				_("SMART status check returned \"DISK OK\" but we found that some (usage or prefail) Attributes have been less than threshold at some time in the past."),
				_("The device error log contains records of errors."),
				_("The device self-test log contains records of errors.")
			};

			std::string str;

			// check every bit
			for (unsigned int i = 0; i <= 7; i++) {
				if ( (status & (1 << i)) != 0 ) {
					if (!str.empty())
						str += "\n";  // new-line-separate each entry
					str += table[i];
				}
			}

			return str;
		}


		/// Import the last error from command executor and clear all errors there
		void import_error() override
		{
			AsyncCommandExecutor& cmdex = this->get_async_executor();

			AsyncCommandExecutor::error_list_t errors = cmdex.get_errors();  // these are not clones

			hz::ErrorBase* e = nullptr;
			// find the last relevant error.
			for (auto iter = errors.crbegin(); iter != errors.crend(); ++iter) {
				// ignore iochannel errors, they may mask the real errors
				if ((*iter)->get_type() != "giochannel" && (*iter)->get_type() != "custom") {
					e = (*iter)->clone();
					break;
				}
			}

			cmdex.clear_errors();  // and clear them

			if (e) {  // if error is present, alert the user
				on_error_warn(e);
			}
		}



		/// This is called when an error occurs in command executor.
		/// Note: The warnings are already printed via debug_* in cmdex.
		void on_error_warn(hz::ErrorBase* e) override
		{
			if (!e)
				return;

			// import the error only if it's relevant.
			const std::string error_type = e->get_type();
			// accept all errors by default, except:

			// Treat most exit codes as non-errors.
			if (error_type == "exit") {
				int exit_code = 0;
				e->get_code(exit_code);
				// Ignore everyone except this.
				// Note that we don't treat exit_open_failed as failure because:
				// * It may be returned from a DVD that returns product info but has no disk inside;
				// * It may be returned from a usb flash drive with -d scsi;
				// * exit_cant_parse is returned when opening unsupported usb drives without -d scsi.
				if ( !(exit_code & exit_cant_parse) )
					return;

				// ignore giochannel errors - higher level errors will be triggered, and they more user-friendly.
			} else if (error_type == "giochannel" || error_type == "custom") {
				return;
			}

			this->set_error_msg(e->get_message());
		}

};




/// Smartctl executor without GUI support
using SmartctlExecutor = SmartctlExecutorGeneric<CommandExecutor>;



/// Get smartctl binary (from config, etc...). Returns an empty string if not found.
hz::fs::path get_smartctl_binary();


/// Execute smartctl on device \c device.
/// \return error message on error, empty string on success.
std::string execute_smartctl(const std::string& device, const std::string& device_opts,
		const std::string& command_options,
		std::shared_ptr<CommandExecutor> smartctl_ex, std::string& smartctl_output);





#endif

/// @}
