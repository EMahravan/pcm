    volScalarField& rho = rhoPCM;
    volVectorField& U = UPCM;
    surfaceScalarField& phi = phiPCM;
    surfaceScalarField& rhoPhi=rhoPhiPCM;

    autoPtr<incompressible::turbulenceModel>& turbulence = turbulencePCM;
    immiscibleIncompressibleTwoPhaseMixture& mixture=mixturePCM;

    volScalarField& p = pPCM;
    const volScalarField& Cp = CpPCM;

    volScalarField& p_rgh = p_rghPCM;
    volScalarField& T = TPCM;

    const volScalarField& k = kPCM;
    volScalarField& alpha1 = alpha1PCM;

    const dimensionedVector& g = gPCM;
    const volScalarField& gh = ghPCM;
    const surfaceScalarField& ghf = ghfPCM;

    fv::options& fvOptions = fvOptionsPCM;

    pimpleNoLoopControl& pimple = pimples.pimple(i);

    pressureControl& pressureControl = pressureControlPCM[i];

    scalar cumulativeContErr = cumulativeContErrs[i];
