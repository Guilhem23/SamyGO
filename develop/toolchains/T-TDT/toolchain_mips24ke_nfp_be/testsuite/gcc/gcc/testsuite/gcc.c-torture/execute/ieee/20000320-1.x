if {[istarget "m68k-*-linux-gnu*"] && [check_effective_target_coldfire_fpu]} {
    # ColdFire FPUs require software handling of subnormals.  Linux 2.6.10
    # does not have this.
    set torture_execute_xfail "m68k-*-linux-gnu*"
}

# Make sure ULLONG_MAX is available.
set additional_flags "-std=c99"
return 0
