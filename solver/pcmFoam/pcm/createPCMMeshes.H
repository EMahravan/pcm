    const wordList pcmNames(rp["pcm"]);

    PtrList<fvMesh> pcmRegions(pcmNames.size());

    forAll(pcmNames, i)
    {
        Info<< "Create pcm mesh for region " << pcmNames[i]
            << " for time = " << runTime.timeName() << nl << endl;

        pcmRegions.set
        (
            i,
            new fvMesh
            (
                IOobject
                (
                    pcmNames[i],
                    runTime.timeName(),
                    runTime,
                    IOobject::MUST_READ
                )
            )
        );
    }
