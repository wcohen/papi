/* This file performs the following test: hardware info and which events are available */

#include "papi_test.h"
extern int TESTS_QUIET;         /* Declared in test_utils.c */


int main(int argc, char **argv)
{
   int i;
   int retval;
   int print_avail_only = 0;
   PAPI_event_info_t info;
   const PAPI_hw_info_t *hwinfo = NULL;

   tests_quiet(argc, argv);     /* Set TESTS_QUIET variable */
   for (i = 0; i < argc; i++)
      if (argv[i]) {
         if (strstr(argv[i], "-a"))
            print_avail_only = 1;
      }

   retval = PAPI_library_init(PAPI_VER_CURRENT);
   if (retval != PAPI_VER_CURRENT)
      test_fail(__FILE__, __LINE__, "PAPI_library_init", retval);

   if (!TESTS_QUIET) {
      retval = PAPI_set_debug(PAPI_VERB_ECONT);
      if (retval != PAPI_OK)
         test_fail(__FILE__, __LINE__, "PAPI_set_debug", retval);
   }

   if ((hwinfo = PAPI_get_hardware_info()) == NULL)
      test_fail(__FILE__, __LINE__, "PAPI_get_hardware_info", 2);

   if (!TESTS_QUIET) {
      printf("Test case 8: Available events and hardware information.\n");
      printf
          ("-------------------------------------------------------------------------\n");
      printf("Vendor string and code   : %s (%d)\n", hwinfo->vendor_string,
             hwinfo->vendor);
      printf("Model string and code    : %s (%d)\n", hwinfo->model_string, hwinfo->model);
      printf("CPU Revision             : %f\n", hwinfo->revision);
      printf("CPU Megahertz            : %f\n", hwinfo->mhz);
      printf("CPU's in this Node       : %d\n", hwinfo->ncpu);
      printf("Nodes in this System     : %d\n", hwinfo->nnodes);
      printf("Total CPU's              : %d\n", hwinfo->totalcpus);
      printf("Number Hardware Counters : %d\n", PAPI_get_opt(PAPI_MAX_HWCTRS, NULL));
      printf("Max Multiplex Counters   : %d\n", PAPI_MPX_DEF_DEG);
      printf
          ("-------------------------------------------------------------------------\n");

      if (print_avail_only) {
         printf("Name\t\tDerived\tDescription (Mgr. Note)\n");
      } else {
         printf("Name\t\tCode\t\tAvail\tDeriv\tDescription (Note)\n");
      }
      i = PRESET_MASK;
      do {
         if (PAPI_get_event_info(i, &info) == PAPI_OK) 
	   {
	     if (print_avail_only)
               {
		 printf("%s\t%s\t%s (%s)\n",
			info.symbol,
			(info.count > 1 ? "Yes" : "No"),
			info.long_descr, (info.vendor_name ? info.vendor_name : ""));
	       }
            else
	      {
		printf("%s\t0x%x\t%s\t%s\t%s (%s)\n",
		       info.symbol,
		       info.event_code,
		       (info.count ? "Yes" : "No"),
		       (info.count > 1 ? "Yes" : "No"),
		       info.long_descr, (info.vendor_name ? info.vendor_name : ""));
	      }
	   }
      } while (PAPI_enum_event(&i, print_avail_only) == PAPI_OK);
      printf
          ("-------------------------------------------------------------------------\n");
   }

   test_pass(__FILE__, NULL, 0);
   exit(1);
}