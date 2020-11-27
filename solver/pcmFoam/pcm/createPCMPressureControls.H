PtrList<pressureControl> pressureControlPCM(pcmRegions.size());

forAll(pcmRegions, i)
{
    pressureControlPCM.set
    (
        i,
        new pressureControl
        (
            p_rghPCM,
            rhoPCM,
            pimples.pimple(i).dict(),
            false
        )
    );
}
