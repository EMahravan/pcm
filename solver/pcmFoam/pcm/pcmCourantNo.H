    scalar pcmCoNum = -great;

    forAll(pcmRegions, regionI)
    {
        pcmCoNum = max
        (
            compressibleCourantNo
            (
                pcmRegions[regionI],
                runTime,
                rhoPCM,
                phiPCM
            ),
            pcmCoNum
        );
    }
