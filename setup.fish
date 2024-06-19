#!/usr/bin/env fish
set -xg PROJECT_PREFIX (dirname (readlink -m (status --current-filename)))
fish_add_path -g {$PROJECT_PREFIX}/bin
set -xag LD_LIBRARY_PATH {$PROJECT_PREFIX}/lib64

set -xg DELPHES_PREFIX {$PROJECT_PREFIX}/delphes
fish_add_path -g $DELPHES_PREFIX
set -xag LD_LIBRARY_PATH {$DELPHES_PREFIX}
set -xag ROOT_INCLUDE_PATH {$DELPHES_PREFIX}
set -xag ROOT_INCLUDE_PATH {$DELPHES_PREFIX}/external

micromamba activate delphes-analysis-py311
