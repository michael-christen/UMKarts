new_project \
    -name {gc} \
    -location {C:\Users\knekritz\Desktop\gc\gc\designer\impl1\gc_fp} \
    -mode {single}
set_device_type -type {A2F200M3F}
set_device_package -package {484 FBGA}
update_programming_file \
    -feature {prog_fpga:on} \
    -fdb_source {fdb} \
    -fdb_file {C:\Users\knekritz\Desktop\gc\gc\designer\impl1\gc.fdb} \
    -feature {prog_from:off} \
    -feature {prog_nvm:on} \
    -efm_content {location:0;source:efc} \
    -efm_block {location:0;config_file:{C:\Users\knekritz\Desktop\gc\gc\component\work\gc_MSS\MSS_ENVM_0\MSS_ENVM_0.efc}} \
    -pdb_file {C:\Users\knekritz\Desktop\gc\gc\designer\impl1\gc_fp\gc.pdb}
set_programming_action -action {PROGRAM}
catch {run_selected_actions} return_val
save_project
close_project
if { $return_val != 0 } {
  exit 1 }
