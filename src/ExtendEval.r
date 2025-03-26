#include "AEConfig.h"

#ifndef AE_OS_WIN
#include "AE_General.r"
#endif
resource 'PiPL' (16000){
	{	
		Kind     { AEGP },			  					  
		Name     { "ExtendEval" },		  				 	  
		Category { "General Plugin" },					 
		Version  { 0x10000 },
	#ifdef AE_OS_WIN
		#ifdef AE_PROC_INTELx64
			CodeWin64X86 { "master" },
		#endif
	#else
		#ifdef AE_OS_MAC
			CodeMacIntel64 { "master" },
			CodeMacARM64 { "master" },
		#endif
	#endif
	}
};
