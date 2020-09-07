#!/bin/bash

function executeCommand {
    $@
    rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
    echo "Exit code[" $@ "]: $rc"
}

executeCommand "source /geant4-install/bin/geant4.sh"
executeCommand "source /root/bin/thisroot.sh"
executeCommand "export cadmesh_DIR=/CADMesh-install"
executeCommand "mkdir -p build"
executeCommand "cd build"
executeCommand "cmake .."
executeCommand "make"

