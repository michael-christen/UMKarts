# Created by Actel Designer Software 11.0.0.23
# Mon Nov 25 11:12:36 2013

# (OPEN DESIGN)

open_design "gc.adb"

# set default back-annotation base-name
set_defvar "BA_NAME" "gc_ba"
set_defvar "IDE_DESIGNERVIEW_NAME" {Impl1}
set_defvar "IDE_DESIGNERVIEW_COUNT" "1"
set_defvar "IDE_DESIGNERVIEW_REV0" {Impl1}
set_defvar "IDE_DESIGNERVIEW_REVNUM0" "1"
set_defvar "IDE_DESIGNERVIEW_ROOTDIR" {C:\Users\mttschlt\UMKarts\gc\designer}
set_defvar "IDE_DESIGNERVIEW_LASTREV" "1"


# import of input files
import_source  \
-format "edif" -edif_flavor "GENERIC" -netlist_naming "VERILOG" {../../synthesis/gc.edn} \
-format "pdc"  {..\..\component\work\gc\gc.pdc} -merge_physical "yes" -merge_timing "yes"
compile
report -type "status" {gc_compile_report.txt}

save_design
