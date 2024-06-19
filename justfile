set shell := ["bash", "-c"]

################################################################################
# help
################################################################################
default: help

alias h := help
help:
    just -l -f {{justfile()}}

################################################################################
#
################################################################################
here := justfile_directory()

################################################################################
#
################################################################################
analysis_dir := (here) / "analysis"

delphes:
    g clone -b DelphesCMSFWLitePSWeight_from_3.5.0 git@github.com:delphes/delphes.git
    cd delphes
    make -j12

wrap:
    meson wrap install tomlplusplus
    meson wrap install argparse

setup:
    cd {{analysis_dir}} && meson setup --reconfigure --prefix {{here}} build

alias b := build
build:
    cd {{analysis_dir}} && meson compile -C build
    cd {{analysis_dir}} && meson install -C build

