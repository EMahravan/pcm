    scalar fluidCoNum = -great;

    forAll(fluidRegions, regionI)
    {
        fluidCoNum = max
        (
            compressibleCourantNo
            (
                fluidRegions[regionI],
                runTime,
                rhoFluid[regionI],
                phiFluid[regionI]
            ),
            fluidCoNum
        );
    }
