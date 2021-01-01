#include "createMeshes.H"

if (!fluidRegions.size() && !pcmRegions.size())
{
    FatalErrorIn(args.executable())
        << "No region meshes present" << exit(FatalError);
}

fvMesh& mesh = fluidRegions.size() ? fluidRegions[0] : pcmRegions[0];
