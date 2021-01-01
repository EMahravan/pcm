pimpleMultiRegionControl pimplesPCM(pcmRegions, fluidRegions);
fvMesh& mesh=pcmRegions[0];
Info<< "Reading transportProperties\n" << endl;

Info<< "Reading field p_rgh\n" << endl;

volScalarField p_rghPCM
(
    IOobject
    (
        "p_rgh",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

volScalarField TPCM
(
    IOobject
    (
        "T",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "Reading field U\n" << endl;
volVectorField UPCM
(
    IOobject
    (
        "U",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "Reading/calculating face flux field phi\n" << endl;

surfaceScalarField phiPCM
(
    IOobject
    (
        "phi",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
    ),
    fvc::flux(UPCM)
);
Info<<"Calling for making immiscibleIncompressibleTwoPhaseMixture"<<endl;
immiscibleIncompressibleTwoPhaseMixture mixturePCM(UPCM, phiPCM);


volScalarField& alpha1PCM(mixturePCM.alpha1());
volScalarField& alpha2PCM(mixturePCM.alpha2());

const dimensionedScalar& rhoPCM1 = mixturePCM.rho1();
const dimensionedScalar& rhoPCM2 = mixturePCM.rho2();

const volScalarField& nuPCM = mixturePCM.nu();
const volScalarField& kPCM = mixturePCM.k();
const volScalarField& LPCM = mixturePCM.L();


// Need to store rho for ddt(rho, U)
volScalarField rhoPCM
(
    IOobject
    (
        "rho",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT
    ),
    alpha1PCM*rhoPCM1 + alpha2PCM*rhoPCM2
);

rhoPCM.oldTime();

// Need to store rho for ddt(rho, U)
const volScalarField& CpPCM=mixturePCM.Cp();
// Mass flux
surfaceScalarField rhoPhiPCM
(
    IOobject
    (
        "rhoPhi",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    fvc::interpolate(rhoPCM)*phiPCM
);


// Construct incompressible turbulence model
autoPtr<incompressible::turbulenceModel> turbulencePCM
(
    incompressible::turbulenceModel::New(UPCM, phiPCM, mixturePCM)
);

Info<< "\nReading hRef for PCM" << endl;
uniformDimensionedScalarField hRefPCM
(
    IOobject
    (
        "hRef",
        runTime.constant(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::NO_WRITE
    ),
    dimensionedScalar(dimLength, 0)
);

Info<< "\nReading g for PCM" << endl;
    
uniformDimensionedVectorField gPCM
(
    IOobject
    (
       "g",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ,
        IOobject::NO_WRITE
    )
);

Info<< "Calculating field g.h\n" << endl;

dimensionedScalar ghRefPCM(- mag(gPCM)*hRefPCM);
volScalarField ghPCM("ghPCM", (gPCM & mesh.C()) - ghRefPCM);
surfaceScalarField ghfPCM("ghf", (gPCM & mesh.Cf()) - ghRefPCM);

volScalarField pPCM
(
    IOobject
    (
        "p",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    p_rghPCM + rhoPCM*ghPCM
);

label pRefCell = 0;
scalar pRefValue = 0.0;

{

Info<<pimplesPCM.pimple(0).dict()<<endl;
    setRefCell
    (
        pPCM,
        p_rghPCM,
        pimplesPCM.pimple(0).dict(),
        pRefCell,
        pRefValue
    );//This leads to compile error since pimple is not yet defined
}

if (p_rghPCM.needReference())
{
    pPCM += dimensionedScalar
    (
        "p",
        pPCM.dimensions(),
        pRefValue - getRefCellValue(pPCM, pRefCell)
    );
    
    p_rghPCM = pPCM - rhoPCM*ghPCM;
}

mesh.setFluxRequired(p_rghPCM.name());
mesh.setFluxRequired(alpha1PCM.name());

fv::options& fvOptionsPCM(fv::options::New(mesh));

if (!fvOptionsPCM.optionList::size())
{
    Info << "No finite volume options present" << endl;
}

